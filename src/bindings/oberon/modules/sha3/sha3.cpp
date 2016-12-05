// Copyright 2016 The Oberon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <oberon/helper>
#include <oberon/modules/sha3.h>

static int lua_oberon_sha3_sha224( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, sha3::sha224(path, size), 56 );
  return 1;
}

static int lua_oberon_sha3_sha256( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, sha3::sha256(path, size), 64 );
  return 1;
}

static int lua_oberon_sha3_sha384( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, sha3::sha384(path, size), 96 );
  return 1;
}

static int lua_oberon_sha3_sha512( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, sha3::sha512(path, size), 128 );
  return 1;
}

extern "C" {
  int luaopen_oberon_sha3( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"sha224", lua_oberon_sha3_sha224},
      {"sha256", lua_oberon_sha3_sha256},
      {"sha384", lua_oberon_sha3_sha384},
      {"sha512", lua_oberon_sha3_sha512},
      {NULL, NULL}
    };
    luaL_register(L, "sha3", Map);
    return 1;
  }
}
