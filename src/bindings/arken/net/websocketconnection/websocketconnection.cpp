// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/net/websocket.h>

using arken::net::WebSocketConnection;
using arken::net::WebSocketRegistry;

/**
 * ClassMethods
 */

// WebSocketConnection.send(sessionId, payload, binary) - manda uma
// mensagem pra uma conexão diferente da atual (broadcast), identificada
// pelo sessionId dela. Não precisa de instância porque não é "a minha
// conexão", é "uma conexão qualquer que eu conheço o id".
static int
arken_net_WebSocketConnection_class_send( lua_State *L ) {
  size_t sessionLen;
  const char * sessionId = luaL_checklstring(L, 1, &sessionLen);
  size_t payloadLen;
  const char * payload = luaL_checklstring(L, 2, &payloadLen);
  bool binary = lua_toboolean(L, 3);
  WebSocketRegistry::send(std::string(sessionId, sessionLen), std::string(payload, payloadLen), binary);
  return 0;
}

static const luaL_reg arken_net_WebSocketConnection[] = {
  {"send", arken_net_WebSocketConnection_class_send},
  {nullptr, nullptr}
};

void static
register_arken_net_WebSocketConnection( lua_State *L ) {
  luaL_newmetatable(L, "arken.net.WebSocketConnection");
  luaL_register(L, nullptr, arken_net_WebSocketConnection);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * checkWebSocketConnection
 */

WebSocketConnection *
checkWebSocketConnection( lua_State *L ) {
  return *static_cast<WebSocketConnection **>(luaL_checkudata(L, 1, "arken.net.WebSocketConnection.metatable"));
}

/**
 * InstanceMethods
 */

static int
arken_net_WebSocketConnection_send( lua_State *L ) {
  WebSocketConnection * udata = checkWebSocketConnection( L );
  size_t len;
  const char * payload = luaL_checklstring(L, 2, &len);
  bool binary = lua_toboolean(L, 3);
  udata->send(std::string(payload, len), binary);
  return 0;
}

static int
arken_net_WebSocketConnection_close( lua_State *L ) {
  WebSocketConnection * udata = checkWebSocketConnection( L );
  size_t len = 0;
  const char * reason = lua_isstring(L, 2) ? luaL_checklstring(L, 2, &len) : "";
  udata->close(std::string(reason, len));
  return 0;
}

static int
arken_net_WebSocketConnection_sessionId( lua_State *L ) {
  WebSocketConnection * udata = checkWebSocketConnection( L );
  lua_pushstring(L, udata->sessionId().data());
  return 1;
}

static int
arken_net_WebSocketConnection_path( lua_State *L ) {
  WebSocketConnection * udata = checkWebSocketConnection( L );
  lua_pushstring(L, udata->path().data());
  return 1;
}

static int
arken_net_WebSocketConnection_gc( lua_State *L ) {
  WebSocketConnection *udata = checkWebSocketConnection( L );
  delete udata;
  return 0;
}

static const
luaL_reg arken_net_WebSocketConnection_metatable[] = {
  {"send",      arken_net_WebSocketConnection_send},
  {"close",     arken_net_WebSocketConnection_close},
  {"sessionId", arken_net_WebSocketConnection_sessionId},
  {"path",      arken_net_WebSocketConnection_path},
  {"__gc",      arken_net_WebSocketConnection_gc},
  {nullptr, nullptr}
};

void static
register_arken_net_WebSocketConnection_metatable( lua_State *L ) {
  luaL_newmetatable(L,  "arken.net.WebSocketConnection.metatable");
  luaL_register(L, nullptr, arken_net_WebSocketConnection_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_net_WebSocketConnection( lua_State *L ) {
    register_arken_net_WebSocketConnection_metatable(L);
    register_arken_net_WebSocketConnection(L);
    return 1;
  }
}
