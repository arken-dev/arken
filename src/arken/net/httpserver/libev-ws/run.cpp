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
#include <arken/mvm.h>
#include <arken/base>
#include <arken/digest/sha1.h>
#include <arken/base64.h>

using HttpServer = arken::net::HttpServer;
using HttpEnv    = arken::net::HttpEnv;
using sha1       = arken::digest::sha1;
using base64     = arken::base64;

/* RFC 6455 4.2.2 */
#define WS_GUID "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

//-----------------------------------------------------------------------------
// WEBSOCKET ACCEPT KEY (RFC 6455 4.2.2)
//-----------------------------------------------------------------------------

static std::string
websocket_accept_key(const std::string & key)
{
  std::string combined = key + WS_GUID;

  unsigned char * digest  = sha1::bytes(combined.data(), combined.size());
  char *          encoded = base64::encode(reinterpret_cast<const char *>(digest), 20);

  std::string result(encoded);

  delete[] digest;
  delete[] encoded;

  return result;
}

/* client number limitation */
#define MAX_CLIENTS 1000

/* message length limitation */
#define MAX_MESSAGE_LEN (4096)

/* record the number of clients */
static int client_number;

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
// READ CALLBACK
//-----------------------------------------------------------------------------

static void
read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
  char buf[MAX_MESSAGE_LEN+1] = {0};
  ssize_t ret;
  std::string tmp;

  do {
    ret = recv(watcher->fd, buf, MAX_MESSAGE_LEN, MSG_DONTWAIT);
    if( ret < 0 ) {
      break;
    }
    tmp.append(buf, ret);
  } while(ret == MAX_MESSAGE_LEN);


  if (ret > 0) {
    HttpEnv * env = new HttpEnv(tmp.c_str(), tmp.size(), false);

    std::string data;
    if( strcmp(env->field("Connection").data(), "Upgrade") == 0 &&
        strcmp(env->field("Upgrade").data(), "websocket") == 0 ) {
      std::string acceptKey = websocket_accept_key(env->field("Sec-WebSocket-Key").data());

      data.append(HttpServer::status(101));
      data.append("\r\n");
      data.append("Upgrade: websocket\r\n");
      data.append("Connection: Upgrade\r\n");
      data.append("Sec-WebSocket-Accept: ");
      data.append(acceptKey);
      data.append("\r\n\r\n");
    } else {
      data = HttpServer::handler(env);
    }

    delete env;

    const char * result = data.c_str();
    auto size = static_cast<ssize_t>(data.size());
    //ssize_t write(int fildes, const void *buf, size_t nbyte);
    ssize_t bytes = write(watcher->fd, result, size);
    while( bytes < size ) {
      if (bytes == -1) {
        puts("write error");
        break;
      }
      bytes += write(watcher->fd, result+bytes, size-bytes);
    }

  } else if ((ret < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  } else {
    --client_number;
    ev_io_stop(loop, watcher);
    close(watcher->fd);
    free(watcher); //NOLINT
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
      ev_io *client = (ev_io *) calloc(1, sizeof(*client)); //NOLINT
      ev_io_init(client, read_cb, connfd, EV_READ); //NOLINT ev_io_init is macro
      ev_io_start(loop, client);
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
