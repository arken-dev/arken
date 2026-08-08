// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "chart_common.h"
#include <arken/graphics/chart/pie.h>

using arken::graphics::chart::Pie;

static Pie *
checkPie(lua_State *L, int position = 1) {
  return *static_cast<Pie **>(luaL_checkudata(L, position, "arken.graphics.chart.Pie.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_graphics_chart_Pie_new(lua_State *L) {
  int width  = lua_isnumber(L, 1) ? (int) luaL_checkinteger(L, 1) : 800;
  int height = lua_isnumber(L, 2) ? (int) luaL_checkinteger(L, 2) : -1;

  auto ptr = static_cast<Pie **>(lua_newuserdata(L, sizeof(Pie *)));
  *ptr = new Pie(width, height);

  luaL_getmetatable(L, "arken.graphics.chart.Pie.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_graphics_chart_Pie[] = {
  {"new", arken_graphics_chart_Pie_new},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_Pie(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.Pie");
  luaL_register(L, nullptr, arken_graphics_chart_Pie);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_graphics_chart_Pie_setZeroDegree(lua_State *L) {
  checkPie(L)->setZeroDegree(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_chart_Pie_zeroDegree(lua_State *L) {
  lua_pushnumber(L, checkPie(L)->zeroDegree());
  return 1;
}

static int
arken_graphics_chart_Pie_gc(lua_State *L) {
  delete checkPie(L);
  return 0;
}

static const luaL_reg arken_graphics_chart_Pie_metatable[] = {
  {"setZeroDegree", arken_graphics_chart_Pie_setZeroDegree},
  {"zeroDegree",    arken_graphics_chart_Pie_zeroDegree},
  {"__gc",          arken_graphics_chart_Pie_gc},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_Pie_metatable(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.Pie.metatable");
  luaL_register(L, nullptr, arken_graphics_chart_Pie_metatable);
  register_arken_graphics_Chart_common(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_graphics_chart_Pie(lua_State *L) {
    register_arken_graphics_chart_Pie_metatable(L);
    register_arken_graphics_chart_Pie(L);
    return 1;
  }
}
