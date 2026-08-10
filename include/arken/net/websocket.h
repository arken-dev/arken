// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_NET_WEBSOCKET_
#define _ARKEN_NET_WEBSOCKET_

#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <unordered_map>

#include <arken/net/httpenv.h>

struct lua_State;

namespace arken {
namespace net {

enum class WebSocketOpcode {
  Text   = 0x1,
  Binary = 0x2,
};

struct WebSocketMessage {
  WebSocketOpcode opcode;
  std::string     payload;
};

// Decodifica frames RFC 6455 recebidos do cliente e monta frames de
// resposta. Não sabe nada de fd/socket/event loop — só bytes entram
// (parse), mensagens de aplicação e bytes de resposta saem.
//
// ping/pong/close são tratados aqui dentro e nunca viram WebSocketMessage:
//   - ping  -> gera um pong de resposta (mesmo payload) em output()
//   - close -> gera um close de resposta em output() e marca closed()
//   - erro de protocolo (RFC 6455) -> gera close 1002 em output() e marca closed()
//
// Não implementa negociação de Sec-WebSocket-Protocol/Extensions
// (RFC 6455 §11.3.4 / §9) - decisão deliberada, não esquecida. Protocol é
// pra quando cliente e servidor não são a mesma aplicação (cliente de
// terceiro, múltiplos protocolos no mesmo servidor); Extensions
// (permessage-deflate) exigiria compressão de verdade no framing, não só
// trocar header. Se algum dia precisar: Protocol dá pra fazer com uma
// lista fixa configurada no HttpServer (server:setWebSocketProtocols),
// sem precisar mudar a ordem handshake-antes-do-Lua atual; por-controller
// exigiria resolver a rota antes do 101, que hoje só acontece depois
// (open() já dispara com a resposta enviada).
class WebSocketParser {
  public:
  static bool        isWebSocketUpgrade(HttpEnv * env);
  static std::string acceptKey(const std::string & key);
  static std::string buildMessage(WebSocketOpcode opcode, const std::string & payload);
  static std::string buildPing(const std::string & payload = "");
  static std::string buildClose(uint16_t code = 1000, const std::string & reason = "");

  void parse(const char * data, size_t len);

  bool             hasMessage();
  WebSocketMessage message();

  bool             hasOutput();
  std::string      output();

  bool closed();
  uint16_t closeCode(); // só significativo depois de closed() == true; 1000 = close normal (não é erro)

  bool consumePong(); // true se um pong chegou desde a última checagem (e reseta) - usado pro ping do servidor

  private:
  std::string m_buffer;

  std::queue<WebSocketMessage> m_messages;
  std::queue<std::string>      m_output;

  bool     m_closed    = false;
  uint16_t m_closeCode = 1000;
  bool     m_pongReceived = false;

  bool            m_fragmented = false;
  WebSocketOpcode m_fragmentOpcode;
  std::string     m_fragmentPayload;

  // fecha com o close code dado (1002 violação de protocolo, 1007 UTF-8
  // inválido, 1009 mensagem grande demais) e marca closed() - o
  // Connection encerra o fd
  void closeWithCode(uint16_t code);
};

// Representa a conexão do ponto de vista de quem quer mandar uma mensagem
// pra ela (tipicamente exposto pro Lua). Carrega fd (send), sessionId
// (chave estável pro app persistir estado, ex: em arken.cache) e path (o
// path do handshake, pra resolver roteamento) - não sabe nada de Lua.
class WebSocketConnection {
  public:
  WebSocketConnection(int fd, const std::string & sessionId, const std::string & path);
  void send(const std::string & payload, bool binary = false);

  // manda um frame de close pro cliente - encerramento "educado" (RFC
  // 6455 7.1.2), não derruba o fd na hora. A conexão termina de verdade
  // pelo caminho normal (cliente responde com close, ou a leitura na
  // thread dona percebe EOF) - fechar de fato só pode acontecer na
  // thread dona (não dá pra mexer no ev_io/timer de fora com segurança)
  void close(const std::string & reason = "");

  const std::string & sessionId();
  const std::string & path();

  private:
  int         m_fd;
  std::string m_sessionId;
  std::string m_path;
};

// Registro de conexões WebSocket vivas, indexado por sessionId - permite
// mandar mensagem pra uma conexão diferente da que está processando o
// evento atual (ex: broadcast num chat). Compartilhado entre threads
// (cada thread do event loop tem seu próprio conjunto de Connection, mas
// esse registro é único pro processo inteiro, igual arken::cache).
//
// Todo write num fd passa por aqui - inclusive o de WebSocketConnection
// (a própria conexão respondendo pra si mesma) e as respostas automáticas
// (pong/close) do libev-ws - porque duas threads podem escrever no mesmo
// fd ao mesmo tempo (a dona da conexão, respondendo algo normal, e outra
// thread mandando uma mensagem pra essa sessão via send()) e os bytes dos
// dois writes podiam se intercalar e corromper o frame.
class WebSocketRegistry {
  public:
  static void add(const std::string & sessionId, int fd);
  static void remove(const std::string & sessionId); // bloqueia até qualquer write em andamento terminar

  static void send(const std::string & sessionId, const std::string & payload, bool binary = false);
  static void writeFrame(const std::string & sessionId, const std::string & frame);

  private:
  struct Entry {
    int        fd;
    std::mutex writeMutex;
    explicit Entry(int fd) : fd(fd) {}
  };

  static std::mutex                                              s_mutex;
  static std::unordered_map<std::string, std::shared_ptr<Entry>> s_connections;
};

// Ponte entre os eventos de uma conexão WebSocket e o Lua: pega uma VM do
// pool, chama o campo (open/message/close/error) do dispatcher configurado
// passando a connection. Igual o HttpServer::handler() faz pro HTTP - por
// isso mora fora do core (precisa de lua_State/mvm), não em websocket.cpp.
//
// São 4 entradas porque cada evento acontece em momento e lugar diferentes
// do event loop (open no handshake, message a cada frame completo, close
// na desconexão, error em violação de protocolo/timeout de ping) - não dá
// pra unificar numa chamada só. Por baixo, compartilham o mesmo mecanismo
// (prepareCall).
//
// error() não é sobre exceção Lua - isso o próprio WebSocket:pexecute já
// resolve sozinho (pcall + self:rescue()), sem precisar voltar pro C++.
// Esse error() aqui é o C++ avisando de algo que ele mesmo detectou
// (violação de protocolo, UTF-8 inválido, mensagem grande demais, timeout
// de ping) - o dispatcher Lua chama object:rescue(reason) direto, sem
// passar por pexecute de novo (evita recursão se o próprio rescue falhar).
class WebSocketHandler {
  public:
  static void setDispatcher(std::string dispatcher);

  static void open(int fd, const std::string & sessionId, const std::string & path);
  static void message(int fd, const std::string & sessionId, const std::string & path,
                       const std::string & payload, bool binary);
  static void close(int fd, const std::string & sessionId, const std::string & path);
  static void error(int fd, const std::string & sessionId, const std::string & path,
                     const std::string & reason);

  private:
  static std::string dispatcher;

  // deixa a stack pronta com [dispatcherTable, method, connection] no
  // topo - quem chamou empilha os args extras (se tiver) e faz o
  // lua_pcall. Devolve nullptr (e já limpa a stack) se algo falhar antes
  // de chegar nesse ponto.
  static WebSocketConnection * prepareCall(lua_State * L, int fd, const std::string & sessionId,
                                            const std::string & path, const char * method);
};

} // namespace net
} // namespace arken

#endif
