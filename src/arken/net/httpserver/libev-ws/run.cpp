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

/* teto de bytes drenados do socket por chamada de read_cb - sem isso, uma
 * conexão que mantém dado sempre disponível (flood, upload grande) prende
 * a thread lendo só dela até o kernel secar, atrasando as outras conexões
 * do mesmo ev_loop (cada thread processa uma conexão de cada vez). Ao
 * bater o teto a gente simplesmente para e devolve o controle pro loop -
 * como EV_READ é level-triggered, se ainda sobrar dado no kernel o
 * read_cb é chamado de novo na próxima volta, só que depois de dar chance
 * pras outras conexões dessa thread rodarem também. */
#define MAX_READ_PER_CALLBACK (256 * 1024)

/* intervalo do ping que o servidor manda pra cada conexão WebSocket -
 * detecta conexão morta (sem pong de volta) e mantém viva através de
 * proxy/NAT que derrubam conexão ociosa. Quem decide o que fazer quando
 * falta resposta é o app (via WebSocket:rescue("timeout")), não o C++ -
 * o framework só avisa, nunca fecha sozinho por causa disso. */
#define PING_INTERVAL 30.0

/* record the number of clients - acessado de todas as worker threads
 * (accept_cb incrementa, closeConnection decrementa, cada thread com seu
 * próprio ev_loop), por isso atomic em vez de int puro */
static std::atomic<int> client_number{0};

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
  std::string     sessionId;   // só existe depois do handshake (type == WEBSOCKET)
  std::string     path;        // idem - path do request que fez o upgrade
  std::string     queryString; // idem - query string do request que fez o upgrade

  ev_timer pingTimer;               // idem - só existe depois do handshake
  bool     awaitingPong = false;    // true = já mandamos um ping e ainda não veio pong

  // buffer de saída local - toda escrita (resposta HTTP, handshake,
  // frames de WebSocket) acaba aqui e é drenada por write_cb via
  // EV_WRITE quando o socket fica pronto pra escrita; nunca escrevemos
  // sincronamente dentro do callback que gerou o dado, senão um cliente
  // lento trava a thread inteira (todas as outras conexões do mesmo
  // ev_loop) esperando o kernel abrir espaço no buffer de envio.
  std::string outputBuffer;
  size_t      outputOffset = 0;
  bool        writeWatcherActive = false;
  static constexpr size_t MAX_OUTPUT_BUFFER = 8 * 1024 * 1024; // 8 MiB - acima disso é slow consumer, fecha a conexão

  // só existe depois do handshake (type == WEBSOCKET). Acordado (de
  // QUALQUER thread, via WebSocketRegistry::writeFrame/send) sempre que
  // essa sessão tem bytes pendentes pra escrever - inclusive quando é a
  // própria conexão escrevendo pra si mesma (ping_cb, pong/close-echo do
  // parser), por uniformidade: um único caminho de escrita pra toda
  // mensagem de WebSocket, local ou cross-thread (broadcast). ev_async é
  // a única forma segura do libev de notificar a thread dona de um loop
  // a partir de outra thread.
  ev_async asyncWatcher;

  // casca fina - a lógica/limiar de verdade mora em HttpServer, porque
  // esse Connection aqui é específico do libev-ws; um backend futuro
  // (libevent-ws, epoll-ws) tem o seu próprio Connection, mas delega pro
  // mesmo HttpServer::headerTooLarge()
  bool isMaxHttpHeaderSize() {
    return HttpServer::headerTooLarge(input.size());
  }
};

// código de close != 1000 (violação de protocolo/UTF-8/tamanho) tem que
// virar rescue() no app - mapeia pro motivo que o dispatcher Lua recebe
static const char *
closeCodeReason(uint16_t code)
{
  switch(code) {
    case 1007: return "invalid_utf8";
    case 1009: return "too_large";
    default:   return "protocol"; // 1002 e qualquer outro código de erro
  }
}

// dispara a cada PING_INTERVAL segundos numa conexão WebSocket já aberta.
// Usa watcher->data (não o truque de "primeiro membro" do ev_io) porque
// ev_timer não é o primeiro membro do Connection - só o ev_io pode ser.
static void
ping_cb(struct ev_loop *loop, struct ev_timer *watcher, int revents)
{
  auto * connection = static_cast<Connection *>(watcher->data);

  if( connection->awaitingPong && ! connection->websocket.consumePong() ) {
    // não fecha sozinho - só avisa o app, quem decide o que fazer é o
    // controller (ex: contar quantas vezes seguidas isso aconteceu e
    // decidir remover a pessoa de uma sala depois de N)
    WebSocketHandler::error(connection->io.fd, connection->sessionId, connection->path,
                             connection->queryString, "timeout");
  }

  WebSocketRegistry::writeFrame(connection->sessionId, WebSocketParser::buildPing());
  connection->awaitingPong = true;
}

