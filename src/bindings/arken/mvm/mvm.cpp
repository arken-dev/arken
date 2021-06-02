// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/mvm>

using arken::mvm;

static int
arken_mvm_gc(lua_State *L) {
  lua_pushinteger(L, mvm::gc());
  return 1;
}

static int
arken_mvm_reload(lua_State *L) {
  lua_pushnumber(L, mvm::reload());
  return 1;
}

static int
arken_mvm_path(lua_State *L) {
  // TODO rename arkenPath => path
  lua_pushstring(L, mvm::path());
  return 1;
}

static int
arken_mvm_clear(lua_State *L) {
  lua_pushnumber(L, mvm::clear());
  return 1;
}

static int
arken_mvm_uptime(lua_State *L) {
  lua_pushnumber(L, mvm::uptime());
  return 1;
}

static int
arken_mvm_version(lua_State *L) {
  lua_pushnumber(L, mvm::version());
  return 1;
}

static int
arken_mvm_set(lua_State *L) {
  const char *key = luaL_checkstring(L, 1);
  int value = luaL_checkinteger(L, 2);
  mvm::set(key, value);
  return 0;
}

static int
arken_mvm_at(lua_State *L) {
  const char *key = luaL_checkstring(L, 1);
  lua_pushnumber(L, mvm::at(key));
  return 1;
}

static int
arken_mvm_pool(lua_State *L) {
  lua_pushnumber(L, mvm::pool());
  return 1;
}

static int
arken_mvm_wait(lua_State *L) {
  mvm::wait();
  return 0;
}

static int
arken_mvm_env(lua_State *L) {
  if(lua_gettop(L) == 1) { // number of arguments
    const char *env = luaL_checkstring(L, 1);
    mvm::env(env);
    return 0;
  } else {
    const char * env = mvm::env();
    lua_pushstring(L, env);
    return 1;
  }
}

static int
arken_mvm_cext(lua_State *L) {
  const char * cext = mvm::cext();
  lua_pushstring(L, cext);
  return 1;
}

static void
register_arken_mvm( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"gc",      arken_mvm_gc},
    {"version", arken_mvm_version},
    {"reload",  arken_mvm_reload},
    {"clear",   arken_mvm_clear},
    {"uptime",  arken_mvm_uptime},
    {"pool",    arken_mvm_pool},
    {"set",     arken_mvm_set},
    {"at",      arken_mvm_at},
    {"wait",    arken_mvm_wait},
    {"path",    arken_mvm_path},
    {"env",     arken_mvm_env},
    {"cext",    arken_mvm_cext},
    {NULL, NULL}
  };
  luaL_newmetatable(L, "mvm");
  luaL_register(L, NULL, Map);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int luaopen_arken_mvm( lua_State *L ) {
    register_arken_mvm(L);
    return 1;
  }
}
