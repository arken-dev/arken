// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/helper>

using namespace charon;

static int lua_charon_digest_md4( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, digest::md4(path, size), 32 );
  return 1;
}

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

static int lua_charon_digest_sha224( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, digest::sha224(path, size), 56 );
  return 1;
}

static int lua_charon_digest_sha256( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, digest::sha256(path, size), 64 );
  return 1;
}

static int lua_charon_digest_sha384( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, digest::sha384(path, size), 96 );
  return 1;
}

static int lua_charon_digest_sha512( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushlstring( L, digest::sha512(path, size), 128 );
  return 1;
}

extern "C" {
  int luaopen_charon_digest( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"md4", lua_charon_digest_md4},
      {"md5", lua_charon_digest_md5},
      {"sha1", lua_charon_digest_sha1},
      {"sha224", lua_charon_digest_sha224},
      {"sha256", lua_charon_digest_sha256},
      {"sha384", lua_charon_digest_sha384},
      {"sha512", lua_charon_digest_sha512},
      {NULL, NULL}
    };
    luaL_newmetatable(L, "digest");
    luaL_register(L, NULL, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
