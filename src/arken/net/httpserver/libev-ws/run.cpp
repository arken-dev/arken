// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// https://developpaper.com/how-to-use-libev/
// http://para.se/perldoc/EV/libev.html
// https://libev.schmorp.narkive.com/7XetX0DX/using-libev-with-multiple-threads
// http://pod.tst.eu/http://cvs.schmorp.de/libev/ev.pod
// https://github.com/dexgeh/webserver-libev-httpparser.git
// https://blog.titanwolf.in/a?ID=01200-c35750d4-727b-474c-9c57-f539e92e1e28
// https://titanwolf.org/Network/Articles/Article?AID=c17ed0b7-7679-467e-b324-05def67552ff

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/ioctl.h>

#include <vector>
#include <thread>

#include <ev.h>

#include <arken/net/httpserver.h>
#include <arken/net/httpenv.h>
#include <arken/net/websocket.h>
#include <arken/mvm.h>
#include <arken/os.h>
#include <arken/base>

using HttpServer        = arken::net::HttpServer;
using HttpEnv           = arken::net::HttpEnv;
using WebSocketParser   = arken::net::WebSocketParser;
using WebSocketHandler  = arken::net::WebSocketHandler;
using WebSocketRegistry = arken::net::WebSocketRegistry;

/* client number limitation */
#define MAX_CLIENTS 1000

/* message length limitation */
#define MAX_MESSAGE_LEN (4096)

/* record the number of clients */
static int client_number;

/* record fd for close with SIGTERM */
static int fd;

//-----------------------------------------------------------------------------
// CONNECTION
//-----------------------------------------------------------------------------

enum class ConnectionType {
  HTTP,
  WEBSOCKET,
};

// io precisa ser o primeiro membro: o libev só conhece struct ev_io*, então
// todo callback recebe o endereço de connection->io de volta - como é o
// primeiro membro, esse endereço é o mesmo do Connection inteiro, e dá pra
// converter de volta com reinterpret_cast (mesma técnica do titan/HttpData).
struct Connection {
  ev_io           io;
  std::string     input;
  ConnectionType  type = ConnectionType::HTTP;
  WebSocketParser websocket;
  std::string     sessionId; // só existe depois do handshake (type == WEBSOCKET)
  std::string     path;      // idem - path do request que fez o upgrade

  // casca fina - a lógica/limiar de verdade mora em HttpServer, porque
  // esse Connection aqui é específico do libev-ws; um backend futuro
  // (libevent-ws, epoll-ws) tem o seu próprio Connection, mas delega pro
  // mesmo HttpServer::headerTooLarge()
  bool isMaxHttpHeaderSize() {
    return HttpServer::headerTooLarge(input.size());
  }
};

static void
writeAll(int fd, const char * data, size_t size)
{
  ssize_t bytes = write(fd, data, size);
  while( bytes < static_cast<ssize_t>(size) ) {
    if( bytes == -1 ) {
      puts("write error");
      return;
    }
    bytes += write(fd, data + bytes, size - bytes);
  }
}

static void
closeConnection(struct ev_loop *loop, Connection * connection)
{
  if( connection->type == ConnectionType::WEBSOCKET ) {
    WebSocketHandler::close(connection->io.fd, connection->sessionId, connection->path);
    WebSocketRegistry::remove(connection->sessionId);
  }

  --client_number;
  ev_io_stop(loop, &connection->io);
  close(connection->io.fd);
  delete connection;
}

//-----------------------------------------------------------------------------
// CREATE SERVER
//-----------------------------------------------------------------------------

static int
create_serverfd(char const *addr, uint16_t port)
{
  struct sockaddr_in server;

  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    std::cerr << "socket err\n";
    throw;
  }

  // https://www.baeldung.com/linux/socket-options-difference
  // https://stackoverflow.com/questions/24194961/how-do-i-use-setsockoptso-reuseaddr
  int optval = 1;
  if(setsockopt(fd, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
    std::cerr << "setsockopt fail\n";
    throw;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  inet_pton(AF_INET, addr, &server.sin_addr);

  if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0) { //NOLINT
    std::cerr << "bind err\n";
    throw;
  }

  if (listen(fd, 2048) < 0) {
    std::cerr << "listen err\n";
    throw;
  }

  // set nonblock flag
  if( fd > 0 ) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
  }

  // https://peps.python.org/pep-0446/
  int res;
  res = ioctl(fd, FIOCLEX, 0);
  if (!res) {
    //std::cerr << "ioctl fd FIOCLEX err\n";
    //throw;
  }

  return fd;
}

//-----------------------------------------------------------------------------
// READ CALLBACK
//-----------------------------------------------------------------------------

