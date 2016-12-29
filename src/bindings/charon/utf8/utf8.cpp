// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/helper>
#include <charon/utf8>

using charon::utf8;

static int lua_charon_utf8_lower( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  lua_pushstring( L, utf8::lower(string) );
  return 1;
}

static int lua_charon_utf8_upper( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  lua_pushstring( L, utf8::upper(string) );
  return 1;
}

static int lua_charon_utf8_len( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  lua_pushinteger( L, utf8::len(string) );
  return 1;
}


extern "C" {
  int luaopen_charon_utf8( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"lower", lua_charon_utf8_lower},
      {"upper", lua_charon_utf8_upper},
      {"len",   lua_charon_utf8_len},
      {NULL, NULL}
    };
    luaL_newmetatable(L, "utf8");
    luaL_register(L, NULL, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
