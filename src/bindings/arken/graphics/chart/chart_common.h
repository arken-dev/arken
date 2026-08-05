// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Shared Lua bindings for the arken::graphics::chart::Base API. Included by
// each concrete chart binding (bar.cpp, line.cpp, pie.cpp, area.cpp) so the
// ~70 inherited setters/getters are written once instead of four times.
// Every wrapper below works against any of the four chart userdata types,
// since Bar/Line/Pie/Area are plain (non-virtual, single) subclasses of
// chart::Base and therefore share the same object address.

#ifndef _ARKEN_BINDINGS_GRAPHICS_CHART_COMMON_
#define _ARKEN_BINDINGS_GRAPHICS_CHART_COMMON_

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/graphics/chart.h>

using arken::graphics::chart::Base;

static inline Base *
checkChart(lua_State *L, int position = 1) {
  return *static_cast<Base **>(lua_touserdata(L, position));
}

//------------------------------------------------------------------------------
// macros for the many trivial setter/getter pairs
//------------------------------------------------------------------------------

#define CHART_SET_DOUBLE(m) \
static int arken_graphics_Chart_##m(lua_State *L) { \
  checkChart(L)->m(luaL_checknumber(L, 2)); \
  return 0; \
}

#define CHART_GET_DOUBLE(m) \
static int arken_graphics_Chart_##m(lua_State *L) { \
  lua_pushnumber(L, checkChart(L)->m()); \
  return 1; \
}

#define CHART_SET_INT(m) \
static int arken_graphics_Chart_##m(lua_State *L) { \
  checkChart(L)->m((int) luaL_checkinteger(L, 2)); \
  return 0; \
}

#define CHART_GET_INT(m) \
static int arken_graphics_Chart_##m(lua_State *L) { \
  lua_pushinteger(L, checkChart(L)->m()); \
  return 1; \
}

#define CHART_SET_BOOL(m) \
static int arken_graphics_Chart_##m(lua_State *L) { \
  checkChart(L)->m(lua_toboolean(L, 2)); \
  return 0; \
}

#define CHART_GET_BOOL(m) \
static int arken_graphics_Chart_##m(lua_State *L) { \
  lua_pushboolean(L, checkChart(L)->m()); \
  return 1; \
}

#define CHART_SET_STRING(m) \
static int arken_graphics_Chart_##m(lua_State *L) { \
  size_t len; \
  const char *value = luaL_checklstring(L, 2, &len); \
  checkChart(L)->m(string(value, len)); \
  return 0; \
}

#define CHART_GET_STRING(m) \
static int arken_graphics_Chart_##m(lua_State *L) { \
  string value = checkChart(L)->m(); \
  lua_pushlstring(L, value.data(), value.size()); \
  return 1; \
}

#define CHART_ACTION0(m) \
static int arken_graphics_Chart_##m(lua_State *L) { \
  checkChart(L)->m(); \
  return 0; \
}

//------------------------------------------------------------------------------
// margins
//------------------------------------------------------------------------------

CHART_SET_DOUBLE(setTopMargin)    CHART_GET_DOUBLE(topMargin)
CHART_SET_DOUBLE(setBottomMargin) CHART_GET_DOUBLE(bottomMargin)
CHART_SET_DOUBLE(setLeftMargin)   CHART_GET_DOUBLE(leftMargin)
CHART_SET_DOUBLE(setRightMargin)  CHART_GET_DOUBLE(rightMargin)
CHART_SET_DOUBLE(setMargins)
CHART_SET_DOUBLE(setTitleMargin)  CHART_GET_DOUBLE(titleMargin)
CHART_SET_DOUBLE(setLegendMargin) CHART_GET_DOUBLE(legendMargin)

//------------------------------------------------------------------------------
// labels / axis
//------------------------------------------------------------------------------

CHART_SET_BOOL(setCenterLabelsOverPoint) CHART_GET_BOOL(centerLabelsOverPoint)
CHART_SET_BOOL(setHasLeftLabels)         CHART_GET_BOOL(hasLeftLabels)

