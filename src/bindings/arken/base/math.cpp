// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <cmath>

static int charon_math_round( lua_State *L ) {
  double number    = luaL_checknumber(L, 1);
  int    precision = luaL_checkint(L, 2);
  double result    = math::round(number, precision);
  lua_pushnumber(L, result);
  return 1;
}

static int charon_math_truncate( lua_State *L ) {
  double number    = luaL_checknumber(L, 1);
  int    precision = luaL_checkint(L, 2);
  double result    = math::truncate(number, precision);
  lua_pushnumber(L, result);
  return 1;
}

static int charon_math_format( lua_State *L ) {
  double number = luaL_checknumber(L, 1);
  int params    = lua_gettop(L);
  int decimal;
  const char * dpoint;
  const char * spoint;
  char * result = 0;

  if( params == 1 ) {
    result = math::format(number);
  }

  if( params == 2 ) {
    decimal = luaL_checkint(L, 2);
    result = math::format(number, decimal);
  }

  if( params == 3 ) {
    decimal = luaL_checkint(L, 2);
    dpoint  = luaL_checkstring(L, 3);
    result  = math::format(number, decimal, dpoint[0]);
  }

  if( params == 4 ) {
    decimal = luaL_checkint(L, 2);
    dpoint  = luaL_checkstring(L, 3);
    spoint  = luaL_checkstring(L, 4);
    result  = math::format(number, decimal, dpoint[0], spoint[0]);
  }

  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

int luaopen_charon_math( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"round",    charon_math_round},
    {"truncate", charon_math_truncate},
    {"format",   charon_math_format},
    {NULL, NULL}
  };
  luaL_register(L, "math", Map);
  return 1;
}
