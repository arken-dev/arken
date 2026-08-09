// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// https://datatracker.ietf.org/doc/html/rfc6455#section-5.2

#include <cstring>
#include <unistd.h>

#include <arken/net/websocket.h>
#include <arken/digest/sha1.h>
#include <arken/base64.h>

namespace arken {
namespace net {

namespace {

using sha1   = arken::digest::sha1;
using base64 = arken::base64;

/* RFC 6455 4.2.2 */
const char * WS_GUID = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

// limite de tamanho pra payload de um frame e pra soma de uma mensagem
// fragmentada (senão um cliente mal-intencionado podia declarar um
// payloadLen gigante, ou emendar muitos frames pequenos, e fazer o
// m_buffer/m_fragmentPayload crescer sem limite até estourar memória
const uint64_t MAX_MESSAGE_SIZE = 16 * 1024 * 1024; // 16MB

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

// RFC 6455 8.1: mensagens de texto tem que ser UTF-8 válido. Valida o
// payload inteiro de uma vez (não incrementalmente por frame) - mais
// simples, e o RFC só exige que a validação aconteça antes da mensagem
// chegar na aplicação, não que seja feita cedo.
bool
isValidUtf8(const std::string & data)
{
  size_t i   = 0;
  size_t len = data.size();

  while( i < len ) {
    auto byte = static_cast<uint8_t>(data[i]);

    int      extra;
    uint32_t codepoint;
    uint32_t minValue;

    if( (byte & 0x80) == 0x00 ) {
      i++;
      continue;
    } else if( (byte & 0xE0) == 0xC0 ) {
      extra = 1; codepoint = byte & 0x1F; minValue = 0x80;
    } else if( (byte & 0xF0) == 0xE0 ) {
      extra = 2; codepoint = byte & 0x0F; minValue = 0x800;
    } else if( (byte & 0xF8) == 0xF0 ) {
      extra = 3; codepoint = byte & 0x07; minValue = 0x10000;
    } else {
      return false; // byte de continuação sozinho, ou 0xF8-0xFF (nunca válido em UTF-8)
    }

    if( i + extra >= len ) {
      return false; // sequência cortada, faltam bytes de continuação
    }

    for(int j = 1; j <= extra; j++) {
      auto cont = static_cast<uint8_t>(data[i + j]);
      if( (cont & 0xC0) != 0x80 ) {
        return false; // esperava byte de continuação (10xxxxxx), não veio
      }
      codepoint = (codepoint << 6) | (cont & 0x3F);
    }

    if( codepoint < minValue ) {
      return false; // encoding "overlong" (mesmo code point codificado com mais bytes do que precisa)
    }

    if( codepoint >= 0xD800 && codepoint <= 0xDFFF ) {
      return false; // metade de surrogate pair - inválido em UTF-8 (só existe em UTF-16)
    }

    if( codepoint > 0x10FFFF ) {
      return false; // além do range válido do Unicode
    }

    i += extra + 1;
  }

  return true;
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

std::string
WebSocketParser::acceptKey(const std::string & key)
{
  std::string combined = key + WS_GUID;

  unsigned char * digest  = sha1::bytes(combined.data(), combined.size());
  char *          encoded = base64::encode(reinterpret_cast<const char *>(digest), 20);

  std::string result(encoded);

  delete[] digest;
  delete[] encoded;

  return result;
}

std::string
WebSocketParser::buildMessage(WebSocketOpcode opcode, const std::string & payload)
{
  return buildFrame(static_cast<uint8_t>(opcode), payload);
}

std::string
WebSocketParser::buildPing(const std::string & payload)
{
  return buildFrame(OPCODE_PING, payload);
}

std::string
WebSocketParser::buildClose(uint16_t code, const std::string & reason)
{
  return buildCloseFrame(code, reason);
}

void
WebSocketParser::closeWithCode(uint16_t code)
{
  m_closeCode = code;
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
      closeWithCode(1002);
      return;
    }

    if( rsv != 0 ) {
      closeWithCode(1002);
      return;
    }

    if( ! isKnownOpcode(opcode) ) {
      closeWithCode(1002);
      return;
    }

    if( isControlOpcode(opcode) && (! fin || len7 > 125) ) {
      closeWithCode(1002);
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

    // rejeita antes de esperar os bytes do payload chegarem - senão um
    // frame declarando um tamanho gigante faria a gente bufferizar tudo
    // isso só pra descobrir depois que era grande demais
    if( payloadLen > MAX_MESSAGE_SIZE ) {
      closeWithCode(1009);
      return;
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
          closeWithCode(1002);
          return;
        }
        // cada frame de continuação já passou no limite individual, mas a
        // soma de vários frames pequenos também precisa ser limitada
        if( m_fragmentPayload.size() + payload.size() > MAX_MESSAGE_SIZE ) {
          closeWithCode(1009);
          return;
        }
        m_fragmentPayload.append(payload);
        if( fin ) {
          if( m_fragmentOpcode == WebSocketOpcode::Text && ! isValidUtf8(m_fragmentPayload) ) {
            closeWithCode(1007);
            return;
          }
          m_messages.push({m_fragmentOpcode, std::move(m_fragmentPayload)});
          m_fragmented = false;
          m_fragmentPayload.clear();
        }
        break;

      case OPCODE_TEXT:
      case OPCODE_BINARY:
        if( m_fragmented ) {
          // frame de dados novo no meio de uma mensagem fragmentada ainda aberta
          closeWithCode(1002);
          return;
        }
        if( fin ) {
          if( opcode == OPCODE_TEXT && ! isValidUtf8(payload) ) {
            closeWithCode(1007);
            return;
          }
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
        m_pongReceived = true;
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

uint16_t
WebSocketParser::closeCode()
{
  return m_closeCode;
}

bool
WebSocketParser::consumePong()
{
  bool result = m_pongReceived;
  m_pongReceived = false;
  return result;
}

WebSocketConnection::WebSocketConnection(int fd, const std::string & sessionId, const std::string & path)
  : m_fd(fd), m_sessionId(sessionId), m_path(path)
{
}

const std::string &
WebSocketConnection::sessionId()
{
  return m_sessionId;
}

const std::string &
WebSocketConnection::path()
{
  return m_path;
}

void
WebSocketConnection::send(const std::string & payload, bool binary)
{
  WebSocketRegistry::send(m_sessionId, payload, binary);
}

void
WebSocketConnection::close(const std::string & reason)
{
  WebSocketRegistry::writeFrame(m_sessionId, WebSocketParser::buildClose(1000, reason));
}

std::mutex WebSocketRegistry::s_mutex;
std::unordered_map<std::string, std::shared_ptr<WebSocketRegistry::Entry>> WebSocketRegistry::s_connections;

void
WebSocketRegistry::add(const std::string & sessionId, int fd)
{
  std::lock_guard<std::mutex> lock(s_mutex);
  s_connections[sessionId] = std::make_shared<Entry>(fd);
}

void
WebSocketRegistry::remove(const std::string & sessionId)
{
  std::shared_ptr<Entry> entry;
  {
    std::lock_guard<std::mutex> lock(s_mutex);
    auto it = s_connections.find(sessionId);
    if( it == s_connections.end() ) {
      return;
    }
    entry = it->second;
    s_connections.erase(it);
  }

  // espera qualquer write em andamento terminar antes de devolver -
  // garante que ninguém mais vai escrever nesse fd depois que a gente
  // volta pra quem chamou (que aí sim pode dar close() com segurança,
  // sem risco do fd ser reaproveitado por uma conexão nova enquanto
  // ainda tem alguém escrevendo na antiga)
  std::lock_guard<std::mutex> writeLock(entry->writeMutex);
}

void
WebSocketRegistry::writeFrame(const std::string & sessionId, const std::string & frame)
{
  std::shared_ptr<Entry> entry;
  {
    std::lock_guard<std::mutex> lock(s_mutex);
    auto it = s_connections.find(sessionId);
    if( it == s_connections.end() ) {
      return;
    }
    entry = it->second; // shared_ptr local mantém o Entry vivo mesmo se remove() apagar do mapa entre aqui e o write
  }

  std::lock_guard<std::mutex> writeLock(entry->writeMutex);

  ssize_t bytes = write(entry->fd, frame.data(), frame.size());
  while( bytes < static_cast<ssize_t>(frame.size()) ) {
    if( bytes == -1 ) {
      break;
    }
    bytes += write(entry->fd, frame.data() + bytes, frame.size() - bytes);
  }
}

void
WebSocketRegistry::send(const std::string & sessionId, const std::string & payload, bool binary)
{
  WebSocketOpcode opcode = binary ? WebSocketOpcode::Binary : WebSocketOpcode::Text;
  writeFrame(sessionId, WebSocketParser::buildMessage(opcode, payload));
}

} // namespace net
} // namespace arken
