// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/helper>
#include <cmath>

static int lua_charon_math_round( lua_State *L ) {
  double number    = luaL_checknumber(L, 1);
  int    precision = luaL_checkint(L, 2);
  double result    = math::round(number, precision);
  lua_pushnumber(L, result);
  return 1;
}

static int lua_charon_math_truncate( lua_State *L ) {
  double number    = luaL_checknumber(L, 1);
  int    precision = luaL_checkint(L, 2);
  double result    = math::truncate(number, precision);
  lua_pushnumber(L, result);
  return 1;
}

int luaopen_charon_math( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"round",    lua_charon_math_round},
    {"truncate", lua_charon_math_truncate},
    {NULL, NULL}
  };
  luaL_register(L, "math", Map);
  return 1;
}
