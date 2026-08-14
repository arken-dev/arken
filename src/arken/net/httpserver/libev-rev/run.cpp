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

#include <atomic>
#include <vector>
#include <thread>

#include <ev.h>

#include <arken/net/httpserver.h>
#include <arken/mvm.h>
#include <arken/base>

using HttpServer = arken::net::HttpServer;

/* client number limitation */
#define MAX_CLIENTS 1000

/* message length limitation */
#define MAX_MESSAGE_LEN (4096)

/* record the number of clients - acessado de todas as worker threads
 * (accept_cb incrementa, closeConnection decrementa, cada thread com seu
 * próprio ev_loop), por isso atomic em vez de int puro */
static std::atomic<int> client_number{0};

/* record fd for close with SIGTERM */
static int fd;

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
// CONNECTION
//-----------------------------------------------------------------------------

// io precisa ser o primeiro membro: o libev só conhece struct ev_io*, então
// todo callback recebe o endereço de connection->io de volta - como é o
// primeiro membro, esse endereço é o mesmo do Connection inteiro, e dá pra
// converter de volta com reinterpret_cast (mesma técnica do libev-ws).
struct Connection {
  ev_io       io;
  // buffer de saída - toda resposta HTTP acaba aqui e é drenada por
  // write_cb via EV_WRITE quando o socket fica pronto pra escrita; nunca
  // escrevemos sincronamente dentro do callback que gerou a resposta,
  // senão um cliente lento trava a thread inteira (todas as outras
  // conexões do mesmo ev_loop) esperando o kernel abrir espaço no buffer
  // de envio.
  std::string output;
  size_t      offset = 0;
  bool        writeWatcherActive = false;
};

static void
closeConnection(struct ev_loop *loop, Connection *connection)
{
  --client_number;
  ev_io_stop(loop, &connection->io);
  close(connection->io.fd);
  delete connection;
}

static void
enableWrite(struct ev_loop *loop, Connection *connection)
{
  if( connection->writeWatcherActive || connection->output.empty() ) {
    return;
  }
  ev_io_stop(loop, &connection->io);
  ev_io_set(&connection->io, connection->io.fd, EV_READ | EV_WRITE);
  ev_io_start(loop, &connection->io);
  connection->writeWatcherActive = true;
}

static void
disableWrite(struct ev_loop *loop, Connection *connection)
{
  if( ! connection->writeWatcherActive ) {
    return;
  }
  ev_io_stop(loop, &connection->io);
  ev_io_set(&connection->io, connection->io.fd, EV_READ);
  ev_io_start(loop, &connection->io);
  connection->writeWatcherActive = false;
}

// drena o output até esvaziar ou até o socket não aceitar mais (EAGAIN) -
// nunca bloqueia. Devolve false se a conexão foi fechada (erro real de
// escrita); quem chamou não deve tocar em `connection` de novo nesse caso.
static bool
write_cb(struct ev_loop *loop, Connection *connection)
{
  while( connection->offset < connection->output.size() ) {
    const char * data      = connection->output.data() + connection->offset;
    size_t       remaining = connection->output.size() - connection->offset;

    ssize_t sent = send(connection->io.fd, data, remaining, MSG_DONTWAIT | MSG_NOSIGNAL);

    if( sent > 0 ) {
      connection->offset += static_cast<size_t>(sent);
      continue;
    }

    if( sent < 0 && (errno == EAGAIN || errno == EWOULDBLOCK) ) {
      return true; // socket ainda não tá pronto - espera o próximo EV_WRITE
    }

    closeConnection(loop, connection);
    return false;
  }

  connection->output.clear();
  connection->offset = 0;
  disableWrite(loop, connection);
  return true;
}

//-----------------------------------------------------------------------------
// READ CALLBACK
//-----------------------------------------------------------------------------

static void
read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  auto * connection = reinterpret_cast<Connection *>(watcher);

  if( (revents & EV_WRITE) && ! write_cb(loop, connection) ) {
    return; // conexão foi fechada dentro do write_cb (erro real de escrita)
  }
  if( ! (revents & EV_READ) ) {
    return;
  }

  char buf[MAX_MESSAGE_LEN+1] = {0};
  ssize_t ret;
  std::string tmp;

  do {
    ret = recv(connection->io.fd, buf, MAX_MESSAGE_LEN, MSG_DONTWAIT);
    if( ret < 0 ) {
      break;
    }
    tmp.append(buf, ret);
  } while(ret == MAX_MESSAGE_LEN);


  if (ret > 0) {
    connection->output = HttpServer::handler(tmp.c_str(), tmp.size());
    connection->offset = 0;
    enableWrite(loop, connection);
  } else if ((ret < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  } else {
    closeConnection(loop, connection);
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
    // conexão aceita entra bloqueante por padrão - o send() não-bloqueante
    // em write_cb/closeConnection depende disso pra nunca travar a thread
    // (todas as outras conexões do mesmo ev_loop) mesmo se o cliente for
    // lento pra consumir a resposta
    int flags = fcntl(connfd, F_GETFL, 0);
    if( flags >= 0 ) {
      fcntl(connfd, F_SETFL, flags | O_NONBLOCK);
    }

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
    // watcher->fd é o socket de escuta compartilhado por todas as worker
    // threads (cada uma tem seu próprio ev_loop, mas todas dão accept()
    // no mesmo fd) - um erro aqui (ex: EMFILE/ENFILE, comum sob alta
    // concorrência) é local a esta thread; fechar watcher->fd derrubaria
    // o accept pra todas as outras threads, e ev_break só mata o loop
    // desta thread, deixando as demais rodando com o fd compartilhado já
    // fechado (accept() delas passa a falhar com EBADF em loop). Só para
    // de aceitar nesta thread - as outras continuam normalmente.
    fprintf(stderr, "arken.net.HttpServer (libev-rev): accept error (errno=%d), parando accept nesta thread\n", errno);
    ev_io_stop(loop, watcher);
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
  std::cout << "start arken.net.HttpServer (libev-rev) " << m_address <<
    ":" << m_port << " (" << m_threads << ") threads..." << std::endl;

  // avoid process death when writing to a socket the peer already closed
  signal(SIGPIPE, SIG_IGN);

  //signal(SIGTERM, signal_handler);
  //signal(SIGINT,  signal_handler);

  start_server(m_address, m_port, m_threads);
}
