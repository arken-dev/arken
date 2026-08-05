// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "chart_common.h"
#include <arken/graphics/chart/stacked_bar.h>

using arken::graphics::chart::StackedBar;

static StackedBar *
checkStackedBar(lua_State *L, int position = 1) {
  return *static_cast<StackedBar **>(luaL_checkudata(L, position, "arken.graphics.chart.StackedBar.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_graphics_chart_StackedBar_new(lua_State *L) {
  int width  = lua_isnumber(L, 1) ? (int) luaL_checkinteger(L, 1) : 800;
  int height = lua_isnumber(L, 2) ? (int) luaL_checkinteger(L, 2) : -1;

  auto ptr = static_cast<StackedBar **>(lua_newuserdata(L, sizeof(StackedBar *)));
  *ptr = new StackedBar(width, height);

  luaL_getmetatable(L, "arken.graphics.chart.StackedBar.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_graphics_chart_StackedBar[] = {
  {"new", arken_graphics_chart_StackedBar_new},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_StackedBar(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.StackedBar");
  luaL_register(L, nullptr, arken_graphics_chart_StackedBar);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_graphics_chart_StackedBar_setBarSpacing(lua_State *L) {
  checkStackedBar(L)->setBarSpacing(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_chart_StackedBar_barSpacing(lua_State *L) {
  lua_pushnumber(L, checkStackedBar(L)->barSpacing());
  return 1;
}

static int
arken_graphics_chart_StackedBar_gc(lua_State *L) {
  delete checkStackedBar(L);
  return 0;
}

static const luaL_reg arken_graphics_chart_StackedBar_metatable[] = {
  {"setBarSpacing", arken_graphics_chart_StackedBar_setBarSpacing},
  {"barSpacing",    arken_graphics_chart_StackedBar_barSpacing},
  {"__gc",          arken_graphics_chart_StackedBar_gc},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_StackedBar_metatable(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.StackedBar.metatable");
  luaL_register(L, nullptr, arken_graphics_chart_StackedBar_metatable);
  register_arken_graphics_Chart_common(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_graphics_chart_StackedBar(lua_State *L) {
    register_arken_graphics_chart_StackedBar_metatable(L);
    register_arken_graphics_chart_StackedBar(L);
    return 1;
  }
}
