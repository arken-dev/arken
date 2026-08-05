// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "chart_common.h"
#include <arken/graphics/chart/bar.h>

using arken::graphics::chart::Bar;

static Bar *
checkBar(lua_State *L, int position = 1) {
  return *static_cast<Bar **>(luaL_checkudata(L, position, "arken.graphics.chart.Bar.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_graphics_chart_Bar_new(lua_State *L) {
  int width  = lua_isnumber(L, 1) ? (int) luaL_checkinteger(L, 1) : 800;
  int height = lua_isnumber(L, 2) ? (int) luaL_checkinteger(L, 2) : -1;

  auto ptr = static_cast<Bar **>(lua_newuserdata(L, sizeof(Bar *)));
  *ptr = new Bar(width, height);

  luaL_getmetatable(L, "arken.graphics.chart.Bar.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_graphics_chart_Bar[] = {
  {"new", arken_graphics_chart_Bar_new},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_Bar(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.Bar");
  luaL_register(L, nullptr, arken_graphics_chart_Bar);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_graphics_chart_Bar_setBarSpacing(lua_State *L) {
  checkBar(L)->setBarSpacing(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_chart_Bar_barSpacing(lua_State *L) {
  lua_pushnumber(L, checkBar(L)->barSpacing());
  return 1;
}

static int
arken_graphics_chart_Bar_gc(lua_State *L) {
  delete checkBar(L);
  return 0;
}

static const luaL_reg arken_graphics_chart_Bar_metatable[] = {
  {"setBarSpacing", arken_graphics_chart_Bar_setBarSpacing},
  {"barSpacing",    arken_graphics_chart_Bar_barSpacing},
  {"__gc",          arken_graphics_chart_Bar_gc},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_Bar_metatable(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.Bar.metatable");
  luaL_register(L, nullptr, arken_graphics_chart_Bar_metatable);
  register_arken_graphics_Chart_common(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_graphics_chart_Bar(lua_State *L) {
    register_arken_graphics_chart_Bar_metatable(L);
    register_arken_graphics_chart_Bar(L);
    return 1;
  }
}
