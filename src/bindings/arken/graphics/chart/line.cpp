// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "chart_common.h"

using arken::graphics::ChartLine;

static ChartLine *
checkChartLine(lua_State *L, int position = 1) {
  return *static_cast<ChartLine **>(luaL_checkudata(L, position, "arken.graphics.ChartLine.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_graphics_ChartLine_new(lua_State *L) {
  int width  = lua_isnumber(L, 1) ? (int) luaL_checkinteger(L, 1) : 800;
  int height = lua_isnumber(L, 2) ? (int) luaL_checkinteger(L, 2) : -1;

  auto ptr = static_cast<ChartLine **>(lua_newuserdata(L, sizeof(ChartLine *)));
  *ptr = new ChartLine(width, height);

  luaL_getmetatable(L, "arken.graphics.ChartLine.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_graphics_ChartLine[] = {
  {"new", arken_graphics_ChartLine_new},
  {nullptr, nullptr}
};

void static
register_arken_graphics_ChartLine(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.ChartLine");
  luaL_register(L, nullptr, arken_graphics_ChartLine);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_graphics_ChartLine_setBaselineValue(lua_State *L) {
  checkChartLine(L)->setBaselineValue(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_ChartLine_baselineValue(lua_State *L) {
  lua_pushnumber(L, checkChartLine(L)->baselineValue());
  return 1;
}

static int
arken_graphics_ChartLine_setBaselineColor(lua_State *L) {
  checkChartLine(L)->setBaselineColor(string(luaL_checkstring(L, 2)));
  return 0;
}

static int
arken_graphics_ChartLine_baselineColor(lua_State *L) {
  string value = checkChartLine(L)->baselineColor();
  lua_pushlstring(L, value.data(), value.size());
  return 1;
}

static int
arken_graphics_ChartLine_setLineWidth(lua_State *L) {
  checkChartLine(L)->setLineWidth(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_ChartLine_lineWidth(lua_State *L) {
  lua_pushnumber(L, checkChartLine(L)->lineWidth());
  return 1;
}

static int
arken_graphics_ChartLine_setDotRadius(lua_State *L) {
  checkChartLine(L)->setDotRadius(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_ChartLine_dotRadius(lua_State *L) {
  lua_pushnumber(L, checkChartLine(L)->dotRadius());
  return 1;
}

static int
arken_graphics_ChartLine_setHideDots(lua_State *L) {
  checkChartLine(L)->setHideDots(lua_toboolean(L, 2));
  return 0;
}

static int
arken_graphics_ChartLine_hideDots(lua_State *L) {
  lua_pushboolean(L, checkChartLine(L)->hideDots());
  return 1;
}

static int
arken_graphics_ChartLine_setHideLines(lua_State *L) {
  checkChartLine(L)->setHideLines(lua_toboolean(L, 2));
  return 0;
}

static int
arken_graphics_ChartLine_hideLines(lua_State *L) {
  lua_pushboolean(L, checkChartLine(L)->hideLines());
  return 1;
}

static int
arken_graphics_ChartLine_gc(lua_State *L) {
  delete checkChartLine(L);
  return 0;
}

static const luaL_reg arken_graphics_ChartLine_metatable[] = {
  {"setBaselineValue", arken_graphics_ChartLine_setBaselineValue},
  {"baselineValue",    arken_graphics_ChartLine_baselineValue},
  {"setBaselineColor", arken_graphics_ChartLine_setBaselineColor},
  {"baselineColor",    arken_graphics_ChartLine_baselineColor},
  {"setLineWidth",     arken_graphics_ChartLine_setLineWidth},
  {"lineWidth",        arken_graphics_ChartLine_lineWidth},
  {"setDotRadius",     arken_graphics_ChartLine_setDotRadius},
  {"dotRadius",        arken_graphics_ChartLine_dotRadius},
  {"setHideDots",      arken_graphics_ChartLine_setHideDots},
  {"hideDots",         arken_graphics_ChartLine_hideDots},
  {"setHideLines",     arken_graphics_ChartLine_setHideLines},
  {"hideLines",        arken_graphics_ChartLine_hideLines},
  {"__gc",             arken_graphics_ChartLine_gc},
  {nullptr, nullptr}
};

void static
register_arken_graphics_ChartLine_metatable(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.ChartLine.metatable");
  luaL_register(L, nullptr, arken_graphics_ChartLine_metatable);
  register_arken_graphics_Chart_common(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_graphics_ChartLine(lua_State *L) {
    register_arken_graphics_ChartLine_metatable(L);
    register_arken_graphics_ChartLine(L);
    return 1;
  }
}