CHART_SET_STRING(setXAxisLabel) CHART_GET_STRING(xAxisLabel)
CHART_SET_STRING(setYAxisLabel) CHART_GET_STRING(yAxisLabel)
CHART_SET_DOUBLE(setYAxisIncrement) CHART_GET_DOUBLE(yAxisIncrement)

//------------------------------------------------------------------------------
// title / font
//------------------------------------------------------------------------------

CHART_SET_STRING(setTitle) CHART_GET_STRING(title)
CHART_SET_STRING(setFont)  CHART_GET_STRING(font)
CHART_SET_STRING(setFontColor) CHART_GET_STRING(fontColor)

//------------------------------------------------------------------------------
// hide flags
//------------------------------------------------------------------------------

CHART_SET_BOOL(setHideLineMarkers) CHART_GET_BOOL(hideLineMarkers)
CHART_SET_BOOL(setHideLegend)      CHART_GET_BOOL(hideLegend)
CHART_SET_BOOL(setHideTitle)       CHART_GET_BOOL(hideTitle)
CHART_SET_BOOL(setHideLineNumbers) CHART_GET_BOOL(hideLineNumbers)

CHART_SET_STRING(setNoDataMessage) CHART_GET_STRING(noDataMessage)

//------------------------------------------------------------------------------
// font sizes / marker / min-max / sort / legend box
//------------------------------------------------------------------------------

CHART_SET_DOUBLE(setTitleFontSize)  CHART_GET_DOUBLE(titleFontSize)
CHART_SET_DOUBLE(setLegendFontSize) CHART_GET_DOUBLE(legendFontSize)
CHART_SET_DOUBLE(setMarkerFontSize) CHART_GET_DOUBLE(markerFontSize)

CHART_SET_STRING(setMarkerColor) CHART_GET_STRING(markerColor)
CHART_SET_INT(setMarkerCount)    CHART_GET_INT(markerCount)

CHART_SET_DOUBLE(setMinimumValue) CHART_GET_DOUBLE(minimumValue)
CHART_SET_DOUBLE(setMaximumValue) CHART_GET_DOUBLE(maximumValue)

CHART_SET_BOOL(setSort) CHART_GET_BOOL(sort)

CHART_SET_DOUBLE(setLegendBoxSize) CHART_GET_DOUBLE(legendBoxSize)

//------------------------------------------------------------------------------
// themes
//------------------------------------------------------------------------------

CHART_ACTION0(themeKeynote)
CHART_ACTION0(theme37signals)
CHART_ACTION0(themeRailsKeynote)
CHART_ACTION0(themeOdeo)
CHART_ACTION0(themePastel)
CHART_ACTION0(themeGreyscale)

//------------------------------------------------------------------------------
// data(name, value|{values...}, color?)
//------------------------------------------------------------------------------

static int
arken_graphics_Chart_data(lua_State *L) {
  Base *chart = checkChart(L);

  size_t nameLen;
  const char *name = luaL_checklstring(L, 2, &nameLen);

  string color = lua_isstring(L, 4) ? string(luaL_checkstring(L, 4)) : string("");

  if (lua_istable(L, 3)) {
    std::vector<double> values;
    size_t n = lua_objlen(L, 3);
    for (size_t i = 1; i <= n; i++) {
      lua_rawgeti(L, 3, (int) i);
      values.push_back(luaL_checknumber(L, -1));
      lua_pop(L, 1);
    }
    chart->data(string(name, nameLen), values, color);
  } else {
    double value = luaL_checknumber(L, 3);
    chart->data(string(name, nameLen), value, color);
  }

  return 0;
}

//------------------------------------------------------------------------------
// colors
//------------------------------------------------------------------------------

static int
arken_graphics_Chart_addColor(lua_State *L) {
  Base *chart = checkChart(L);
  chart->addColor(string(luaL_checkstring(L, 2)));
  return 0;
}

