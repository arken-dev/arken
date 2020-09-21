// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/utf8>

using arken::utf8;

static int arken_utf8_lower( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  lua_pushstring( L, utf8::lower(string) );
  return 1;
}

static int arken_utf8_upper( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  lua_pushstring( L, utf8::upper(string) );
  return 1;
}

static int arken_utf8_format( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  lua_pushstring( L, utf8::format(string) );
  return 1;
}

static int arken_utf8_decode( lua_State *L ) {
  const char * string  = luaL_checkstring(L, 1);
  const char * charset = luaL_checkstring(L, 2);
  lua_pushstring( L, utf8::decode(string, charset) );
  return 1;
}

static int arken_utf8_encode( lua_State *L ) {
  const char * string  = luaL_checkstring(L, 1);
  const char * charset = luaL_checkstring(L, 2);
  lua_pushstring( L, utf8::encode(string, charset) );
  return 1;
}

static int arken_utf8_len( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  lua_pushinteger( L, utf8::len(string) );
  return 1;
}

static void
register_arken_utf8( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"lower",  arken_utf8_lower},
    {"upper",  arken_utf8_upper},
    {"format", arken_utf8_format},
    {"len",    arken_utf8_len},
    {"decode", arken_utf8_decode},
    {"encode", arken_utf8_encode},
    {NULL, NULL}
  };
  luaL_newmetatable(L, "utf8");
  luaL_register(L, NULL, Map);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int luaopen_arken_utf8( lua_State *L ) {
    register_arken_utf8( L );
    return 1;
  }
}
