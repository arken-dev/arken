// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "chart_common.h"
#include <arken/graphics/chart/stacked_area.h>

using arken::graphics::chart::StackedArea;

static StackedArea *
checkStackedArea(lua_State *L, int position = 1) {
  return *static_cast<StackedArea **>(luaL_checkudata(L, position, "arken.graphics.chart.StackedArea.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_graphics_chart_StackedArea_new(lua_State *L) {
  int width  = lua_isnumber(L, 1) ? (int) luaL_checkinteger(L, 1) : 800;
  int height = lua_isnumber(L, 2) ? (int) luaL_checkinteger(L, 2) : -1;

  auto ptr = static_cast<StackedArea **>(lua_newuserdata(L, sizeof(StackedArea *)));
  *ptr = new StackedArea(width, height);

  luaL_getmetatable(L, "arken.graphics.chart.StackedArea.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_graphics_chart_StackedArea[] = {
  {"new", arken_graphics_chart_StackedArea_new},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_StackedArea(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.StackedArea");
  luaL_register(L, nullptr, arken_graphics_chart_StackedArea);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_graphics_chart_StackedArea_setLastSeriesGoesOnBottom(lua_State *L) {
  checkStackedArea(L)->setLastSeriesGoesOnBottom(lua_toboolean(L, 2));
  return 0;
}

static int
arken_graphics_chart_StackedArea_lastSeriesGoesOnBottom(lua_State *L) {
  lua_pushboolean(L, checkStackedArea(L)->lastSeriesGoesOnBottom());
  return 1;
}

static int
arken_graphics_chart_StackedArea_gc(lua_State *L) {
  delete checkStackedArea(L);
  return 0;
}

static const luaL_reg arken_graphics_chart_StackedArea_metatable[] = {
  {"setLastSeriesGoesOnBottom", arken_graphics_chart_StackedArea_setLastSeriesGoesOnBottom},
  {"lastSeriesGoesOnBottom",    arken_graphics_chart_StackedArea_lastSeriesGoesOnBottom},
  {"__gc",                      arken_graphics_chart_StackedArea_gc},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_StackedArea_metatable(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.StackedArea.metatable");
  luaL_register(L, nullptr, arken_graphics_chart_StackedArea_metatable);
  register_arken_graphics_Chart_common(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_graphics_chart_StackedArea(lua_State *L) {
    register_arken_graphics_chart_StackedArea_metatable(L);
    register_arken_graphics_chart_StackedArea(L);
    return 1;
  }
}