static int
arken_graphics_Chart_setColors(lua_State *L) {
  Base *chart = checkChart(L);
  luaL_checktype(L, 2, LUA_TTABLE);

  std::vector<string> colors;
  size_t n = lua_objlen(L, 2);
  for (size_t i = 1; i <= n; i++) {
    lua_rawgeti(L, 2, (int) i);
    colors.push_back(string(luaL_checkstring(L, -1)));
    lua_pop(L, 1);
  }

  chart->setColors(colors);
  return 0;
}

static int
arken_graphics_Chart_colors(lua_State *L) {
  Base *chart = checkChart(L);
  std::vector<string> colors = chart->colors();

  lua_newtable(L);
  for (size_t i = 0; i < colors.size(); i++) {
    lua_pushlstring(L, colors[i].data(), colors[i].size());
    lua_rawseti(L, -2, (int) (i + 1));
  }
  return 1;
}

//------------------------------------------------------------------------------
// labels
//------------------------------------------------------------------------------

static int
arken_graphics_Chart_addLabel(lua_State *L) {
  Base *chart = checkChart(L);
  int index = (int) luaL_checkinteger(L, 2);
  chart->addLabel(index, string(luaL_checkstring(L, 3)));
  return 0;
}

static int
arken_graphics_Chart_setLabels(lua_State *L) {
  Base *chart = checkChart(L);
  luaL_checktype(L, 2, LUA_TTABLE);

  std::map<int, string> labels;

  lua_pushnil(L);
  while (lua_next(L, 2) != 0) {
    int index = (int) luaL_checkinteger(L, -2);
    labels[index] = string(luaL_checkstring(L, -1));
    lua_pop(L, 1);
  }

  chart->setLabels(labels);
  return 0;
}

//------------------------------------------------------------------------------
// output
//------------------------------------------------------------------------------

static int
arken_graphics_Chart_write(lua_State *L) {
  Base *chart = checkChart(L);
  const char *filename = lua_isstring(L, 2) ? luaL_checkstring(L, 2) : "graph.png";
  chart->write(string(filename));
  return 0;
}

//------------------------------------------------------------------------------
// shared metatable entries, layered onto a type's own metatable via a second
// luaL_register() call (the class-specific bindings register their own
// entries first, then call register_arken_graphics_Chart_common(L)).
//------------------------------------------------------------------------------

