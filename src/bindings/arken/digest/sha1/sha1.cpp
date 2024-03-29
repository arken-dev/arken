// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/digest/sha1.h>

using sha1 = arken::digest::sha1;

static int arken_digest_sha1_bytes( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  char* result = reinterpret_cast<char*>(sha1::bytes(path, size));
  lua_pushlstring( L, result, 20 );
  delete[] result;
  return 1;
}

static int arken_digest_sha1_hash( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  char * result = sha1::hash(path, size);
  lua_pushlstring( L, result, 40 );
  delete[] result;
  return 1;
}

static int arken_digest_sha1_file( lua_State *L ) {
  const char *path = luaL_checkstring(L, 1);
  char * result = sha1::file(path);
  lua_pushlstring( L, result, 40 );
  delete[] result;

  return 1;
}

static int arken_digest_sha1_assign( lua_State *L ) {
  size_t size;
  const char *path = luaL_checkstring(L, 1);
  const char *value = luaL_checklstring(L, 2, &size);
  unsigned char* result = sha1::assign(path, value, size);
  if( result == nullptr ) {
    lua_pushnil( L );
  } else {
    lua_pushlstring( L, reinterpret_cast<char*>(result), 256 );
  }
  delete[] result;
  return 1;
}

static void register_arken_digest_sha1( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"bytes",  arken_digest_sha1_bytes},
    {"hash",   arken_digest_sha1_hash},
    {"file",   arken_digest_sha1_file},
    {"assign", arken_digest_sha1_assign},
    {nullptr, nullptr}
  };
  luaL_newmetatable(L, "arken.digest.sha1");
  luaL_register(L, nullptr, Map);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int luaopen_arken_digest_sha1( lua_State *L ) {
    register_arken_digest_sha1(L);
    return 1;
  }
}
