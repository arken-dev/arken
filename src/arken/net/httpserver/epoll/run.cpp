// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Pure C++17 / Linux epoll backend, no external event library.
// Same threading model as the libev/libevent backends: each thread
// owns an independent epoll instance and competes for accept() on the
// shared listening socket.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include <array>
#include <functional>
#include <unordered_map>
#include <system_error>
#include <vector>
#include <thread>

#include <arken/net/httpserver.h>
#include <arken/mvm.h>
#include <arken/base>

using HttpServer = arken::net::HttpServer;

/* client number limitation */
#define MAX_CLIENTS 1000

/* message length limitation */
#define MAX_MESSAGE_LEN (4096)

/* record the number of clients */
static int client_number;

/* record fd for close with SIGTERM */
static int fd;

//-----------------------------------------------------------------------------
// EVENT LOOP
//-----------------------------------------------------------------------------

class EventLoop
{
  public:
  using Callback = std::function<void(int fd, uint32_t events)>;

  EventLoop() : m_epfd(epoll_create1(EPOLL_CLOEXEC))
  {
    if (m_epfd < 0) {
      throw std::system_error(errno, std::generic_category(), "epoll_create1");
    }
  }

  ~EventLoop()
  {
    close(m_epfd);
  }

  EventLoop(const EventLoop &) = delete;
  EventLoop & operator=(const EventLoop &) = delete;

  void add(int fd, uint32_t events, Callback cb)
  {
    m_callbacks[fd] = std::move(cb);
    epoll_event ev{};
    ev.events  = events;
    ev.data.fd = fd;
    epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev);
  }

  void remove(int fd)
  {
    epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, nullptr);
    m_callbacks.erase(fd);
  }

  void run()
  {
    std::array<epoll_event, 64> events;
    m_running = true;

    while (m_running) {
      int n = epoll_wait(m_epfd, events.data(), static_cast<int>(events.size()), -1);
      if (n < 0) {
        if (errno == EINTR) {
          continue;
        }
        break;
      }
      for (int i = 0; i < n; i++) {
        auto it = m_callbacks.find(events[i].data.fd);
        if (it != m_callbacks.end()) {
          it->second(events[i].data.fd, events[i].events);
        }
      }
    }
  }

  void stop()
  {
    m_running = false;
  }

  private:
  int m_epfd;
  std::unordered_map<int, Callback> m_callbacks;
  bool m_running = false;
};

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

  return fd;
}

//-----------------------------------------------------------------------------
// READ CALLBACK
//-----------------------------------------------------------------------------

static void
read_cb(EventLoop *loop, int fd, uint32_t events)
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
    auto size = static_cast<ssize_t>(data.size());
    ssize_t bytes = write(fd, result, size);
    while( bytes < size ) {
      if (bytes == -1) {
        puts("write error");
        break;
      }
      bytes += write(fd, result+bytes, size-bytes);
    }
  } else if ((ret < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  } else {
    --client_number;
    loop->remove(fd);
    close(fd);
  }
}

//-----------------------------------------------------------------------------
// ACCEPT CALLBACK
//-----------------------------------------------------------------------------

static void
accept_cb(EventLoop *loop, int fd, uint32_t events)
{
  int connfd = accept(fd, nullptr, nullptr);
  if (connfd > 0) {
    if (++client_number > MAX_CLIENTS) {
      close(connfd);
      --client_number;
    } else {
      loop->add(connfd, EPOLLIN, [loop](int cfd, uint32_t ev) {
        read_cb(loop, cfd, ev);
      });
    }
  } else if ((connfd < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  } else {
    close(fd);
    loop->stop();
    /* this will lead main to exit, no need to free watchers of clients */
  }
}

//-----------------------------------------------------------------------------
// WORKING
//-----------------------------------------------------------------------------

static void
working(int fd)
{
  EventLoop loop;

  loop.add(fd, EPOLLIN, [&loop](int lfd, uint32_t ev) {
    accept_cb(&loop, lfd, ev);
  });

  loop.run();
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
// HTTP SERVER RUN
//-----------------------------------------------------------------------------

void HttpServer::run()
{
  std::cout << "start arken.net.HttpServer (epoll) " << m_address <<
    ":" << m_port << " (" << m_threads << ") threads..." << std::endl;

  // avoid process death when writing to a socket the peer already closed
  signal(SIGPIPE, SIG_IGN);

  start_server(m_address, m_port, m_threads);
}