// enfileira dado no buffer de saída local - nunca escreve no fd direto.
// devolve false se estourar o teto (slow consumer) - quem chamou deve
// tratar como motivo pra fechar a conexão.
static bool
queueOutput(Connection * connection, const char * data, size_t size)
{
  if( size == 0 ) {
    return true;
  }
  if( connection->outputBuffer.size() - connection->outputOffset + size > Connection::MAX_OUTPUT_BUFFER ) {
    return false;
  }
  if( connection->outputOffset > 0 ) {
    connection->outputBuffer.erase(0, connection->outputOffset);
    connection->outputOffset = 0;
  }
  connection->outputBuffer.append(data, size);
  return true;
}

static void
enableWrite(struct ev_loop *loop, Connection *connection)
{
  if( connection->writeWatcherActive || connection->outputBuffer.empty() ) {
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

static void
closeConnection(struct ev_loop *loop, Connection * connection)
{
  if( connection->type == ConnectionType::WEBSOCKET ) {
    ev_timer_stop(loop, &connection->pingTimer);
    WebSocketHandler::close(connection->io.fd, connection->sessionId, connection->path,
                             connection->queryString);

    // drena o que sobrou pendente no registry (ex: o close frame que a
    // gente acabou de enfileirar, ou um pong/close-echo do parser) antes
    // de tirar a sessão do registro - depois do remove() abaixo ninguém
    // mais consegue achar essa sessão pra drenar
    std::string pending = WebSocketRegistry::drainPending(connection->sessionId);
    if( ! pending.empty() ) {
      queueOutput(connection, pending.data(), pending.size());
    }

    WebSocketRegistry::remove(connection->sessionId); // espera qualquer enfileiramento em andamento terminar
    ev_async_stop(loop, &connection->asyncWatcher);    // só seguro depois do remove() acima
  }

  if( connection->outputOffset < connection->outputBuffer.size() ) {
    // última tentativa, não-bloqueante e best-effort, de mandar o que
    // sobrou (resposta HTTP, handshake reject ou frame de WebSocket)
    // antes de fechar. Não é garantia de entrega - se o kernel não tiver
    // espaço agora o final se perde, mas a conexão já está de saída de
    // qualquer forma (nunca mais vamos rodar EV_WRITE pra ela)
    send(connection->io.fd, connection->outputBuffer.data() + connection->outputOffset,
         connection->outputBuffer.size() - connection->outputOffset, MSG_DONTWAIT | MSG_NOSIGNAL);
  }

  --client_number;
  ev_io_stop(loop, &connection->io);
  close(connection->io.fd);
  delete connection;
}

// drena o outputBuffer até esvaziar ou até o socket não aceitar mais
// (EAGAIN) - nunca bloqueia. Devolve false se a conexão foi fechada (erro
// real de escrita); quem chamou não deve tocar em `connection` de novo
// nesse caso.
static bool
write_cb(struct ev_loop *loop, Connection *connection)
{
  while( connection->outputOffset < connection->outputBuffer.size() ) {
    const char * data      = connection->outputBuffer.data() + connection->outputOffset;
    size_t       remaining = connection->outputBuffer.size() - connection->outputOffset;

    ssize_t sent = send(connection->io.fd, data, remaining, MSG_DONTWAIT | MSG_NOSIGNAL);

    if( sent > 0 ) {
      connection->outputOffset += static_cast<size_t>(sent);
      continue;
    }

    if( sent < 0 && (errno == EAGAIN || errno == EWOULDBLOCK) ) {
      return true; // socket ainda não tá pronto - espera o próximo EV_WRITE
    }

    // erro real (EPIPE, ECONNRESET etc.) - encerra a conexão
    closeConnection(loop, connection);
    return false;
  }

  connection->outputBuffer.clear();
  connection->outputOffset = 0;
  disableWrite(loop, connection);
  return true;
}

// disparado (nesta mesma thread dona) quando WebSocketRegistry::writeFrame/
// send enfileirou bytes novos pra esta sessão - seja de outra thread
// (broadcast) ou desta mesma (ping_cb, pong/close-echo, mensagens): tudo
// passa por aqui, por uniformidade. Só aqui é seguro mexer no
// outputBuffer/EV_WRITE desta conexão.
static void
asyncWake_cb(struct ev_loop *loop, struct ev_async *watcher, int revents)
{
  auto * connection = static_cast<Connection *>(watcher->data);

  std::string pending = WebSocketRegistry::drainPending(connection->sessionId);
  if( pending.empty() ) {
    return;
  }

  if( ! queueOutput(connection, pending.data(), pending.size()) ) {
    closeConnection(loop, connection); // estourou MAX_OUTPUT_BUFFER - slow consumer
    return;
  }

  enableWrite(loop, connection);
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
      queueOutput(connection, response.data(), response.size());

      closeConnection(loop, connection);
    }
    return;
  }

  HttpEnv * env = new HttpEnv(connection->input.data(), connection->input.size(), true);

  std::string data;
  bool isUpgrade = WebSocketParser::isWebSocketUpgrade(env);

  if( isUpgrade ) {
    std::string sessionId, path, queryString;
    sessionId   = arken::os::uuid();
    path        = env->requestPath();
    queryString = env->queryString();

    std::string handshakeReject = WebSocketHandler::handshake(connection->io.fd, sessionId, path, queryString);

    if( ! handshakeReject.empty() ) {
      delete env;
      connection->input.clear();

      queueOutput(connection, handshakeReject.data(), handshakeReject.size());

      closeConnection(loop, connection);
      return;
    }

    std::string acceptKey = WebSocketParser::acceptKey(env->field("Sec-WebSocket-Key").data());

    data.append(HttpServer::status(101));
    data.append("\r\n");
    data.append("Upgrade: websocket\r\n");
    data.append("Connection: Upgrade\r\n");
    data.append("Sec-WebSocket-Accept: ");
    data.append(acceptKey);
    data.append("\r\n\r\n");

    connection->type        = ConnectionType::WEBSOCKET;
    connection->sessionId   = sessionId;
    connection->path        = path;
    connection->queryString = queryString;

    delete env;
  } else {
    data = HttpServer::handler(env);
  }

 // delete env;
  connection->input.clear();

  queueOutput(connection, data.data(), data.size());
  enableWrite(loop, connection);

  if( isUpgrade ) {
    ev_async_init(&connection->asyncWatcher, asyncWake_cb); //NOLINT ev_async_init is macro
    connection->asyncWatcher.data = connection;
    ev_async_start(loop, &connection->asyncWatcher);

    WebSocketRegistry::add(connection->sessionId, [loop, connection]() {
      ev_async_send(loop, &connection->asyncWatcher);
    });

    ev_timer_init(&connection->pingTimer, ping_cb, PING_INTERVAL, PING_INTERVAL); //NOLINT ev_timer_init is macro
    connection->pingTimer.data = connection;
    ev_timer_start(loop, &connection->pingTimer);

    if( ! WebSocketHandler::open(connection->io.fd, connection->sessionId, connection->path,
                                  connection->queryString) ) {
      WebSocketRegistry::writeFrame(connection->sessionId, WebSocketParser::buildClose(1011, "internal error"));
      closeConnection(loop, connection);
      return;
    }
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
    if( ! WebSocketHandler::message(connection->io.fd, connection->sessionId, connection->path,
                                     connection->queryString, message.payload, binary) ) {
      WebSocketRegistry::writeFrame(connection->sessionId, WebSocketParser::buildClose(1011, "internal error"));
      closeConnection(loop, connection);
      return;
    }
  }

  if( connection->websocket.closed() ) {
    if( connection->websocket.closeCode() != 1000 ) {
      WebSocketHandler::error(connection->io.fd, connection->sessionId, connection->path,
        connection->queryString, closeCodeReason(connection->websocket.closeCode()));
    }
    closeConnection(loop, connection);
  }
}

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

  char buf[MAX_MESSAGE_LEN];
  ssize_t ret;
  size_t  totalRead = 0;

  do {
    ret = recv(connection->io.fd, buf, MAX_MESSAGE_LEN, MSG_DONTWAIT);
    if( ret > 0 ) {
      connection->input.append(buf, ret);
      totalRead += static_cast<size_t>(ret);
    }
  } while( ret == MAX_MESSAGE_LEN && totalRead < MAX_READ_PER_CALLBACK );

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
    // conexão aceita entra bloqueante por padrão - o send() não-bloqueante
    // em write_cb/closeConnection depende disso pra nunca travar a thread
    // (todas as outras conexões do mesmo ev_loop) mesmo se o cliente for
    // lento pra consumir e o buffer de envio do kernel encher
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
    fprintf(stderr, "arken.net.HttpServer (libev-ws): accept error (errno=%d), parando accept nesta thread\n", errno);
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
  std::cout << "start arken.net.HttpServer (libev-ws) " << m_address <<
    ":" << m_port << " (" << m_threads << ") threads..." << std::endl;

  // avoid process death when writing to a socket the peer already closed
  signal(SIGPIPE, SIG_IGN);

  //signal(SIGTERM, signal_handler);
  //signal(SIGINT,  signal_handler);

  start_server(m_address, m_port, m_threads);
}
