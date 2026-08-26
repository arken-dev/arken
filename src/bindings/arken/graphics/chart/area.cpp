// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "chart_common.h"
#include <arken/graphics/chart/area.h>

using arken::graphics::chart::Area;

static Area *
checkArea(lua_State *L, int position = 1) {
  return *static_cast<Area **>(luaL_checkudata(L, position, "arken.graphics.chart.Area.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_graphics_chart_Area_new(lua_State *L) {
  int width  = lua_isnumber(L, 1) ? (int) luaL_checkinteger(L, 1) : 800;
  int height = lua_isnumber(L, 2) ? (int) luaL_checkinteger(L, 2) : -1;

  auto ptr = static_cast<Area **>(lua_newuserdata(L, sizeof(Area *)));
  *ptr = new Area(width, height);

  luaL_getmetatable(L, "arken.graphics.chart.Area.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_graphics_chart_Area[] = {
  {"new", arken_graphics_chart_Area_new},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_Area(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.Area");
  luaL_register(L, nullptr, arken_graphics_chart_Area);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_graphics_chart_Area_gc(lua_State *L) {
  delete checkArea(L);
  return 0;
}

static const luaL_reg arken_graphics_chart_Area_metatable[] = {
  {"__gc", arken_graphics_chart_Area_gc},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_Area_metatable(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.Area.metatable");
  luaL_register(L, nullptr, arken_graphics_chart_Area_metatable);
  register_arken_graphics_Chart_common(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_graphics_chart_Area(lua_State *L) {
    register_arken_graphics_chart_Area_metatable(L);
    register_arken_graphics_chart_Area(L);
    return 1;
  }
}
