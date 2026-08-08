// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "chart_common.h"
#include <arken/graphics/chart/line.h>

using arken::graphics::chart::Line;

static Line *
checkLine(lua_State *L, int position = 1) {
  return *static_cast<Line **>(luaL_checkudata(L, position, "arken.graphics.chart.Line.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_graphics_chart_Line_new(lua_State *L) {
  int width  = lua_isnumber(L, 1) ? (int) luaL_checkinteger(L, 1) : 800;
  int height = lua_isnumber(L, 2) ? (int) luaL_checkinteger(L, 2) : -1;

  auto ptr = static_cast<Line **>(lua_newuserdata(L, sizeof(Line *)));
  *ptr = new Line(width, height);

  luaL_getmetatable(L, "arken.graphics.chart.Line.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_graphics_chart_Line[] = {
  {"new", arken_graphics_chart_Line_new},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_Line(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.Line");
  luaL_register(L, nullptr, arken_graphics_chart_Line);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_graphics_chart_Line_setBaselineValue(lua_State *L) {
  checkLine(L)->setBaselineValue(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_chart_Line_baselineValue(lua_State *L) {
  lua_pushnumber(L, checkLine(L)->baselineValue());
  return 1;
}

static int
arken_graphics_chart_Line_setBaselineColor(lua_State *L) {
  checkLine(L)->setBaselineColor(string(luaL_checkstring(L, 2)));
  return 0;
}

static int
arken_graphics_chart_Line_baselineColor(lua_State *L) {
  string value = checkLine(L)->baselineColor();
  lua_pushlstring(L, value.data(), value.size());
  return 1;
}

static int
arken_graphics_chart_Line_setLineWidth(lua_State *L) {
  checkLine(L)->setLineWidth(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_chart_Line_lineWidth(lua_State *L) {
  lua_pushnumber(L, checkLine(L)->lineWidth());
  return 1;
}

static int
arken_graphics_chart_Line_setDotRadius(lua_State *L) {
  checkLine(L)->setDotRadius(luaL_checknumber(L, 2));
  return 0;
}

static int
arken_graphics_chart_Line_dotRadius(lua_State *L) {
  lua_pushnumber(L, checkLine(L)->dotRadius());
  return 1;
}

static int
arken_graphics_chart_Line_setHideDots(lua_State *L) {
  checkLine(L)->setHideDots(lua_toboolean(L, 2));
  return 0;
}

static int
arken_graphics_chart_Line_hideDots(lua_State *L) {
  lua_pushboolean(L, checkLine(L)->hideDots());
  return 1;
}

static int
arken_graphics_chart_Line_setHideLines(lua_State *L) {
  checkLine(L)->setHideLines(lua_toboolean(L, 2));
  return 0;
}

static int
arken_graphics_chart_Line_hideLines(lua_State *L) {
  lua_pushboolean(L, checkLine(L)->hideLines());
  return 1;
}

static int
arken_graphics_chart_Line_gc(lua_State *L) {
  delete checkLine(L);
  return 0;
}

static const luaL_reg arken_graphics_chart_Line_metatable[] = {
  {"setBaselineValue", arken_graphics_chart_Line_setBaselineValue},
  {"baselineValue",    arken_graphics_chart_Line_baselineValue},
  {"setBaselineColor", arken_graphics_chart_Line_setBaselineColor},
  {"baselineColor",    arken_graphics_chart_Line_baselineColor},
  {"setLineWidth",     arken_graphics_chart_Line_setLineWidth},
  {"lineWidth",        arken_graphics_chart_Line_lineWidth},
  {"setDotRadius",     arken_graphics_chart_Line_setDotRadius},
  {"dotRadius",        arken_graphics_chart_Line_dotRadius},
  {"setHideDots",      arken_graphics_chart_Line_setHideDots},
  {"hideDots",         arken_graphics_chart_Line_hideDots},
  {"setHideLines",     arken_graphics_chart_Line_setHideLines},
  {"hideLines",        arken_graphics_chart_Line_hideLines},
  {"__gc",             arken_graphics_chart_Line_gc},
  {nullptr, nullptr}
};

void static
register_arken_graphics_chart_Line_metatable(lua_State *L) {
  luaL_newmetatable(L, "arken.graphics.chart.Line.metatable");
  luaL_register(L, nullptr, arken_graphics_chart_Line_metatable);
  register_arken_graphics_Chart_common(L);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_graphics_chart_Line(lua_State *L) {
    register_arken_graphics_chart_Line_metatable(L);
    register_arken_graphics_chart_Line(L);
    return 1;
  }
}
