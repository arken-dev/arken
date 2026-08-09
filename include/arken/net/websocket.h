// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_NET_WEBSOCKET_
#define _ARKEN_NET_WEBSOCKET_

#include <cstdint>
#include <queue>
#include <string>

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
class WebSocketParser {
  public:
  static bool        isWebSocketUpgrade(HttpEnv * env);
  static std::string acceptKey(const std::string & key);
  static std::string buildMessage(WebSocketOpcode opcode, const std::string & payload);

  void parse(const char * data, size_t len);

  bool             hasMessage();
  WebSocketMessage message();

  bool             hasOutput();
  std::string      output();

  bool closed();

  private:
  std::string m_buffer;

  std::queue<WebSocketMessage> m_messages;
  std::queue<std::string>      m_output;

  bool m_closed = false;

  bool            m_fragmented = false;
  WebSocketOpcode m_fragmentOpcode;
  std::string     m_fragmentPayload;

  // fecha com o close code dado (1002 violação de protocolo, 1009
  // mensagem grande demais) e marca closed() - o Connection encerra o fd
  void closeWithCode(uint16_t code);
};

// Representa a conexão do ponto de vista de quem quer mandar uma mensagem
// pra ela (tipicamente exposto pro Lua). Carrega fd (send), sessionId
// (chave estável pro app persistir estado, ex: em arken.cache) e path (o
// path do handshake, pra resolver roteamento) - não sabe nada de Lua.
class WebSocketConnection {
  public:
  WebSocketConnection(int fd, const std::string & sessionId, const std::string & path);
  void send(const std::string & payload);

  const std::string & sessionId();
  const std::string & path();

  private:
  int         m_fd;
  std::string m_sessionId;
  std::string m_path;
};

// Ponte entre os eventos de uma conexão WebSocket e o Lua: pega uma VM do
// pool, chama o campo (open/message/close) do dispatcher configurado
// passando a connection. Igual o HttpServer::handler() faz pro HTTP - por
// isso mora fora do core (precisa de lua_State/mvm), não em websocket.cpp.
//
// São 3 entradas porque cada evento acontece em momento e lugar diferentes
// do event loop (open no handshake, message a cada frame completo, close
// na desconexão) - não dá pra unificar numa chamada só. Por baixo,
// compartilham o mesmo mecanismo (prepareCall).
//
// Não existe WebSocketHandler::error() - erro é tratado inteiramente do
// lado Lua (WebSocket:pexecute captura a exceção com pcall e chama
// self:error() na mesma instância, sem precisar voltar pro C++).
class WebSocketHandler {
  public:
  static void setDispatcher(std::string dispatcher);

  static void open(int fd, const std::string & sessionId, const std::string & path);
  static void message(int fd, const std::string & sessionId, const std::string & path,
                       const std::string & payload, bool binary);
  static void close(int fd, const std::string & sessionId, const std::string & path);

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
