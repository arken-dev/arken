// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_NET_WEBSOCKET_CLIENT_
#define _ARKEN_NET_WEBSOCKET_CLIENT_

#include <arken/base>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>

struct lws;
struct lws_context;
struct lws_protocols;
struct lua_State;

namespace arken {
namespace net {

// Cliente WebSocket (RFC 6455) sobre libwebsockets. connect() nunca
// bloqueia: dispara uma thread própria rodando o service loop do lws e
// volta na hora - igual o `new WebSocket(url)` do JavaScript, onde a
// conexão acontece em background. send()/close() podem ser chamados a
// qualquer momento depois disso, de qualquer thread.
//
// Os 4 eventos do protocolo (onOpen/onMessage/onClose/onError) são
// funções Lua atribuídas direto no objeto (client.onOpen = function(self)
// ... end, via __newindex no binding), guardadas como referência
// (luaL_ref) na VM que fez a atribuição. A thread do lws NUNCA chama Lua
// diretamente - ela só empilha o evento (pushEvent) numa fila
// thread-safe e sinaliza m_eventCv; quem efetivamente invoca os callbacks
// é loop(), chamado pelo script (`while client:loop() do ... end`) a
// partir da SUA PRÓPRIA thread. loop() bloqueia em m_eventCv até ter
// evento pra entregar (ou a conexão fechar) - dá pro script escrever um
// laço simples sem gerenciar sleep/poll na mão, e sem gastar CPU
// esperando à toa. Uma função Lua é um closure preso ao heap/GC de um
// lua_State específico (não existe em outra VM, sem lua_xmove nesse
// código), e chamar de volta na VM a partir da thread do lws colidiria
// com o que a thread do script estiver fazendo na mesma VM ao mesmo
// tempo - sem lock nenhum do lado do Lua/LuaJIT, isso corrompe o heap
// (reproduzido: connection refused dispara CLIENT_CONNECTION_ERROR em
// menos de 1ms, cedo o suficiente pra colidir com o próprio script ainda
// terminando de retornar de connect()).
class WebSocketClient
{
  private:
  string      m_host;
  string      m_path;
  int         m_port = 0;
  bool        m_ssl  = false;

  std::atomic<bool> m_connected{false};
  std::atomic<bool> m_failure{false};
  std::atomic<bool> m_stop{false};
  std::atomic<bool> m_closeRequested{false};
  std::atomic<bool> m_pendingLwsClose{false};
  std::atomic<lws *> m_wsi{nullptr};

  std::mutex  m_messageMutex;
  string      m_message;

  std::mutex  m_closeMutex;
  string      m_closeReason;

  std::mutex  m_sendMutex;
  std::queue<std::pair<string, bool>> m_sendQueue;

  string      m_recvBuffer; // só acessado pela thread do service loop

  // populados em LWS_CALLBACK_WS_PEER_INITIATED_CLOSE (o único evento em
  // que o lws entrega código/motivo do close como veio no frame - ver
  // comentário no .cpp) e consumidos no CLOSED/CLIENT_CLOSED logo depois;
  // só acessados pela thread do service loop
  int    m_peerCloseCode = 1000;
  string m_peerCloseReason;

  std::atomic<lws_context *> m_context{nullptr};
  lws_protocols             * m_protocols = nullptr;
  std::thread                 m_thread;

  lua_State  * m_ownerState  = nullptr;
  int          m_onOpenRef    = -2; // LUA_NOREF - evita puxar lua.h nesse header
  int          m_onMessageRef = -2;
  int          m_onCloseRef   = -2;
  int          m_onErrorRef   = -2;

  enum class EventKind { Open, Message, Close, Error };
  struct Event {
    EventKind kind;
    string    text;        // payload (Message) ou motivo (Close/Error)
    bool      binary = false;
    int       code    = 0; // só usado em Close
  };

  std::mutex              m_eventMutex;
  std::condition_variable m_eventCv;
  std::queue<Event>       m_eventQueue;

  void parseUrl(const string & url);
  void setFailure(const string & message);
  void run();
  void wakeService(); // lws_service() dorme até lws_cancel_service() acordar - ver .cpp

  void setCallback(lua_State * L, int & slot, int ref);

  void pushEvent(Event event);
  void dispatchOpen();
  void dispatchMessage(string payload, bool binary);
  void dispatchClose(int code, string reason);
  void dispatchError(string reason);

  bool beginCall(lua_State * L, int ref); // deixa [callback, self] no topo da pilha
  void invoke(int ref);                 // sem args extras (Open)
  void invoke(int ref, string a, bool b); // Message
  void invoke(int ref, int code, string a); // Close
  void invoke(int ref, string a);         // Error

  static int callback(lws * wsi, int reason, void * user, void * in, size_t len);

  public:
  WebSocketClient(const char * url);
  ~WebSocketClient();

  void   setOnOpen(lua_State * L, int ref);
  void   setOnMessage(lua_State * L, int ref);
  void   setOnClose(lua_State * L, int ref);
  void   setOnError(lua_State * L, int ref);

  void   connect();
  void   send(string payload, bool binary = false);
  void   close(string reason = "");

  // uma iteração: bloqueia até ter evento (ou a conexão fechar), drena e
  // chama os callbacks correspondentes. Retorna false quando a conexão
  // já fechou e não há mais nada pendente (uso: `while client:loop() do
  // ... end`) - só é seguro chamar da mesma VM/thread que atribuiu os
  // callbacks (onOpen/onMessage/onClose/onError)
  bool   loop(lua_State * L);

  bool   connected();
  bool   failure();
  string message();
};

} // namespace net
} // namespace arken

#endif
