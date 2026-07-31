// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Linux io_uring backend (via liburing).
//
// Accepting is done on a single dedicated thread using a plain blocking
// accept() syscall, not an io_uring SQE. The kernel charges a thread
// blocked inside io_uring_wait_cqe() as iowait whenever it has an
// unfinished SQE outstanding (an armed accept, recv, or even a read on
// an otherwise-idle eventfd all count), but a thread blocked in a plain
// accept()/epoll_wait() syscall is not. With the previous design, every
// worker thread kept its own accept SQE armed on the shared listening
// socket, so idle worker_count threads alone were enough to show high
// %wa in top/vmstat with zero clients connected. Funneling accept()
// through one ordinary blocking thread removes that.
//
// Each worker thread owns an io_uring ring used only for recv/send on
// the connections handed to it by the acceptor. A worker with no live
// connections parks on a condition_variable instead of leaving an idle
// SQE armed in its ring, for the same iowait-accounting reason. Once it
// owns at least one connection, waiting on its completions is genuine
// I/O wait (it really is waiting on client data), so no special care is
// needed there.
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
#include <sys/eventfd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#include <array>
#include <atomic>
#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
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
static std::atomic<int> client_number{0};

/* record fd for close with SIGTERM */
static int fd;

//-----------------------------------------------------------------------------
// CONNECTION / REQUEST STATE
//-----------------------------------------------------------------------------

enum class OpType { Notify, Read, Write };

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
  Connection *conn = nullptr; // null for the Notify request
};

// One io_uring ring plus the handoff queue the acceptor thread uses to
// give it new connections.
struct Worker
{
  io_uring ring{};
  int notify_fd = -1;
  uint64_t notify_buf = 0;

  std::mutex mtx;
  std::condition_variable cv;
  std::deque<int> pending_fds;
  int live = 0;
  bool stop = false;
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
submit_notify(Worker *w)
{
  auto *req = new IoRequest{OpType::Notify, nullptr}; //NOLINT
  io_uring_sqe *sqe = io_uring_get_sqe(&w->ring);
  io_uring_prep_read(sqe, w->notify_fd, &w->notify_buf, sizeof(w->notify_buf), 0);
  io_uring_sqe_set_data(sqe, req);
  io_uring_submit(&w->ring);
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
close_connection(Worker *w, Connection *conn)
{
  --w->live;
  --client_number;
  close(conn->fd);
  delete conn; //NOLINT
}

//-----------------------------------------------------------------------------
// COMPLETION HANDLERS
//-----------------------------------------------------------------------------

static void
handle_read(Worker *w, Connection *conn, int res)
{
  if (res <= 0) {
    close_connection(w, conn);
    return;
  }

  conn->request.append(conn->readbuf.data(), static_cast<size_t>(res));

  if (static_cast<size_t>(res) == conn->readbuf.size()) {
    // socket may still have more buffered data, keep draining
    submit_recv(&w->ring, conn);
    return;
  }

  conn->response = HttpServer::handler(conn->request.c_str(), conn->request.size());
  conn->request.clear();
  conn->sent = 0;
  submit_send(&w->ring, conn);
}

static void
handle_write(Worker *w, Connection *conn, int res)
{
  if (res < 0) {
    close_connection(w, conn);
    return;
  }

  conn->sent += static_cast<size_t>(res);
  if (conn->sent < conn->response.size()) {
    submit_send(&w->ring, conn);
    return;
  }

  conn->response.clear();
  submit_recv(&w->ring, conn); // keep-alive: wait for the next request
}

//-----------------------------------------------------------------------------
// ACCEPTOR
//-----------------------------------------------------------------------------

static void
hand_off(Worker *w, int connfd)
{
  {
    std::lock_guard<std::mutex> lock(w->mtx);
    w->pending_fds.push_back(connfd);
  }
  w->cv.notify_one();

  // Wake the worker if it is currently parked inside io_uring_wait_cqe()
  // servicing other live connections rather than on the condvar.
  uint64_t one = 1;
  ssize_t ret = write(w->notify_fd, &one, sizeof(one));
  (void)ret;
}

static void
acceptor_loop(int listenfd, std::vector<std::unique_ptr<Worker>> *workers)
{
  size_t next = 0;

  for (;;) {
    int connfd = accept(listenfd, nullptr, nullptr);
    if (connfd < 0) {
      if (errno == EINTR) {
        continue;
      }
      break; // listening socket is gone
    }

    if (++client_number > MAX_CLIENTS) {
      --client_number;
      close(connfd);
      continue;
    }

    Worker *w = (*workers)[next++ % workers->size()].get();
    hand_off(w, connfd);
  }

  for (auto &w : *workers) {
    std::lock_guard<std::mutex> lock(w->mtx);
    w->stop = true;
    w->cv.notify_one();
  }
}

//-----------------------------------------------------------------------------
// WORKER
//-----------------------------------------------------------------------------

static void
worker_loop(Worker *w)
{
  if (io_uring_queue_init(QUEUE_DEPTH, &w->ring, 0) < 0) {
    std::cerr << "io_uring_queue_init err\n";
    return;
  }

  bool notify_armed = false;

  for (;;) {
    if (w->live == 0) {
      // Nothing assigned: park on the condvar instead of leaving an
      // idle SQE armed in the ring (see file header comment).
      std::unique_lock<std::mutex> lock(w->mtx);
      w->cv.wait(lock, [&] { return !w->pending_fds.empty() || w->stop; });
      if (w->pending_fds.empty() && w->stop) {
        break;
      }
    }

    {
      std::lock_guard<std::mutex> lock(w->mtx);
      while (!w->pending_fds.empty()) {
        int connfd = w->pending_fds.front();
        w->pending_fds.pop_front();
        auto *conn = new Connection(); //NOLINT
        conn->fd = connfd;
        submit_recv(&w->ring, conn);
        ++w->live;
      }
    }

    if (!notify_armed) {
      submit_notify(w);
      notify_armed = true;
    }

    io_uring_cqe *cqe;
    if (io_uring_wait_cqe(&w->ring, &cqe) != 0) {
      break;
    }

    auto *req = static_cast<IoRequest *>(io_uring_cqe_get_data(cqe));
    int res = cqe->res;

    switch (req->type) {
      case OpType::Notify:
        notify_armed = false;
        delete req; //NOLINT
        break;
      case OpType::Read:
        handle_read(w, req->conn, res);
        delete req; //NOLINT
        break;
      case OpType::Write:
        handle_write(w, req->conn, res);
        delete req; //NOLINT
        break;
    }

    io_uring_cqe_seen(&w->ring, cqe);
  }

  io_uring_queue_exit(&w->ring);
  close(w->notify_fd);
}

//-----------------------------------------------------------------------------
// START SERVER
//-----------------------------------------------------------------------------

static void
start_server(char const *addr, uint16_t port, int threads)
{
  int fd = create_serverfd(addr, port);

  std::vector<std::unique_ptr<Worker>> workers;
  for (int i = 0; i < threads; i++) {
    auto w = std::make_unique<Worker>();
    w->notify_fd = eventfd(0, EFD_NONBLOCK);
    workers.push_back(std::move(w));
  }

  std::vector<std::thread> worker_threads;
  for (auto &w : workers) {
    worker_threads.emplace_back(worker_loop, w.get());
  }

  std::thread acceptor(acceptor_loop, fd, &workers);
  acceptor.join();

  for (std::thread &t : worker_threads) {
    t.join();
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
