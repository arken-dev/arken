// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/core.h>
#include <arken/json.h>

using core = arken::concurrent::core;

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_concurrent_core_max(lua_State *L) {
  if(lua_gettop(L) == 1) { // number of arguments
    int max = luaL_checkinteger(L, 1);
    core::max() = max;
    return 0;
  } else {
    lua_pushinteger(L, core::max());
    return 1;
  }
}

static int
arken_concurrent_core_workers(lua_State *L) {
  lua_pushnumber(L, core::workers().size());
  return 1;
}

static int
arken_concurrent_core_actives(lua_State *L) {
  lua_pushnumber(L, core::actives());
  return 1;
}

static const luaL_reg arken_concurrent_core[] = {
  {"max",     arken_concurrent_core_max},
  {"workers", arken_concurrent_core_workers},
  {"actives", arken_concurrent_core_actives},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_core( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.core");
  luaL_register(L, nullptr, arken_concurrent_core);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_core( lua_State *L ) {
    register_arken_concurrent_core(L);
    return 1;
  }
}