static void
processHttp(struct ev_loop *loop, Connection * connection)
{
  // espera os headers completos chegarem antes de tentar processar -
  // uma requisição (ou o handshake) pode vir fragmentada em vários recv()
  if( connection->input.find("\r\n\r\n") == std::string::npos ) {
    if( connection->isMaxHttpHeaderSize() ) {
      fprintf(stderr, "arken.net.HttpServer (libev-ws): headers grandes demais (%zu bytes), fechando conexão\n",
        connection->input.size());

      std::string response(HttpServer::status(431));
      response.append("\r\nConnection: close\r\nContent-Length: 0\r\n\r\n");
      writeAll(connection->io.fd, response.data(), response.size());

      closeConnection(loop, connection);
    }
    return;
  }

  HttpEnv * env = new HttpEnv(connection->input.data(), connection->input.size(), false);

  std::string data;
  bool isUpgrade = WebSocketParser::isWebSocketUpgrade(env);

  if( isUpgrade ) {
    std::string acceptKey = WebSocketParser::acceptKey(env->field("Sec-WebSocket-Key").data());

    data.append(HttpServer::status(101));
    data.append("\r\n");
    data.append("Upgrade: websocket\r\n");
    data.append("Connection: Upgrade\r\n");
    data.append("Sec-WebSocket-Accept: ");
    data.append(acceptKey);
    data.append("\r\n\r\n");

    connection->type      = ConnectionType::WEBSOCKET;
    connection->sessionId = arken::os::uuid();
    connection->path      = env->requestPath();
  } else {
    data = HttpServer::handler(env);
  }

  delete env;
  connection->input.clear();

  writeAll(connection->io.fd, data.data(), data.size());

  if( isUpgrade ) {
    WebSocketRegistry::add(connection->sessionId, connection->io.fd);
    WebSocketHandler::open(connection->io.fd, connection->sessionId, connection->path);
  }
}

static void
processWebSocket(struct ev_loop *loop, Connection * connection)
{
  connection->websocket.parse(connection->input.data(), connection->input.size());
  connection->input.clear();

  while( connection->websocket.hasOutput() ) {
    std::string out = connection->websocket.output();
    WebSocketRegistry::writeFrame(connection->sessionId, out);
  }

  while( connection->websocket.hasMessage() ) {
    auto message = connection->websocket.message();
    bool binary  = message.opcode == arken::net::WebSocketOpcode::Binary;
    WebSocketHandler::message(connection->io.fd, connection->sessionId, connection->path,
                               message.payload, binary);
  }

  if( connection->websocket.closed() ) {
    closeConnection(loop, connection);
  }
}

static void
read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  auto * connection = reinterpret_cast<Connection *>(watcher);

  char buf[MAX_MESSAGE_LEN];
  ssize_t ret;

  do {
    ret = recv(connection->io.fd, buf, MAX_MESSAGE_LEN, MSG_DONTWAIT);
    if( ret > 0 ) {
      connection->input.append(buf, ret);
    }
  } while( ret == MAX_MESSAGE_LEN );

  if( ret == 0 || (ret < 0 && errno != EAGAIN && errno != EWOULDBLOCK) ) {
    closeConnection(loop, connection);
    return;
  }

  if( connection->type == ConnectionType::WEBSOCKET ) {
    processWebSocket(loop, connection);
  } else {
    processHttp(loop, connection);
  }
}

//-----------------------------------------------------------------------------
// ACCEPT CALLBACK
//-----------------------------------------------------------------------------

static void
accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  int connfd = accept(watcher->fd, nullptr, nullptr);
  if (connfd > 0) {
    if (++client_number > MAX_CLIENTS) {
      close(connfd);
      --client_number;
    } else {
      Connection * connection = new Connection();
      ev_io_init(&connection->io, read_cb, connfd, EV_READ); //NOLINT ev_io_init is macro
      ev_io_start(loop, &connection->io);
    }
  } else if ((connfd < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  } else {
    close(watcher->fd);
    ev_break(loop, EVBREAK_ALL);
    /* this will lead main to exit, no need to free watchers of clients */
  }
}

//-----------------------------------------------------------------------------
// WORKING
//-----------------------------------------------------------------------------

static void
working(int fd)
{
  //struct ev_loop *loop = EV_DEFAULT; //ev_default_loop(EVFLAG_NOENV);
  struct ev_loop * loop = ev_loop_new(EVFLAG_NOENV);//EVBACKEND_EPOLL | EVFLAG_NOENV);
  ev_io *watcher = (ev_io *) calloc(1, sizeof(*watcher)); //NOLINT
  //assert("can not alloc memory\n");//, loop && watcher));

  ev_io_init(watcher, accept_cb, fd, EV_READ); //NOLINT ev_io_init is a macro
  ev_io_start(loop, watcher);
  ev_run(loop, 0);

  ev_loop_destroy(loop);
  free(watcher); //NOLINT
}

//-----------------------------------------------------------------------------
// START SERVER
//-----------------------------------------------------------------------------

static void
start_server(char const *addr, uint16_t port, int threads)
{
  int fd = create_serverfd(addr, port);

  std::vector<std::thread *> worker;

  for(int i=0; i < threads; i++) {
    worker.push_back(new std::thread(working, fd));
  }

  for(std::thread *t : worker) {
    t->join();
  }

  for(std::thread *t : worker) {
    delete t;
  }
}

//-----------------------------------------------------------------------------
// SIGNAL HANDLER
//-----------------------------------------------------------------------------

/*
static void
signal_handler(int signo)
{
  switch (signo) {
    case SIGINT:
      std::cout << std::endl;
    case SIGTERM:
      if( fd > 0 ) {
        std::cout << "arken.net.HttpServer (libev) close socket" << std::endl;
        close(fd);
        exit(0);
      }
      break;
    default:
      // unreachable
      break;
  }
}
*/

//-----------------------------------------------------------------------------
// HTTP SERVER RUN
//-----------------------------------------------------------------------------

void HttpServer::run()
{
  std::cout << "start arken.net.HttpServer (libev-ws) " << m_address <<
    ":" << m_port << " (" << m_threads << ") threads..." << std::endl;

  // avoid process death when writing to a socket the peer already closed
  signal(SIGPIPE, SIG_IGN);

  //signal(SIGTERM, signal_handler);
  //signal(SIGINT,  signal_handler);

  start_server(m_address, m_port, m_threads);
}
