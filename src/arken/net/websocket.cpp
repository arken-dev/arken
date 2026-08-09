// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// https://datatracker.ietf.org/doc/html/rfc6455#section-5.2

#include <cstring>

#include <arken/net/websocket.h>

namespace arken {
namespace net {

namespace {

const uint8_t OPCODE_CONTINUATION = 0x0;
const uint8_t OPCODE_TEXT         = 0x1;
const uint8_t OPCODE_BINARY       = 0x2;
const uint8_t OPCODE_CLOSE        = 0x8;
const uint8_t OPCODE_PING         = 0x9;
const uint8_t OPCODE_PONG         = 0xA;

bool
isControlOpcode(uint8_t opcode)
{
  return opcode == OPCODE_CLOSE || opcode == OPCODE_PING || opcode == OPCODE_PONG;
}

bool
isKnownOpcode(uint8_t opcode)
{
  switch(opcode) {
    case OPCODE_CONTINUATION:
    case OPCODE_TEXT:
    case OPCODE_BINARY:
    case OPCODE_CLOSE:
    case OPCODE_PING:
    case OPCODE_PONG:
      return true;
    default:
      return false;
  }
}

// monta um frame de resposta do servidor pro cliente.
// servidor NUNCA mascara (RFC 6455 5.1) - MASK sempre 0.
std::string
buildFrame(uint8_t opcode, const std::string & payload)
{
  std::string frame;
  size_t      len = payload.size();

  frame.push_back(static_cast<char>(0x80 | opcode)); // FIN=1, RSV=0

  if( len <= 125 ) {
    frame.push_back(static_cast<char>(len));
  } else if( len <= 0xFFFF ) {
    frame.push_back(static_cast<char>(126));
    frame.push_back(static_cast<char>((len >> 8) & 0xFF));
    frame.push_back(static_cast<char>(len & 0xFF));
  } else {
    frame.push_back(static_cast<char>(127));
    for(int i = 7; i >= 0; i--) {
      frame.push_back(static_cast<char>((len >> (i * 8)) & 0xFF));
    }
  }

  frame.append(payload);
  return frame;
}

std::string
buildCloseFrame(uint16_t code, const std::string & reason)
{
  std::string payload;
  payload.push_back(static_cast<char>((code >> 8) & 0xFF));
  payload.push_back(static_cast<char>(code & 0xFF));
  payload.append(reason);
  return buildFrame(OPCODE_CLOSE, payload);
}

} // namespace

bool
WebSocketParser::isWebSocketUpgrade(HttpEnv * env)
{
  return strcmp(env->field("Connection").data(), "Upgrade") == 0 &&
         strcmp(env->field("Upgrade").data(), "websocket") == 0;
}

void
WebSocketParser::protocolError(uint16_t code)
{
  m_output.push(buildCloseFrame(code, ""));
  m_closed = true;
}

void
WebSocketParser::parse(const char * data, size_t len)
{
  m_buffer.append(data, len);

  while( ! m_closed ) {
    if( m_buffer.size() < 2 ) {
      return;
    }

    auto byte0 = static_cast<uint8_t>(m_buffer[0]);
    auto byte1 = static_cast<uint8_t>(m_buffer[1]);

    bool    fin    = (byte0 & 0x80) != 0;
    uint8_t rsv    = byte0 & 0x70;
    uint8_t opcode = byte0 & 0x0F;
    bool    mask   = (byte1 & 0x80) != 0;
    uint8_t len7   = byte1 & 0x7F;

    // cliente é obrigado a mascarar (RFC 6455 5.1)
    if( ! mask ) {
      protocolError(1002);
      return;
    }

    if( rsv != 0 ) {
      protocolError(1002);
      return;
    }

    if( ! isKnownOpcode(opcode) ) {
      protocolError(1002);
      return;
    }

    if( isControlOpcode(opcode) && (! fin || len7 > 125) ) {
      protocolError(1002);
      return;
    }

    size_t   offset = 2;
    uint64_t payloadLen = len7;

    if( len7 == 126 ) {
      if( m_buffer.size() < offset + 2 ) {
        return;
      }
      payloadLen = (static_cast<uint8_t>(m_buffer[offset]) << 8) |
                    static_cast<uint8_t>(m_buffer[offset + 1]);
      offset += 2;
    } else if( len7 == 127 ) {
      if( m_buffer.size() < offset + 8 ) {
        return;
      }
      payloadLen = 0;
      for(int i = 0; i < 8; i++) {
        payloadLen = (payloadLen << 8) | static_cast<uint8_t>(m_buffer[offset + i]);
      }
      offset += 8;
    }

    if( m_buffer.size() < offset + 4 ) {
      return;
    }

    unsigned char maskKey[4];
    for(int i = 0; i < 4; i++) {
      maskKey[i] = static_cast<uint8_t>(m_buffer[offset + i]);
    }
    offset += 4;

    if( m_buffer.size() < offset + payloadLen ) {
      return; // frame incompleto, espera mais bytes
    }

    std::string payload;
    payload.resize(payloadLen);
    for(uint64_t i = 0; i < payloadLen; i++) {
      payload[i] = static_cast<char>(static_cast<uint8_t>(m_buffer[offset + i]) ^ maskKey[i % 4]);
    }

    m_buffer.erase(0, offset + payloadLen);

    switch(opcode) {
      case OPCODE_CONTINUATION:
        if( ! m_fragmented ) {
          protocolError(1002);
          return;
        }
        m_fragmentPayload.append(payload);
        if( fin ) {
          m_messages.push({m_fragmentOpcode, std::move(m_fragmentPayload)});
          m_fragmented = false;
          m_fragmentPayload.clear();
        }
        break;

      case OPCODE_TEXT:
      case OPCODE_BINARY:
        if( m_fragmented ) {
          // frame de dados novo no meio de uma mensagem fragmentada ainda aberta
          protocolError(1002);
          return;
        }
        if( fin ) {
          m_messages.push({static_cast<WebSocketOpcode>(opcode), std::move(payload)});
        } else {
          m_fragmented      = true;
          m_fragmentOpcode  = static_cast<WebSocketOpcode>(opcode);
          m_fragmentPayload = std::move(payload);
        }
        break;

      case OPCODE_CLOSE:
        m_output.push(buildCloseFrame(1000, ""));
        m_closed = true;
        return;

      case OPCODE_PING:
        m_output.push(buildFrame(OPCODE_PONG, payload));
        break;

      case OPCODE_PONG:
        break;
    }
  }
}

bool
WebSocketParser::hasMessage()
{
  return ! m_messages.empty();
}

WebSocketMessage
WebSocketParser::message()
{
  WebSocketMessage msg = std::move(m_messages.front());
  m_messages.pop();
  return msg;
}

bool
WebSocketParser::hasOutput()
{
  return ! m_output.empty();
}

std::string
WebSocketParser::output()
{
  std::string out = std::move(m_output.front());
  m_output.pop();
  return out;
}

bool
WebSocketParser::closed()
{
  return m_closed;
}

} // namespace net
} // namespace arken
