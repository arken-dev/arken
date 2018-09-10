// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/mvm>

using charon::mvm;

static int
charon_mvm_gc(lua_State *L) {
  lua_pushinteger(L, mvm::gc());
  return 1;
}

static int
charon_mvm_reload(lua_State *L) {
  mvm::reload();
  lua_pushinteger(L, mvm::version());
  return 0;
}

static int
charon_mvm_clear(lua_State *L) {
  lua_pushnumber(L, mvm::clear());
  return 1;
}

static int
charon_mvm_version(lua_State *L) {
  lua_pushnumber(L, mvm::version());
  return 1;
}

static int
charon_mvm_set(lua_State *L) {
  const char *key = luaL_checkstring(L, 1);
  int value = luaL_checkinteger(L, 2);
  mvm::set(key, value);
  return 0;
}

static int
charon_mvm_at(lua_State *L) {
  const char *key = luaL_checkstring(L, 1);
  lua_pushnumber(L, mvm::at(key));
  return 1;
}


static int
charon_mvm_pool(lua_State *L) {
  lua_pushnumber(L, mvm::pool());
  return 1;
}

extern "C" {
  int luaopen_charon_mvm( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"gc",      charon_mvm_gc},
      {"version", charon_mvm_version},
      {"reload",  charon_mvm_reload},
      {"clear",   charon_mvm_clear},
      {"pool",    charon_mvm_pool},
      {"set",     charon_mvm_set},
      {"at",      charon_mvm_at},
      {NULL, NULL}
    };
    luaL_newmetatable(L, "mvm");
    luaL_register(L, NULL, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
