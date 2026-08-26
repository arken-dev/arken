// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "chart_common.h"
#include <arken/graphics/chart/side_bar.h>

using arken::graphics::chart::SideBar;

static SideBar *
checkSideBar(lua_State *L, int position = 1) {
  return *static_cast<SideBar **>(luaL_checkudata(L, position, "arken.graphics.chart.SideBar.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_graphics_chart_SideBar_new(lua_State *L) {
  int width  = lua_isnumber(L, 1) ? (int) luaL_checkinteger(L, 1) : 800;
  int height = lua_isnumber(L, 2) ? (int) luaL_checkinteger(L, 2) : -1;

  auto ptr = static_cast<SideBar **>(lua_newuserdata(L, sizeof(SideBar *)));
  *ptr = new SideBar(width, height);

  luaL_getmetatable(L, "arken.graphics.chart.SideBar.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_graphics_chart_SideBar[] = {
  {"new", arken_graphics_chart_SideBar_new},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_SideBar(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.SideBar");
  luaL_register(L, nullptr, arken_graphics_chart_SideBar);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_graphics_chart_SideBar_setBarSpacing(lua_State *L) {
  checkSideBar(L)->setBarSpacing(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_chart_SideBar_barSpacing(lua_State *L) {
  lua_pushnumber(L, checkSideBar(L)->barSpacing());
  return 1;
}

static int
arken_graphics_chart_SideBar_gc(lua_State *L) {
  delete checkSideBar(L);
  return 0;
}

static const luaL_reg arken_graphics_chart_SideBar_metatable[] = {
  {"setBarSpacing", arken_graphics_chart_SideBar_setBarSpacing},
  {"barSpacing",    arken_graphics_chart_SideBar_barSpacing},
  {"__gc",          arken_graphics_chart_SideBar_gc},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_SideBar_metatable(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.SideBar.metatable");
  luaL_register(L, nullptr, arken_graphics_chart_SideBar_metatable);
  register_arken_graphics_Chart_common(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_graphics_chart_SideBar(lua_State *L) {
    register_arken_graphics_chart_SideBar_metatable(L);
    register_arken_graphics_chart_SideBar(L);
    return 1;
  }
}
