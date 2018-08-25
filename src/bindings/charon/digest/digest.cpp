// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>

using namespace charon;

static int charon_digest_md5( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  char * result = digest::md5(path, size);
  lua_pushlstring( L, result, 32 );
  delete[] result;

  return 1;
}

static int charon_digest_sha1( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  char * result = digest::sha1(path, size);
  lua_pushlstring( L, result, 40 );
  delete[] result;
  return 1;
}

extern "C" {
  int luaopen_charon_digest( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"md5", charon_digest_md5},
      {"sha1", charon_digest_sha1},
      {NULL, NULL}
    };
    luaL_newmetatable(L, "digest");
    luaL_register(L, NULL, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
