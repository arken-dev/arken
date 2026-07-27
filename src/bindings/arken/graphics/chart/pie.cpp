// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "chart_common.h"

using arken::graphics::ChartPie;

static ChartPie *
checkChartPie(lua_State *L, int position = 1) {
  return *static_cast<ChartPie **>(luaL_checkudata(L, position, "arken.graphics.ChartPie.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_graphics_ChartPie_new(lua_State *L) {
  int width  = lua_isnumber(L, 1) ? (int) luaL_checkinteger(L, 1) : 800;
  int height = lua_isnumber(L, 2) ? (int) luaL_checkinteger(L, 2) : -1;

  auto ptr = static_cast<ChartPie **>(lua_newuserdata(L, sizeof(ChartPie *)));
  *ptr = new ChartPie(width, height);

  luaL_getmetatable(L, "arken.graphics.ChartPie.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_graphics_ChartPie[] = {
  {"new", arken_graphics_ChartPie_new},
  {nullptr, nullptr}
};

void static
register_arken_graphics_ChartPie(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.ChartPie");
  luaL_register(L, nullptr, arken_graphics_ChartPie);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_graphics_ChartPie_setZeroDegree(lua_State *L) {
  checkChartPie(L)->setZeroDegree(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_ChartPie_zeroDegree(lua_State *L) {
  lua_pushnumber(L, checkChartPie(L)->zeroDegree());
  return 1;
}

static int
arken_graphics_ChartPie_gc(lua_State *L) {
  delete checkChartPie(L);
  return 0;
}

static const luaL_reg arken_graphics_ChartPie_metatable[] = {
  {"setZeroDegree", arken_graphics_ChartPie_setZeroDegree},
  {"zeroDegree",    arken_graphics_ChartPie_zeroDegree},
  {"__gc",          arken_graphics_ChartPie_gc},
  {nullptr, nullptr}
};

void static
register_arken_graphics_ChartPie_metatable(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.ChartPie.metatable");
  luaL_register(L, nullptr, arken_graphics_ChartPie_metatable);
  register_arken_graphics_Chart_common(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_graphics_ChartPie(lua_State *L) {
    register_arken_graphics_ChartPie_metatable(L);
    register_arken_graphics_ChartPie(L);
    return 1;
  }
}
