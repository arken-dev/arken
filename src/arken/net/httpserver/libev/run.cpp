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
#include <strings.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <thread>

#include <ev.h>

#include <arken/net/httpserver.h>
#include <arken/mvm.h>
#include <arken/base>

/* client number limitation */
#define MAX_CLIENTS 1000

/* message length limitation */
#define MAX_MESSAGE_LEN (4096)

#define err_message(msg) \
    do {perror(msg); exit(EXIT_FAILURE);} while(0)

namespace arken {
namespace net {



/* record the number of clients */
static int client_number;

static void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);
static void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents);

static int create_serverfd(char const *addr, uint16_t u16port)
{
    int fd;
    struct sockaddr_in server;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
      std::cerr << "socket err\n";
      throw;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(u16port);
    inet_pton(AF_INET, addr, &server.sin_addr);

    if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
      std::cerr << "bind err\n";
      throw;
    }

    if (listen(fd, 10) < 0) {
      std::cerr << "listen err\n";
      throw;
    }

    return fd;
}

static void read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
    char buf[MAX_MESSAGE_LEN+1] = {0};
    ssize_t ret = recv(watcher->fd, buf, MAX_MESSAGE_LEN, MSG_DONTWAIT);
    std::string data;
    if( ret < MAX_MESSAGE_LEN ) {
        buf[ret] = '\0';
        data = HttpServer::handler(buf, sizeof(buf));
    } else {
      std::string tmp;
      tmp.append(buf, ret);
      do {
        ret = recv(watcher->fd, buf, MAX_MESSAGE_LEN, MSG_DONTWAIT);
        tmp.append(buf, ret);
      } while(ret == MAX_MESSAGE_LEN);
      data = HttpServer::handler(tmp.c_str(), tmp.size());
    }

    if (ret > 0) {
        const char * result = data.c_str();
        ssize_t      size   = (ssize_t) data.size();
        //ssize_t write(int fildes, const void *buf, size_t nbyte);
        ssize_t bytes = write(watcher->fd, result, size);
        while( bytes < size ) {
          /*
          if (bytes == -1) {
              puts("write error");
              break;
          }
          */
          bytes += write(watcher->fd, result+bytes, size-bytes);
        }
    } else if ((ret < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
        return;
    } else {
        --client_number;
        ev_io_stop(loop, watcher);
        close(watcher->fd);
        free(watcher);
    }
}

static void accept_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
    int connfd;
    ev_io *client;

    connfd = accept(watcher->fd, NULL, NULL);
    if (connfd > 0) {
        if (++client_number > MAX_CLIENTS) {
            close(watcher->fd);

        } else {
            // cast ...
            client = (ev_io *) calloc(1, sizeof(*client));
            ev_io_init(client, read_cb, connfd, EV_READ);
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

void working(int fd)
{
  //struct ev_loop *loop = EV_DEFAULT; //ev_default_loop(EVFLAG_NOENV);
  struct ev_loop * loop = ev_loop_new(EVFLAG_NOENV);//EVBACKEND_EPOLL | EVFLAG_NOENV);
  ev_io *watcher = (ev_io *) calloc(1, sizeof(*watcher));
  assert("can not alloc memory\n");//, loop && watcher));

  /* set nonblock flag */
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);

  ev_io_init(watcher, accept_cb, fd, EV_READ);
  ev_io_start(loop, watcher);
  ev_run(loop, 0);

  ev_loop_destroy(loop);
  free(watcher);
}

static void start_server(char const *addr, uint16_t u16port, int threads)
{
  int fd = create_serverfd(addr, u16port);

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

static void signal_handler(int signo)
{
    switch (signo) {
        case SIGPIPE:
            break;
        default:
            // unreachable
            break;
    }
}

void HttpServer::run()
{
  std::cout << "start arken.net.HttpServer (libev) " << m_address <<
     ":" << m_port << " (" << m_threads << ") threads..." << std::endl;

  signal(SIGPIPE, signal_handler);
  start_server(m_address, m_port, m_threads);

}

} // namespace net
} // namespace arken