static const luaL_reg arken_graphics_Chart_metatable[] = {
  {"data",                     arken_graphics_Chart_data},
  {"addColor",                 arken_graphics_Chart_addColor},
  {"setColors",                arken_graphics_Chart_setColors},
  {"colors",                   arken_graphics_Chart_colors},
  {"themeKeynote",             arken_graphics_Chart_themeKeynote},
  {"theme37signals",           arken_graphics_Chart_theme37signals},
  {"themeRailsKeynote",        arken_graphics_Chart_themeRailsKeynote},
  {"themeOdeo",                arken_graphics_Chart_themeOdeo},
  {"themePastel",               arken_graphics_Chart_themePastel},
  {"themeGreyscale",           arken_graphics_Chart_themeGreyscale},
  {"addLabel",                 arken_graphics_Chart_addLabel},
  {"setLabels",                arken_graphics_Chart_setLabels},
  {"setTopMargin",             arken_graphics_Chart_setTopMargin},
  {"topMargin",                arken_graphics_Chart_topMargin},
  {"setBottomMargin",          arken_graphics_Chart_setBottomMargin},
  {"bottomMargin",             arken_graphics_Chart_bottomMargin},
  {"setLeftMargin",            arken_graphics_Chart_setLeftMargin},
  {"leftMargin",               arken_graphics_Chart_leftMargin},
  {"setRightMargin",           arken_graphics_Chart_setRightMargin},
  {"rightMargin",              arken_graphics_Chart_rightMargin},
  {"setMargins",               arken_graphics_Chart_setMargins},
  {"setTitleMargin",           arken_graphics_Chart_setTitleMargin},
  {"titleMargin",              arken_graphics_Chart_titleMargin},
  {"setLegendMargin",          arken_graphics_Chart_setLegendMargin},
  {"legendMargin",             arken_graphics_Chart_legendMargin},
  {"setCenterLabelsOverPoint", arken_graphics_Chart_setCenterLabelsOverPoint},
  {"centerLabelsOverPoint",    arken_graphics_Chart_centerLabelsOverPoint},
  {"setHasLeftLabels",         arken_graphics_Chart_setHasLeftLabels},
  {"hasLeftLabels",            arken_graphics_Chart_hasLeftLabels},
  {"setXAxisLabel",            arken_graphics_Chart_setXAxisLabel},
  {"xAxisLabel",               arken_graphics_Chart_xAxisLabel},
  {"setYAxisLabel",            arken_graphics_Chart_setYAxisLabel},
  {"yAxisLabel",               arken_graphics_Chart_yAxisLabel},
  {"setYAxisIncrement",        arken_graphics_Chart_setYAxisIncrement},
  {"yAxisIncrement",           arken_graphics_Chart_yAxisIncrement},
  {"setTitle",                 arken_graphics_Chart_setTitle},
  {"title",                    arken_graphics_Chart_title},
  {"setFont",                  arken_graphics_Chart_setFont},
  {"font",                     arken_graphics_Chart_font},
  {"setFontColor",             arken_graphics_Chart_setFontColor},
  {"fontColor",                arken_graphics_Chart_fontColor},
  {"setHideLineMarkers",       arken_graphics_Chart_setHideLineMarkers},
  {"hideLineMarkers",          arken_graphics_Chart_hideLineMarkers},
  {"setHideLegend",            arken_graphics_Chart_setHideLegend},
  {"hideLegend",               arken_graphics_Chart_hideLegend},
  {"setHideTitle",             arken_graphics_Chart_setHideTitle},
  {"hideTitle",                arken_graphics_Chart_hideTitle},
  {"setHideLineNumbers",       arken_graphics_Chart_setHideLineNumbers},
  {"hideLineNumbers",          arken_graphics_Chart_hideLineNumbers},
  {"setNoDataMessage",         arken_graphics_Chart_setNoDataMessage},
  {"noDataMessage",            arken_graphics_Chart_noDataMessage},
  {"setTitleFontSize",         arken_graphics_Chart_setTitleFontSize},
  {"titleFontSize",            arken_graphics_Chart_titleFontSize},
  {"setLegendFontSize",        arken_graphics_Chart_setLegendFontSize},
  {"legendFontSize",           arken_graphics_Chart_legendFontSize},
  {"setMarkerFontSize",        arken_graphics_Chart_setMarkerFontSize},
  {"markerFontSize",           arken_graphics_Chart_markerFontSize},
  {"setMarkerColor",           arken_graphics_Chart_setMarkerColor},
  {"markerColor",              arken_graphics_Chart_markerColor},
  {"setMarkerCount",           arken_graphics_Chart_setMarkerCount},
  {"markerCount",              arken_graphics_Chart_markerCount},
  {"setMinimumValue",          arken_graphics_Chart_setMinimumValue},
  {"minimumValue",             arken_graphics_Chart_minimumValue},
  {"setMaximumValue",          arken_graphics_Chart_setMaximumValue},
  {"maximumValue",             arken_graphics_Chart_maximumValue},
  {"setSort",                  arken_graphics_Chart_setSort},
  {"sort",                     arken_graphics_Chart_sort},
  {"setLegendBoxSize",         arken_graphics_Chart_setLegendBoxSize},
  {"legendBoxSize",            arken_graphics_Chart_legendBoxSize},
  {"write",                    arken_graphics_Chart_write},
  {nullptr, nullptr}
};

void static
register_arken_graphics_Chart_common(lua_State *L) {
  luaL_register(L, nullptr, arken_graphics_Chart_metatable);
}

#endif
