// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "chart_common.h"

using arken::graphics::ChartBar;

static ChartBar *
checkChartBar(lua_State *L, int position = 1) {
  return *static_cast<ChartBar **>(luaL_checkudata(L, position, "arken.graphics.ChartBar.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_graphics_ChartBar_new(lua_State *L) {
  int width  = lua_isnumber(L, 1) ? (int) luaL_checkinteger(L, 1) : 800;
  int height = lua_isnumber(L, 2) ? (int) luaL_checkinteger(L, 2) : -1;

  auto ptr = static_cast<ChartBar **>(lua_newuserdata(L, sizeof(ChartBar *)));
  *ptr = new ChartBar(width, height);

  luaL_getmetatable(L, "arken.graphics.ChartBar.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_graphics_ChartBar[] = {
  {"new", arken_graphics_ChartBar_new},
  {nullptr, nullptr}
};

void static
register_arken_graphics_ChartBar(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.ChartBar");
  luaL_register(L, nullptr, arken_graphics_ChartBar);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_graphics_ChartBar_setBarSpacing(lua_State *L) {
  checkChartBar(L)->setBarSpacing(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_ChartBar_barSpacing(lua_State *L) {
  lua_pushnumber(L, checkChartBar(L)->barSpacing());
  return 1;
}

static int
arken_graphics_ChartBar_gc(lua_State *L) {
  delete checkChartBar(L);
  return 0;
}

static const luaL_reg arken_graphics_ChartBar_metatable[] = {
  {"setBarSpacing", arken_graphics_ChartBar_setBarSpacing},
  {"barSpacing",    arken_graphics_ChartBar_barSpacing},
  {"__gc",          arken_graphics_ChartBar_gc},
  {nullptr, nullptr}
};

void static
register_arken_graphics_ChartBar_metatable(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.ChartBar.metatable");
  luaL_register(L, nullptr, arken_graphics_ChartBar_metatable);
  register_arken_graphics_Chart_common(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_graphics_ChartBar(lua_State *L) {
    register_arken_graphics_ChartBar_metatable(L);
    register_arken_graphics_ChartBar(L);
    return 1;
  }
}
