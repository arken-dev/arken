// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/helper>

using charon::sha3;

static int lua_charon_sha3_sha224( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, sha3::sha224(path, size), 56 );
  return 1;
}

static int lua_charon_sha3_sha256( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, sha3::sha256(path, size), 64 );
  return 1;
}

static int lua_charon_sha3_sha384( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, sha3::sha384(path, size), 96 );
  return 1;
}

static int lua_charon_sha3_sha512( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, sha3::sha512(path, size), 128 );
  return 1;
}

extern "C" {
  int luaopen_charon_sha3( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"sha224", lua_charon_sha3_sha224},
      {"sha256", lua_charon_sha3_sha256},
      {"sha384", lua_charon_sha3_sha384},
      {"sha512", lua_charon_sha3_sha512},
      {NULL, NULL}
    };
    luaL_newmetatable(L, "sha3");
    luaL_register(L, NULL, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
