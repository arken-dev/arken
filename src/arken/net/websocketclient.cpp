// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>

#include <libwebsockets.h>
#include <lua/lua.hpp>

#include <arken/net/websocketclient.h>

namespace arken {
namespace net {

// userdata não-dono empurrado como `self` em cada dispatch (onOpen/
// onMessage/onClose/onError) - o WebSocketClient real é dono de si mesmo
// (criado por arken.net.WebSocketClient.new, deletado no __gc dessa
// metatable); essa referência usa a metatable irmã
// "arken.net.WebSocketClient.ref", com os mesmos métodos de instância mas
// __gc vazio, só pra dar ao callback Lua acesso a self:send()/self:close()
// sem risco de double free.
static void
pushWebSocketClientRef(lua_State * L, WebSocketClient * client)
{
  auto ptr = static_cast<WebSocketClient **>(lua_newuserdata(L, sizeof(WebSocketClient *)));
  *ptr = client;
  luaL_getmetatable(L, "arken.net.WebSocketClient.ref");
  lua_setmetatable(L, -2);
}

WebSocketClient::WebSocketClient(const char * url)
{
  parseUrl(url);
}

WebSocketClient::~WebSocketClient()
{
  close();
  if( m_thread.joinable() ) {
    m_thread.join();
  }

  // libera as referências na VM de origem - só é seguro se essa VM ainda
  // estiver viva (ver comentário da classe no .h); assume que sim, dado o
  // uso pretendido (script/worker de vida longa)
  if( m_ownerState != nullptr ) {
    luaL_unref(m_ownerState, LUA_REGISTRYINDEX, m_onOpenRef);
    luaL_unref(m_ownerState, LUA_REGISTRYINDEX, m_onMessageRef);
    luaL_unref(m_ownerState, LUA_REGISTRYINDEX, m_onCloseRef);
    luaL_unref(m_ownerState, LUA_REGISTRYINDEX, m_onErrorRef);
  }
}

void
WebSocketClient::parseUrl(const string & url)
{
  string rest = url;

  if( rest.startsWith("wss://") ) {
    m_ssl  = true;
    m_port = 443;
    rest   = rest.mid(6);
  } else if( rest.startsWith("ws://") ) {
    m_ssl  = false;
    m_port = 80;
    rest   = rest.mid(5);
  } else {
    setFailure(string("invalid websocket url: ").append(url));
    return;
  }

  int slash = rest.indexOf("/");
  string authority = slash >= 0 ? rest.mid(0, slash) : rest;
  m_path = slash >= 0 ? rest.mid(slash) : string("/");

  int colon = authority.indexOf(":");
  if( colon >= 0 ) {
    m_host = authority.mid(0, colon);
    m_port = atoi(authority.mid(colon + 1).data());
  } else {
    m_host = authority;
  }
}

void
WebSocketClient::setFailure(const string & message)
{
  {
    std::lock_guard<std::mutex> lock(m_messageMutex);
    m_message = message;
  }
  m_failure = true;
}

void
WebSocketClient::setCallback(lua_State * L, int & slot, int ref)
{
  m_ownerState = L;
  if( slot != LUA_NOREF ) {
    luaL_unref(L, LUA_REGISTRYINDEX, slot);
  }
  slot = ref;
}

void
WebSocketClient::setOnOpen(lua_State * L, int ref)
{
  setCallback(L, m_onOpenRef, ref);
}

void
WebSocketClient::setOnMessage(lua_State * L, int ref)
{
  setCallback(L, m_onMessageRef, ref);
}

void
WebSocketClient::setOnClose(lua_State * L, int ref)
{
  setCallback(L, m_onCloseRef, ref);
}

void
WebSocketClient::setOnError(lua_State * L, int ref)
{
  setCallback(L, m_onErrorRef, ref);
}

void
WebSocketClient::connect()
{
  if( m_thread.joinable() ) {
    return; // já conectado ou conectando
  }
  m_thread = std::thread(&WebSocketClient::run, this);
}

void
WebSocketClient::send(string payload, bool binary)
{
  std::lock_guard<std::mutex> lock(m_sendMutex);
  m_sendQueue.emplace(std::move(payload), binary);
}

void
WebSocketClient::close(string reason)
{
  {
    std::lock_guard<std::mutex> lock(m_closeMutex);
    m_closeReason = std::move(reason);
  }
  m_closeRequested = true;
}

bool
WebSocketClient::connected()
{
  return m_connected;
}

bool
WebSocketClient::failure()
{
  return m_failure;
}

string
WebSocketClient::message()
{
  std::lock_guard<std::mutex> lock(m_messageMutex);
  return m_message;
}

// chamado pela thread do lws (dentro do callback estático) - só empilha
// o evento, nunca toca em Lua diretamente (ver comentário da classe no
// .h sobre por quê)
void
WebSocketClient::pushEvent(Event event)
{
  {
    std::lock_guard<std::mutex> lock(m_eventMutex);
    m_eventQueue.push(std::move(event));
  }
  m_eventCv.notify_one();
}

// dispatchOpen/dispatchMessage/dispatchClose/dispatchError são chamados
// pela thread do lws a partir de callback() - só empacotam o evento e
// mandam pra fila; loop() (chamado pela thread do script) que de fato
// invoca o callback Lua
void
WebSocketClient::dispatchOpen()
{
  pushEvent(Event{EventKind::Open, string(), false, 0});
}

void
WebSocketClient::dispatchMessage(string payload, bool binary)
{
  pushEvent(Event{EventKind::Message, std::move(payload), binary, 0});
}

void
WebSocketClient::dispatchClose(int code, string reason)
{
  pushEvent(Event{EventKind::Close, std::move(reason), false, code});
}

void
WebSocketClient::dispatchError(string reason)
{
  pushEvent(Event{EventKind::Error, std::move(reason), false, 0});
}

// deixa a pilha de L com [callback, self] no topo, pronta pra quem
// chamou empilhar args extras e dar lua_pcall - só chamado a partir de
// loop(), ou seja, sempre na thread do script
bool
WebSocketClient::beginCall(lua_State * L, int ref)
{
  if( ref == LUA_NOREF ) {
    return false;
  }

  lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
  if( ! lua_isfunction(L, -1) ) {
    lua_pop(L, 1);
    return false;
  }

  pushWebSocketClientRef(L, this);
  return true;
}

void
WebSocketClient::invoke(int ref)
{
  lua_State * L = m_ownerState;
  if( ! beginCall(L, ref) ) { return; }

  if( lua_pcall(L, 1, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }
}

void
WebSocketClient::invoke(int ref, string a, bool b)
{
  lua_State * L = m_ownerState;
  if( ! beginCall(L, ref) ) { return; }

  lua_pushlstring(L, a.data(), a.size());
  lua_pushboolean(L, b);

  if( lua_pcall(L, 3, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }
}

void
WebSocketClient::invoke(int ref, int code, string a)
{
  lua_State * L = m_ownerState;
  if( ! beginCall(L, ref) ) { return; }

  lua_pushinteger(L, code);
  lua_pushlstring(L, a.data(), a.size());

  if( lua_pcall(L, 3, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }
}

void
WebSocketClient::invoke(int ref, string a)
{
  lua_State * L = m_ownerState;
  if( ! beginCall(L, ref) ) { return; }

  lua_pushlstring(L, a.data(), a.size());

  if( lua_pcall(L, 2, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
    lua_pop(L, 1);
  }
}

// uma iteração: bloqueia em m_eventCv até ter evento pra entregar ou a
// conexão fechar (com timeout de segurança, pro caso de loop() ser
// chamado antes de connect() e nunca ter nada pra esperar), drena e
// invoca os callbacks correspondentes - sempre na thread que chamou
// loop() (a mesma que atribuiu os callbacks); se vier de outra VM,
// ignora silenciosamente em vez de arriscar mexer no registry errado.
// Retorna false só depois de já ter entregue o evento de close/error
// final (m_stop só vira visível pra quem chama depois que o evento
// correspondente já foi empilhado - ver run()/callback())
bool
WebSocketClient::loop(lua_State * L)
{
  if( m_ownerState == nullptr || L != m_ownerState ) {
    return false;
  }

  std::queue<Event> pending;
  {
    std::unique_lock<std::mutex> lock(m_eventMutex);
    m_eventCv.wait_for(lock, std::chrono::milliseconds(200), [this]() {
      return ! m_eventQueue.empty() || m_stop.load();
    });
    std::swap(pending, m_eventQueue);
  }

  while( ! pending.empty() ) {
    Event event = std::move(pending.front());
    pending.pop();

    switch( event.kind ) {
      case EventKind::Open:
        invoke(m_onOpenRef);
        break;
      case EventKind::Message:
        invoke(m_onMessageRef, event.text, event.binary);
        break;
      case EventKind::Close:
        invoke(m_onCloseRef, event.code, event.text);
        break;
      case EventKind::Error:
        invoke(m_onErrorRef, event.text);
        break;
    }
  }

  return ! m_stop.load();
}

// Roda inteira na thread própria criada por connect() - cria o contexto
// lws, conecta e mantém o service loop até close()/erro. send()/close()
// só mexem em filas/flags protegidas por mutex/atomic; quem efetivamente
// escreve no wsi é sempre essa thread (dentro de LWS_CALLBACK_CLIENT_WRITEABLE),
// nunca quem chamou send()/close() - lws não é thread-safe pra chamadas
// diretas em cima do wsi vindas de outra thread.
void
WebSocketClient::run()
{
  if( m_failure ) { // parseUrl já detectou uma URL inválida
    dispatchError(message());
    dispatchClose(0, message());
    m_stop = true; // depois dos pushEvent - ver comentário em callback()
    return;
  }

  m_protocols = new lws_protocols[2]();
  m_protocols[0].name     = "arken-websocketclient";
  m_protocols[0].callback = reinterpret_cast<lws_callback_function *>(&WebSocketClient::callback);
  m_protocols[0].per_session_data_size = 0;
  m_protocols[0].rx_buffer_size        = 0;

  struct lws_context_creation_info info;
  memset(&info, 0, sizeof(info));
  info.port      = CONTEXT_PORT_NO_LISTEN;
  info.protocols = m_protocols;
  info.options   = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;

  m_context = lws_create_context(&info);
  if( m_context == nullptr ) {
    setFailure("failed to create lws context");
    dispatchError(message());
    dispatchClose(0, message());
    m_stop = true;
    delete[] m_protocols;
    m_protocols = nullptr;
    return;
  }

  struct lws_client_connect_info ccinfo;
  memset(&ccinfo, 0, sizeof(ccinfo));
  ccinfo.context        = m_context;
  ccinfo.address        = m_host.data();
  ccinfo.port           = m_port;
  ccinfo.path           = m_path.data();
  ccinfo.host           = m_host.data();
  ccinfo.origin         = m_host.data();
  ccinfo.protocol       = m_protocols[0].name;
  ccinfo.userdata       = this;
  ccinfo.ssl_connection = m_ssl ? LCCSCF_USE_SSL : 0;

  lws * wsi = lws_client_connect_via_info(&ccinfo);
  if( wsi == nullptr ) {
    setFailure("failed to connect");
    dispatchError(message());
    dispatchClose(0, message());
    m_stop = true;
    lws_context_destroy(m_context);
    m_context = nullptr;
    delete[] m_protocols;
    m_protocols = nullptr;
    return;
  }
  m_wsi = wsi;

  while( ! m_stop ) {
    lws_service(m_context, 50);

    if( m_closeRequested.exchange(false) ) {
      lws * current = m_wsi.load();
      if( current != nullptr ) {
        string reason;
        {
          std::lock_guard<std::mutex> lock(m_closeMutex);
          reason = m_closeReason;
        }
        lws_close_reason(current, LWS_CLOSE_STATUS_NORMAL,
                          (unsigned char *) reason.data(), reason.size());
        m_pendingLwsClose = true;
        lws_callback_on_writable(current);
      }
    }

    lws * current = m_wsi.load();
    if( current != nullptr ) {
      std::lock_guard<std::mutex> lock(m_sendMutex);
      if( ! m_sendQueue.empty() ) {
        lws_callback_on_writable(current);
      }
    }
  }

  lws_context_destroy(m_context);
  m_context = nullptr;
  delete[] m_protocols;
  m_protocols = nullptr;
}

int
WebSocketClient::callback(lws * wsi, int reason, void * user, void * in, size_t len)
{
  auto self = static_cast<WebSocketClient *>(user);

  switch( reason ) {

    case LWS_CALLBACK_CLIENT_ESTABLISHED:
      self->m_connected = true;
      self->dispatchOpen();
      break;

    case LWS_CALLBACK_CLIENT_RECEIVE:
      self->m_recvBuffer.append(static_cast<const char *>(in), len);
      if( lws_is_final_fragment(wsi) ) {
        bool binary = lws_frame_is_binary(wsi);
        self->dispatchMessage(self->m_recvBuffer, binary);
        self->m_recvBuffer.clear();
      }
      break;

    case LWS_CALLBACK_CLIENT_WRITEABLE: {
      if( self->m_pendingLwsClose ) {
        return -1;
      }

      std::pair<string, bool> item;
      bool has = false;
      {
        std::lock_guard<std::mutex> lock(self->m_sendMutex);
        if( ! self->m_sendQueue.empty() ) {
          item = std::move(self->m_sendQueue.front());
          self->m_sendQueue.pop();
          has = true;
        }
      }

      if( has ) {
        std::vector<unsigned char> buffer(LWS_PRE + item.first.size());
        memcpy(buffer.data() + LWS_PRE, item.first.data(), item.first.size());
        lws_write(wsi, buffer.data() + LWS_PRE, item.first.size(),
                   item.second ? LWS_WRITE_BINARY : LWS_WRITE_TEXT);

        std::lock_guard<std::mutex> lock(self->m_sendMutex);
        if( ! self->m_sendQueue.empty() ) {
          lws_callback_on_writable(wsi);
        }
      }
      break;
    }

    // frame de close chegando do peer (independente de quem iniciou -
    // tanto um close espontâneo do peer quanto o ack do peer pro nosso
    // close passam por aqui): in/len trazem 2 bytes de código (network
    // order) seguidos do motivo em UTF-8, igual RECEIVE. lws_get_close_length/
    // lws_get_close_payload() (só existem no header, sem uso real
    // documentado pra esse caso) NÃO servem pra isso - devolvem lixo aqui,
    // então o código/motivo é guardado aqui e só consumido no
    // CLOSED/CLIENT_CLOSED logo em seguida
    case LWS_CALLBACK_WS_PEER_INITIATED_CLOSE:
      if( in != nullptr && len >= 2 ) {
        auto payload = static_cast<const unsigned char *>(in);
        self->m_peerCloseCode   = (payload[0] << 8) | payload[1];
        self->m_peerCloseReason = string(reinterpret_cast<const char *>(payload) + 2, len - 2);
      }
      break;

    // no papel de cliente essa lib entrega CLIENT_CLOSED (75), não a
    // genérica CLOSED (4) usada pelo lado servidor - trata as duas por
    // segurança entre versões/roles
    case LWS_CALLBACK_CLOSED:
    case LWS_CALLBACK_CLIENT_CLOSED: {
      self->m_connected = false;
      self->m_wsi        = nullptr; // wsi está prestes a ser liberado pelo lws

      // empilha o evento ANTES de marcar m_stop: loop() só confia que já
      // viu tudo quando, depois de drenar a fila, m_stop também está
      // true - se a ordem fosse invertida, loop() poderia observar
      // m_stop e devolver false pro `while` antes do evento de close
      // final estar de fato na fila
      self->dispatchClose(self->m_peerCloseCode, self->m_peerCloseReason);
      self->m_stop = true;
      break;
    }

    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR: {
      string reason = (in != nullptr && len > 0)
        ? string(static_cast<const char *>(in), len)
        : string("connection error");

      self->setFailure(reason);
      self->m_connected = false;
      self->m_wsi        = nullptr; // wsi está prestes a ser liberado pelo lws

      // JS WebSocket dispara error e depois close pro mesmo evento -
      // aqui os dois se originam da mesma LWS_CALLBACK_CLIENT_CONNECTION_ERROR.
      // m_stop só depois dos dois pushEvent, mesmo motivo do case acima
      self->dispatchError(reason);
      self->dispatchClose(0, reason);
      self->m_stop = true;
      break;
    }

    default:
      break;
  }

  return 0;
}

} // namespace net
} // namespace arken
