// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/net/websocket.h>

using arken::net::WebSocketConnection;

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
    return 1;
  }
}
