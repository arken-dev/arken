// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>

using namespace charon;

static int lua_charon_digest_md5( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, digest::md5(path, size), 32 );
  return 1;
}

static int lua_charon_digest_sha1( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, digest::sha1(path, size), 40 );
  return 1;
}

extern "C" {
  int luaopen_charon_digest( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"md5", lua_charon_digest_md5},
      {"sha1", lua_charon_digest_sha1},
      {NULL, NULL}
    };
    luaL_newmetatable(L, "digest");
    luaL_register(L, NULL, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
