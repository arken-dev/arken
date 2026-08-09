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

  void protocolError(uint16_t code);
};

// Representa a conexão do ponto de vista de quem quer mandar uma mensagem
// pra ela (tipicamente exposto pro Lua). Só guarda o fd e sabe montar +
// escrever um frame de texto - não sabe nada de Lua.
class WebSocketConnection {
  public:
  explicit WebSocketConnection(int fd);
  void send(const std::string & payload);

  private:
  int m_fd;
};

// Ponte entre uma mensagem já parseada e o Lua: pega uma VM do pool, chama
// o dispatcher configurado passando (connection, payload). Igual o
// HttpServer::handler() faz pro HTTP - por isso mora fora do core (precisa
// de lua_State/mvm), não em websocket.cpp.
class WebSocketHandler {
  public:
  static void setDispatcher(std::string dispatcher);
  static void dispatch(int fd, const std::string & payload);

  private:
  static std::string dispatcher;
};

} // namespace net
} // namespace arken

#endif
