// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "chart_common.h"

using arken::graphics::ChartArea;

static ChartArea *
checkChartArea(lua_State *L, int position = 1) {
  return *static_cast<ChartArea **>(luaL_checkudata(L, position, "arken.graphics.ChartArea.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_graphics_ChartArea_new(lua_State *L) {
  int width  = lua_isnumber(L, 1) ? (int) luaL_checkinteger(L, 1) : 800;
  int height = lua_isnumber(L, 2) ? (int) luaL_checkinteger(L, 2) : -1;

  auto ptr = static_cast<ChartArea **>(lua_newuserdata(L, sizeof(ChartArea *)));
  *ptr = new ChartArea(width, height);

  luaL_getmetatable(L, "arken.graphics.ChartArea.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_graphics_ChartArea[] = {
  {"new", arken_graphics_ChartArea_new},
  {nullptr, nullptr}
};

void static
register_arken_graphics_ChartArea(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.ChartArea");
  luaL_register(L, nullptr, arken_graphics_ChartArea);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_graphics_ChartArea_gc(lua_State *L) {
  delete checkChartArea(L);
  return 0;
}

static const luaL_reg arken_graphics_ChartArea_metatable[] = {
  {"__gc", arken_graphics_ChartArea_gc},
  {nullptr, nullptr}
};

void static
register_arken_graphics_ChartArea_metatable(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.ChartArea.metatable");
  luaL_register(L, nullptr, arken_graphics_ChartArea_metatable);
  register_arken_graphics_Chart_common(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_graphics_ChartArea(lua_State *L) {
    register_arken_graphics_ChartArea_metatable(L);
    register_arken_graphics_ChartArea(L);
    return 1;
  }
}
