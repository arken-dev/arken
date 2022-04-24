// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
//
// This is a modified version of the "simpler ROT13 server with
// Libevent" from:
// http://www.wangafu.net/~nickm/libevent-book/01_intro.html

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

#include <vector>
#include <thread>

#include <event2/event.h>
#include <event2/thread.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <arken/net/httpserver.h>
#include <arken/mvm.h>
#include <arken/base>

#define ARKEN_NET_HTTPSERVER_LIBEVENT_PERSIST 0
#define ARKEN_NET_HTTPSERVER_LIBEVENT_DEBUG   0

using HttpServer = arken::net::HttpServer;

/* client number limitation */
#define MAX_CLIENTS 1000

/* message length limitation */
#define MAX_MESSAGE_LEN (4096)

/* message length limitation */
#define MAX_LINE 4096

/* record the number of clients */
static int client_number;

/* record fd for close with SIGTERM */
static int fd;

//-----------------------------------------------------------------------------
// CREATE SERVER
//-----------------------------------------------------------------------------

static int create_serverfd(char const *addr, uint16_t port)
{
  //setvbuf(stdout, NULL, _IONBF, 0);

  struct sockaddr_in server;

  fd = socket(AF_INET, SOCK_STREAM, 0);

  if (fd < 0) {
    std::cerr << "socket err\n";
    throw;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(port);
  //server.sin_addr.s_addr = inet_addr(addr);
  inet_pton(AF_INET, addr, &server.sin_addr);

  if (bind(fd, (struct sockaddr *)&server, sizeof(server)) < 0) { //NOLINT
    std::cerr << "bind err\n";
    throw;
  }

  if (listen(fd, 10) < 0) {
    std::cerr << "listen err\n";
    throw;
  }

  // set nonblock flag
  if( fd > 0 ) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
  }

  return fd;
}

//-----------------------------------------------------------------------------
// READ CALLBACK
//-----------------------------------------------------------------------------

void read_cb(int fd, short events, void *arg)
{
  char buf[MAX_MESSAGE_LEN+1] = {0};
  ssize_t ret;
  std::string tmp;

  do {
    ret = recv(fd, buf, MAX_MESSAGE_LEN, MSG_DONTWAIT);
    if( ret < 0 ) {
      break;
    }
    tmp.append(buf, ret);
  } while(ret == MAX_MESSAGE_LEN);

  if (ret > 0) {
    std::string data = HttpServer::handler(tmp.c_str(), tmp.size());
    const char * result = data.c_str();
    auto size  = static_cast<ssize_t>(data.size());
    ssize_t bytes = write(fd, result, size);
    while( bytes < size ) {
      if (bytes == -1) {
        puts("write error");
        break;
      }
      bytes += write(fd, result+bytes, size-bytes);
    }
    #if ARKEN_NET_HTTPSERVER_LIBEVENT_PERSIST == 0
    // not works without close socket and read_cb without EV_PERSIST :(
    #if ARKEN_NET_HTTPSERVER_LIBEVENT_DEBUG
      std::cout << "close client" << std::endl;
    #endif
    --client_number;
    close(fd);
    #endif
  } else if ((ret < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  } else {
    --client_number;
    close(fd);
  }

}

//-----------------------------------------------------------------------------
// ACCEPT CALLBACK
//-----------------------------------------------------------------------------

static void
accept_cb(int fd, short event, void *arg)
{
  int sockfd;
  //evutil_socket_t sockfd;
  //struct sockaddr_in client;
  //socklen_t len = sizeof(client);
  sockfd = accept(fd, nullptr, nullptr);//(struct sockaddr*)&client, &len);
  if (sockfd > 0) {
    if (++client_number > MAX_CLIENTS) {
      std::cout << "max clients" << std::endl;
      close(fd); // or close sockfd ???
    } else {
      auto base = static_cast<event_base*>(arg);
      struct event *ev = event_new(nullptr, -1, 0, nullptr, nullptr);
      // not works without close socket and read_cb without EV_PERSIST :(
      #if ARKEN_NET_HTTPSERVER_LIBEVENT_PERSIST == 0
      #if ARKEN_NET_HTTPSERVER_LIBEVENT_DEBUG
        std::cout << "accept EV_READ" << std::endl;
      #endif
      event_assign(ev, base, sockfd, EV_READ, read_cb, (void*)ev);
      #else
      #if ARKEN_NET_HTTPSERVER_LIBEVENT_DEBUG
        std::cout << "accept EV_READ | EV_PERSIST" << std::endl;
      #endif
      event_assign(ev, base, sockfd, EV_READ | EV_PERSIST, read_cb, (void*)ev);
      #endif
      event_add(ev, nullptr);
    }
  } else if ((sockfd < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  } else {
    close(sockfd);
  }
}

//-----------------------------------------------------------------------------
// WORKING
//-----------------------------------------------------------------------------

static void
working( int fd )
{
  struct event_base *base;
  struct event *listener_event;

  base = event_base_new();
  if (!base) {
    // TODO error
    std::cout << "error ..." << std::endl;
    return;
  }

  listener_event = event_new(base, fd, EV_READ | EV_PERSIST, accept_cb, (void *)base);

  /* check it? */
  event_add(listener_event, nullptr);

  event_base_dispatch(base);
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

static void
signal_handler(int signo)
{
  switch (signo) {
    case SIGINT:
    case SIGTERM:
      if( fd > 0 ) {
        std::cout << "arken.net.HttpServer (libevent) close socket" << std::endl;
        close(fd);
        exit(0);
      }
      break;
    default:
      // unreachable
      break;
  }
}

//-----------------------------------------------------------------------------
// HTTP SERVER RUN
//-----------------------------------------------------------------------------

void HttpServer::run()
{
  #ifdef WIN32
  evthread_use_windows_threads();
  #endif
  #ifdef _EVENT_HAVE_PTHREADS
  evthread_use_pthreads();
  #endif

  std::cout << "start arken.net.HttpServer (libevent) " << m_address <<
     ":" << m_port << " (" << m_threads << ") threads..." << std::endl;

  signal(SIGTERM, signal_handler);
  signal(SIGINT,  signal_handler);

  start_server(m_address, m_port, m_threads);
}
