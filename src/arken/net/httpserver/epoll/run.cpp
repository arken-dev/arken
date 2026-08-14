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
#include <atomic>
#include <functional>
#include <memory>
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

/* record the number of clients - acessado de todas as worker threads
 * (accept_cb incrementa, read_cb decrementa, cada thread com seu próprio
 * EventLoop/epoll), por isso atomic em vez de int puro */
static std::atomic<int> client_number{0};

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

  // troca a máscara de eventos de um fd já registrado (ex: ligar EPOLLOUT
  // quando sobra dado no buffer de saída, desligar quando esvazia)
  void modify(int fd, uint32_t events)
  {
    epoll_event ev{};
    ev.events  = events;
    ev.data.fd = fd;
    epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &ev);
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
// CONNECTION STATE
//-----------------------------------------------------------------------------

// buffer de saída por conexão - toda resposta HTTP acaba aqui e é drenada
// por writeSome() via EPOLLOUT quando o socket fica pronto pra escrita;
// nunca escrevemos sincronamente dentro do callback que gerou a resposta,
// senão um cliente lento trava a thread inteira (todas as outras conexões
// do mesmo epoll) esperando o kernel abrir espaço no buffer de envio.
struct ConnState {
  std::string output;
  size_t      offset = 0;
};

static void
closeConn(EventLoop *loop, int fd)
{
  --client_number;
  loop->remove(fd);
  close(fd);
}

// drena o output até esvaziar ou até o socket não aceitar mais (EAGAIN) -
// nunca bloqueia. Devolve false se a conexão foi fechada (erro real de
// escrita); quem chamou não deve tocar em `fd`/`state` de novo nesse caso.
static bool
writeSome(EventLoop *loop, int fd, const std::shared_ptr<ConnState> & state)
{
  while( state->offset < state->output.size() ) {
    const char * data      = state->output.data() + state->offset;
    size_t       remaining = state->output.size() - state->offset;

    ssize_t sent = send(fd, data, remaining, MSG_DONTWAIT | MSG_NOSIGNAL);

    if( sent > 0 ) {
      state->offset += static_cast<size_t>(sent);
      continue;
    }

    if( sent < 0 && (errno == EAGAIN || errno == EWOULDBLOCK) ) {
      return true; // socket ainda não tá pronto - espera o próximo EPOLLOUT
    }

    closeConn(loop, fd);
    return false;
  }

  state->output.clear();
  state->offset = 0;
  loop->modify(fd, EPOLLIN); // já mandou tudo - volta a só ouvir leitura
  return true;
}

//-----------------------------------------------------------------------------
// READ CALLBACK
//-----------------------------------------------------------------------------

static void
read_cb(EventLoop *loop, int fd, uint32_t events, const std::shared_ptr<ConnState> & state)
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
    state->output = HttpServer::handler(tmp.c_str(), tmp.size());
    state->offset = 0;
    loop->modify(fd, EPOLLIN | EPOLLOUT);
  } else if ((ret < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  } else {
    closeConn(loop, fd);
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
    // conexão aceita entra bloqueante por padrão - writeSome() depende de
    // O_NONBLOCK pra nunca travar a thread (todas as outras conexões do
    // mesmo epoll) mesmo se o cliente for lento pra consumir a resposta
    int flags = fcntl(connfd, F_GETFL, 0);
    if( flags >= 0 ) {
      fcntl(connfd, F_SETFL, flags | O_NONBLOCK);
    }

    if (++client_number > MAX_CLIENTS) {
      close(connfd);
      --client_number;
    } else {
      auto state = std::make_shared<ConnState>();
      loop->add(connfd, EPOLLIN, [loop, state](int cfd, uint32_t ev) {
        if( (ev & EPOLLOUT) && ! writeSome(loop, cfd, state) ) {
          return;
        }
        if( ev & EPOLLIN ) {
          read_cb(loop, cfd, ev, state);
        }
      });
    }
  } else if ((connfd < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  } else {
    // `fd` aqui é o socket de escuta compartilhado por todas as worker
    // threads - um erro real (ex: EMFILE/ENFILE sob alta concorrência) é
    // local a esta thread; fechar `fd` derrubaria o accept pra todas as
    // outras, e loop->stop() pararia até as conexões já abertas nesta
    // mesma thread (o EventLoop atende accept E leitura/escrita juntos).
    // Só para de escutar por accept nesta thread - o resto continua.
    fprintf(stderr, "arken.net.HttpServer (epoll): accept error (errno=%d), parando accept nesta thread\n", errno);
    loop->remove(fd);
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
