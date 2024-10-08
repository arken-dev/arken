// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/utf8>

using arken::utf8;

static int arken_utf8_sub( lua_State *L ) {
  const char * str = luaL_checkstring(L, 1);
  int i = luaL_checkinteger(L, 2) - 1;
  int j = -1;
  if(lua_gettop(L) == 3) { // number of arguments
    j = luaL_checkinteger(L, 3);
    if( j > 0 ) {
      j--;
    }
  }
  char * result = utf8::sub(str, i, j);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int arken_utf8_mid( lua_State *L ) {
  const char * str = luaL_checkstring(L, 1);
  int i = luaL_checkinteger(L, 2) - 1;
  int j = -1;
  if(lua_gettop(L) == 3) { // number of arguments
    j = luaL_checkinteger(L, 3);
    if( j > 0 ) {
      j--;
    }
  }
  char * result = utf8::mid(str, i, j);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int arken_utf8_lower( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result = utf8::lower(string);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int arken_utf8_upper( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result = utf8::upper(string);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int arken_utf8_decode( lua_State *L ) {
  const char * str = luaL_checkstring(L, 1);
  char * result = utf8::decode(str);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int arken_utf8_encode( lua_State *L ) {
  const char * str = luaL_checkstring(L, 1);
  char * result = utf8::encode(str);
  lua_pushstring( L, result );
  return 1;
}

static int arken_utf8_len( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  lua_pushinteger( L, utf8::len(string) );
  return 1;
}

static int arken_utf8_sanitize( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  char * result = utf8::sanitize(string);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int arken_utf8_asc( lua_State *L ) {
  const char * str = luaL_checkstring(L, 1);
  char chr = 0;
  if(lua_gettop(L) == 2) { // number of arguments
    chr = luaL_checkstring(L, 2)[0];
  }
  char * res = utf8::asc(str, chr);
  lua_pushstring( L, res );
  delete[] res;
  return 1;
}

static void
register_arken_utf8( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"sub",      arken_utf8_sub},
    {"mid",      arken_utf8_mid},
    {"lower",    arken_utf8_lower},
    {"upper",    arken_utf8_upper},
    {"len",      arken_utf8_len},
    {"decode",   arken_utf8_decode},
    {"encode",   arken_utf8_encode},
    {"sanitize", arken_utf8_sanitize},
    {"asc",      arken_utf8_asc},
    {nullptr, nullptr}
  };
  luaL_newmetatable(L, "arken.utf8");
  luaL_register(L, nullptr, Map);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int luaopen_arken_utf8( lua_State *L ) {
    register_arken_utf8( L );
    return 1;
  }
}
