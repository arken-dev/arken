// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>

using namespace arken;

static int arken_http_read( lua_State *L ) {
  const char *path = luaL_checkstring(L, 1);
  lua_pushstring( L, http::read(path) );
  return 1;
}

static int arken_http_get( lua_State *L ) {
  const char *path = luaL_checkstring(L, 1);
  lua_pushstring( L, http::get(path) );
  return 1;
}

static int arken_http_post( lua_State *L ) {
  const char *path = luaL_checkstring(L, 1);
  lua_pushstring( L, http::post(path) );
  return 1;
}

static int arken_http_put( lua_State *L ) {
  const char *path = luaL_checkstring(L, 1);
  lua_pushstring( L, http::put(path) );
  return 1;
}

static void register_arken_http( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"read", arken_http_read},
    {"get",  arken_http_get},
    {"post", arken_http_post},
    {"put",  arken_http_put},
    {NULL, NULL}
  };
  luaL_newmetatable(L, "http");
  luaL_register(L, NULL, Map);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int luaopen_arken_http( lua_State *L ) {
    register_arken_http(L);
    return 1;
  }
  int luaopen_arken_http( lua_State *L ) {
    register_arken_http(L);
    return 1;
  }
}
