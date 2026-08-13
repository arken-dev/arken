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

#include <atomic>
#include <vector>
#include <thread>

#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>

#include <arken/net/httpserver.h>
#include <arken/mvm.h>
#include <arken/base>

using HttpServer = arken::net::HttpServer;

/* client number limitation */
#define MAX_CLIENTS 1000

/* message length limitation */
#define MAX_MESSAGE_LEN (4096)

/* message length limitation */
#define MAX_LINE 4096

/* record the number of clients - acessado de todas as worker threads
 * (accept_cb incrementa, closeConn decrementa, cada thread com seu
 * próprio event_base), por isso atomic em vez de int puro */
static std::atomic<int> client_number{0};

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
// por writeSome() via EV_WRITE quando o socket fica pronto pra escrita;
// nunca escrevemos sincronamente dentro do callback que gerou a resposta,
// senão um cliente lento trava a thread inteira (todas as outras conexões
// do mesmo event_base) esperando o kernel abrir espaço no buffer de envio.
struct ConnState {
  struct event * ev = nullptr;
  event_base *   base = nullptr;
  std::string    output;
  size_t         offset = 0;
  bool           writeEnabled = false;
};

static void
ioCallback(int fd, short events, void *arg);

static void
closeConn(ConnState *state, int fd)
{
  --client_number;
  event_free(state->ev);
  close(fd);
  delete state;
}

static void
enableWrite(ConnState *state, int fd)
{
  if( state->writeEnabled ) {
    return;
  }
  event_del(state->ev);
  event_assign(state->ev, state->base, fd, EV_READ | EV_WRITE | EV_PERSIST, ioCallback, state);
  event_add(state->ev, nullptr);
  state->writeEnabled = true;
}

static void
disableWrite(ConnState *state, int fd)
{
  if( ! state->writeEnabled ) {
    return;
  }
  event_del(state->ev);
  event_assign(state->ev, state->base, fd, EV_READ | EV_PERSIST, ioCallback, state);
  event_add(state->ev, nullptr);
  state->writeEnabled = false;
}

// drena o output até esvaziar ou até o socket não aceitar mais (EAGAIN) -
// nunca bloqueia. Devolve false se a conexão foi fechada (erro real de
// escrita); quem chamou não deve tocar em `state`/`fd` de novo nesse caso.
static bool
writeSome(ConnState *state, int fd)
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
      return true; // socket ainda não tá pronto - espera o próximo EV_WRITE
    }

    closeConn(state, fd);
    return false;
  }

  state->output.clear();
  state->offset = 0;
  disableWrite(state, fd);
  return true;
}

//-----------------------------------------------------------------------------
// READ CALLBACK
//-----------------------------------------------------------------------------

static void
ioCallback(int fd, short events, void *arg)
{
  auto *state = static_cast<ConnState *>(arg);

  if( (events & EV_WRITE) && ! writeSome(state, fd) ) {
    return;
  }
  if( ! (events & EV_READ) ) {
    return;
  }

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
    enableWrite(state, fd);
  } else if ((ret < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  } else {
    closeConn(state, fd);
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
      close(sockfd);
      --client_number;
    } else {
      // conexão aceita entra bloqueante por padrão - writeSome() depende
      // de O_NONBLOCK pra nunca travar a thread (todas as outras conexões
      // do mesmo event_base) mesmo se o cliente for lento pra consumir
      int flags = fcntl(sockfd, F_GETFL, 0);
      if( flags >= 0 ) {
        fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
      }

      auto self  = static_cast<struct event *>(arg); // listener_event, ver working()
      auto base  = event_get_base(self);
      auto state = new ConnState();
      state->base = base;
      state->ev   = event_new(nullptr, -1, 0, nullptr, nullptr);
      event_assign(state->ev, base, sockfd, EV_READ | EV_PERSIST, ioCallback, state);
      event_add(state->ev, nullptr);
    }
  } else if ((sockfd < 0) && (errno == EAGAIN || errno == EWOULDBLOCK)) {
    return;
  } else {
    // `fd` aqui é o socket de escuta compartilhado por todas as worker
    // threads - um erro real (ex: EMFILE/ENFILE sob alta concorrência) é
    // local a esta thread; fechar `fd` derrubaria o accept pra todas as
    // outras. Só para de escutar por accept nesta thread (event_del não
    // mexe no fd nem afeta as conexões já abertas neste mesmo event_base)
    fprintf(stderr, "arken.net.HttpServer (libevent): accept error (errno=%d), parando accept nesta thread\n", errno);
    event_del(static_cast<struct event *>(arg));
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

  // event_new/event_assign em dois passos (em vez de um event_new só) pra
  // poder passar o próprio listener_event como arg do accept_cb - assim
  // o accept_cb consegue se auto-desregistrar (event_del) em caso de erro,
  // sem precisar de uma variável global pra isso
  listener_event = event_new(nullptr, -1, 0, nullptr, nullptr);
  event_assign(listener_event, base, fd, EV_READ | EV_PERSIST, accept_cb, listener_event);
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
/*
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
*/

//-----------------------------------------------------------------------------
// HTTP SERVER RUN
//-----------------------------------------------------------------------------

void HttpServer::run()
{
  std::cout << "start arken.net.HttpServer (libevent) " << m_address <<
     ":" << m_port << " (" << m_threads << ") threads..." << std::endl;

  // avoid process death when writing to a socket the peer already closed
  signal(SIGPIPE, SIG_IGN);

  //signal(SIGTERM, signal_handler);
  //signal(SIGINT,  signal_handler);

  start_server(m_address, m_port, m_threads);
}
