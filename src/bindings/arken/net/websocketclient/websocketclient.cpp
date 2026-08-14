// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstring>
#include <lua/lua.hpp>
#include <arken/base>
#include <arken/net/websocketclient.h>

using arken::net::WebSocketClient;
using arken::string;

/**
 * checkWebSocketClient
 *
 * aceita tanto a metatable "dona" (arken.net.WebSocketClient.new) quanto a
 * "referência" (empurrada pelo dispatcher a cada evento) - ambas guardam o
 * mesmo layout de userdata (WebSocketClient**), só o __gc muda
 */

WebSocketClient *
checkWebSocketClient( lua_State *L ) {
  void * data = lua_touserdata(L, 1);
  if( data != nullptr && lua_getmetatable(L, 1) ) {
    luaL_getmetatable(L, "arken.net.WebSocketClient.metatable");
    if( lua_rawequal(L, -1, -2) ) {
      lua_pop(L, 2);
      return *static_cast<WebSocketClient **>(data);
    }
    lua_pop(L, 1);

    luaL_getmetatable(L, "arken.net.WebSocketClient.ref");
    if( lua_rawequal(L, -1, -2) ) {
      lua_pop(L, 2);
      return *static_cast<WebSocketClient **>(data);
    }
    lua_pop(L, 2);
  }

  luaL_typerror(L, 1, "arken.net.WebSocketClient");
  return nullptr;
}

/**
 * ClassMethods
 */

static int
arken_net_WebSocketClient_new( lua_State *L ) {
  const char * url = luaL_checkstring(L, 1);
  auto ptr = static_cast<WebSocketClient **>(lua_newuserdata(L, sizeof(WebSocketClient*)));
  *ptr = new WebSocketClient(url);
  luaL_getmetatable(L, "arken.net.WebSocketClient.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_net_WebSocketClient[] = {
  {"new", arken_net_WebSocketClient_new},
  {nullptr, nullptr}
};

void static
register_arken_net_WebSocketClient( lua_State *L ) {
  luaL_newmetatable(L, "arken.net.WebSocketClient");
  luaL_register(L, nullptr, arken_net_WebSocketClient);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

// __newindex - intercepta client.onOpen/onMessage/onClose/onError = function(self, ...) end
// (atribuição direta de campo num userdata não funciona sem __newindex -
// diferente de uma table Lua normal, userdata não tem armazenamento
// próprio). Guarda uma referência (luaL_ref) da função na VM que fez a
// atribuição - ver o comentário de WebSocketClient no header sobre por
// que isso exige uma VM de vida longa.
static int
arken_net_WebSocketClient_newindex( lua_State *L ) {
  WebSocketClient * udata = checkWebSocketClient( L );
  const char      * key   = luaL_checkstring(L, 2);
  luaL_checktype(L, 3, LUA_TFUNCTION);

  lua_pushvalue(L, 3);
  int ref = luaL_ref(L, LUA_REGISTRYINDEX);

  if( strcmp(key, "onOpen") == 0 ) {
    udata->setOnOpen(L, ref);
  } else if( strcmp(key, "onMessage") == 0 ) {
    udata->setOnMessage(L, ref);
  } else if( strcmp(key, "onClose") == 0 ) {
    udata->setOnClose(L, ref);
  } else if( strcmp(key, "onError") == 0 ) {
    udata->setOnError(L, ref);
  } else {
    luaL_unref(L, LUA_REGISTRYINDEX, ref);
    return luaL_error(L, "unknown field '%s' on arken.net.WebSocketClient", key);
  }

  return 0;
}

static int
arken_net_WebSocketClient_loop( lua_State *L ) {
  WebSocketClient * udata = checkWebSocketClient( L );
  lua_pushboolean(L, udata->loop(L));
  return 1;
}

static int
arken_net_WebSocketClient_connect( lua_State *L ) {
  WebSocketClient * udata = checkWebSocketClient( L );
  udata->connect();
  return 0;
}

static int
arken_net_WebSocketClient_send( lua_State *L ) {
  size_t len;
  WebSocketClient * udata   = checkWebSocketClient( L );
  const char      * payload = luaL_checklstring(L, 2, &len);
  bool              binary  = lua_toboolean(L, 3);
  udata->send(string(payload, len), binary);
  return 0;
}

static int
arken_net_WebSocketClient_close( lua_State *L ) {
  size_t len;
  WebSocketClient * udata  = checkWebSocketClient( L );
  const char      * reason = luaL_optlstring(L, 2, "", &len);
  udata->close(string(reason, len));
  return 0;
}

static int
arken_net_WebSocketClient_connected( lua_State *L ) {
  WebSocketClient * udata = checkWebSocketClient( L );
  lua_pushboolean(L, udata->connected());
  return 1;
}

static int
arken_net_WebSocketClient_failure( lua_State *L ) {
  WebSocketClient * udata = checkWebSocketClient( L );
  lua_pushboolean(L, udata->failure());
  return 1;
}

static int
arken_net_WebSocketClient_message( lua_State *L ) {
  WebSocketClient * udata = checkWebSocketClient( L );
  string message = udata->message();
  lua_pushlstring(L, message.data(), message.size());
  return 1;
}

static int
arken_net_WebSocketClient_gc( lua_State *L ) {
  WebSocketClient * udata = checkWebSocketClient( L );
  delete udata;
  return 0;
}

static const
luaL_reg arken_net_WebSocketClient_metatable[] = {
  {"loop",          arken_net_WebSocketClient_loop},
  {"connect",       arken_net_WebSocketClient_connect},
  {"send",          arken_net_WebSocketClient_send},
  {"close",         arken_net_WebSocketClient_close},
  {"connected",     arken_net_WebSocketClient_connected},
  {"failure",       arken_net_WebSocketClient_failure},
  {"message",       arken_net_WebSocketClient_message},
  {nullptr, nullptr}
};

void static
register_arken_net_WebSocketClient_metatable( lua_State *L ) {
  // metatable "dona": criada por arken.net.WebSocketClient.new, deleta o
  // objeto C++ no __gc
  luaL_newmetatable(L,  "arken.net.WebSocketClient.metatable");
  luaL_register(L, nullptr, arken_net_WebSocketClient_metatable);
  lua_pushcfunction(L, arken_net_WebSocketClient_gc);
  lua_setfield(L, -2, "__gc");
  lua_pushcfunction(L, arken_net_WebSocketClient_newindex);
  lua_setfield(L, -2, "__newindex");
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");

  // metatable "referência": empurrada como `self` a cada onOpen/onMessage/
  // onClose/onError (WebSocketClient::dispatch* em
  // src/arken/net/websocketclient.cpp) - mesmos métodos de instância, mas
  // sem __gc, porque o ponteiro C++ continua vivo e é dono de si mesmo
  luaL_newmetatable(L, "arken.net.WebSocketClient.ref");
  luaL_register(L, nullptr, arken_net_WebSocketClient_metatable);
  lua_pushcfunction(L, arken_net_WebSocketClient_newindex);
  lua_setfield(L, -2, "__newindex");
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_net_WebSocketClient( lua_State *L ) {
    register_arken_net_WebSocketClient_metatable(L);
    register_arken_net_WebSocketClient(L);
    return 1;
  }
}
