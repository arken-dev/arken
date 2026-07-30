// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Linux io_uring backend (via liburing), no epoll/select at all.
// Same threading model as the libev/libevent/epoll backends: each thread
// owns an independent ring and competes for accept() on the shared
// listening socket. Unlike the readiness-based backends, every socket
// operation (accept/recv/send) is submitted as an SQE and handled from
// its CQE completion, so there is no separate "wait for readiness then
// call read()" step.
//
// https://unixism.net/loti/
// https://github.com/axboe/liburing

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include <array>
#include <string>
#include <vector>
#include <thread>

#include <liburing.h>

#include <arken/net/httpserver.h>
#include <arken/mvm.h>
#include <arken/base>

using HttpServer = arken::net::HttpServer;

/* client number limitation */
#define MAX_CLIENTS 1000

/* message length limitation */
#define MAX_MESSAGE_LEN (4096)

/* number of submission/completion queue entries per thread ring */
#define QUEUE_DEPTH 256

/* record the number of clients */
static int client_number;

/* record fd for close with SIGTERM */
static int fd;

//-----------------------------------------------------------------------------
// CONNECTION / REQUEST STATE
//-----------------------------------------------------------------------------

enum class OpType { Accept, Read, Write };

struct Connection
{
  int fd = -1;
  std::array<char, MAX_MESSAGE_LEN> readbuf{};
  std::string request;
  std::string response;
  size_t sent = 0;
};

struct IoRequest
{
  OpType type;
  Connection *conn = nullptr; // null for the reusable Accept request
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

  return fd;
}

//-----------------------------------------------------------------------------
// SUBMISSION HELPERS
//-----------------------------------------------------------------------------

static void
submit_accept(io_uring *ring, IoRequest *req, int listenfd)
{
  io_uring_sqe *sqe = io_uring_get_sqe(ring);
  io_uring_prep_accept(sqe, listenfd, nullptr, nullptr, 0);
  io_uring_sqe_set_data(sqe, req);
  io_uring_submit(ring);
}

static void
submit_recv(io_uring *ring, Connection *conn)
{
  auto *req = new IoRequest{OpType::Read, conn}; //NOLINT
  io_uring_sqe *sqe = io_uring_get_sqe(ring);
  io_uring_prep_recv(sqe, conn->fd, conn->readbuf.data(), conn->readbuf.size(), 0);
  io_uring_sqe_set_data(sqe, req);
  io_uring_submit(ring);
}

static void
submit_send(io_uring *ring, Connection *conn)
{
  auto *req = new IoRequest{OpType::Write, conn}; //NOLINT
  io_uring_sqe *sqe = io_uring_get_sqe(ring);
  io_uring_prep_send(sqe, conn->fd, conn->response.data() + conn->sent,
    conn->response.size() - conn->sent, 0);
  io_uring_sqe_set_data(sqe, req);
  io_uring_submit(ring);
}

static void
close_connection(Connection *conn)
{
  --client_number;
  close(conn->fd);
  delete conn; //NOLINT
}

//-----------------------------------------------------------------------------
// COMPLETION HANDLERS
//-----------------------------------------------------------------------------

static void
handle_accept(io_uring *ring, IoRequest *req, int listenfd, int res)
{
  if (res >= 0) {
    if (++client_number > MAX_CLIENTS) {
      close(res);
      --client_number;
    } else {
      auto *conn = new Connection(); //NOLINT
      conn->fd = res;
      submit_recv(ring, conn);
    }
    submit_accept(ring, req, listenfd);
  }
  // res < 0: listening socket is gone, stop re-arming and let the loop end.
}

static void
handle_read(io_uring *ring, Connection *conn, int res)
{
  if (res <= 0) {
    close_connection(conn);
    return;
  }

  conn->request.append(conn->readbuf.data(), static_cast<size_t>(res));

  if (static_cast<size_t>(res) == conn->readbuf.size()) {
    // socket may still have more buffered data, keep draining
    submit_recv(ring, conn);
    return;
  }

  conn->response = HttpServer::handler(conn->request.c_str(), conn->request.size());
  conn->request.clear();
  conn->sent = 0;
  submit_send(ring, conn);
}

static void
handle_write(io_uring *ring, Connection *conn, int res)
{
  if (res < 0) {
    close_connection(conn);
    return;
  }

  conn->sent += static_cast<size_t>(res);
  if (conn->sent < conn->response.size()) {
    submit_send(ring, conn);
    return;
  }

  conn->response.clear();
  submit_recv(ring, conn); // keep-alive: wait for the next request
}

//-----------------------------------------------------------------------------
// WORKING
//-----------------------------------------------------------------------------

static void
working(int listenfd)
{
  io_uring ring;
  if (io_uring_queue_init(QUEUE_DEPTH, &ring, 0) < 0) {
    std::cerr << "io_uring_queue_init err\n";
    return;
  }

  IoRequest accept_req{OpType::Accept, nullptr};
  submit_accept(&ring, &accept_req, listenfd);

  io_uring_cqe *cqe;
  while (io_uring_wait_cqe(&ring, &cqe) == 0) {
    auto *req = static_cast<IoRequest *>(io_uring_cqe_get_data(cqe));
    int res = cqe->res;

    switch (req->type) {
      case OpType::Accept:
        handle_accept(&ring, req, listenfd, res);
        break;
      case OpType::Read:
        handle_read(&ring, req->conn, res);
        delete req; //NOLINT
        break;
      case OpType::Write:
        handle_write(&ring, req->conn, res);
        delete req; //NOLINT
        break;
    }

    io_uring_cqe_seen(&ring, cqe);

    if (req->type == OpType::Accept && res < 0) {
      break;
    }
  }

  io_uring_queue_exit(&ring);
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
  std::cout << "start arken.net.HttpServer (io_uring) " << m_address <<
    ":" << m_port << " (" << m_threads << ") threads..." << std::endl;

  // avoid process death when writing to a socket the peer already closed
  signal(SIGPIPE, SIG_IGN);

  start_server(m_address, m_port, m_threads);
}
