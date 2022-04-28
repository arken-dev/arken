// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/chrono/date.h>

using arken::chrono::Date;
using arken::string;

/**
 * checkDate
 */

Date *
checkDate( lua_State *L, int i = 1 ) {
  return *static_cast<Date **>(luaL_checkudata(L, i, "arken.chrono.Date.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_chrono_Date_today( lua_State *L ) {
  auto ptr = static_cast<Date **>(lua_newuserdata(L, sizeof(Date*)));
  *ptr = new Date(Date::today());
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_chrono_Date_currentDate( lua_State *L ) {
  auto ptr = static_cast<Date **>(lua_newuserdata(L, sizeof(Date*)));
  *ptr = new Date(Date::currentDate());
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_chrono_Date_parse( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  Date dt;
  if( lua_gettop(L) == 1 ) { // number of arguments
    dt = Date::parse(string);
  } else {
    const char *format = luaL_checkstring(L, 2);
    dt = Date::parse(string, format);
  }
  if( dt.isValid() ) {
    auto ptr = static_cast<Date **>(lua_newuserdata(L, sizeof(Date*)));
    *ptr = new Date(dt);
    luaL_getmetatable(L, "arken.chrono.Date.metatable");
    lua_setmetatable(L, -2);
  } else {
    lua_pushnil(L);
  }
  return 1;
}

static const luaL_reg arken_chrono_Date[] = {
  {"today",       arken_chrono_Date_today},
  {"currentDate", arken_chrono_Date_currentDate},
  {"parse",       arken_chrono_Date_parse},
  {nullptr, nullptr}
};

void static
register_arken_chrono_Date( lua_State *L ) {
  luaL_newmetatable(L,  "arken.chrono.Date");
  luaL_register(L, nullptr, arken_chrono_Date);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_chrono_Date_gc( lua_State *L ) {
  Date *udata = checkDate( L );
  delete udata;
  return 0;
}

static int
arken_chrono_Date_asctime( lua_State *L ) {
  Date * udata  = checkDate( L );
  string result = udata->asctime();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_chrono_Date_strftime( lua_State *L ) {
  Date * udata  = checkDate( L );
  const char * format = luaL_checkstring(L, 2);
  string result = udata->strftime(format);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_chrono_Date_toString( lua_State *L ) {
  Date * udata  = checkDate( L );
  string result = udata->toString();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_chrono_Date_addYears( lua_State *L ) {
  Date *t   = checkDate( L );
  int years = luaL_checkinteger(L, 2);
  Date other = t->addYears(years);

  auto ptr = static_cast<Date **>(lua_newuserdata(L, sizeof(Date*)));
  *ptr= new Date(other);
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Date_addMonths( lua_State *L ) {
  Date *t    = checkDate( L );
  int months = luaL_checkinteger(L, 2);
  Date other = t->addMonths(months);

  auto ptr = static_cast<Date **>(lua_newuserdata(L, sizeof(Date*)));
  *ptr= new Date(other);
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Date_addDays( lua_State *L ) {
  Date *t   = checkDate( L );
  int days  = luaL_checkinteger(L, 2);
  Date other = t->addDays(days);

  auto ptr = static_cast<Date **>(lua_newuserdata(L, sizeof(Date*)));
  *ptr= new Date(other);
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Date_beginningOfMonth( lua_State *L ) {
  Date *t     = checkDate( L );
  Date result = t->beginningOfMonth();

  auto ptr = static_cast<Date **>(lua_newuserdata(L, sizeof(Date*)));
  *ptr= new Date(result);
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Date_endOfMonth( lua_State *L ) {
  Date *t     = checkDate( L );
  Date result = t->endOfMonth();

  auto ptr = static_cast<Date **>(lua_newuserdata(L, sizeof(Date*)));
  *ptr= new Date(result);
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Date_isLeapYear( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isLeapYear());
  return 1;
}

static int
arken_chrono_Date_isSunday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isSunday());
  return 1;
}

static int
arken_chrono_Date_isMonday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isMonday());
  return 1;
}

static int
arken_chrono_Date_isTuesday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isTuesday());
  return 1;
}

static int
arken_chrono_Date_isWednesday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isWednesday());
  return 1;
}

static int
arken_chrono_Date_isThursday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isThursday());
  return 1;
}

static int
arken_chrono_Date_isFriday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isFriday());
  return 1;
}

static int
arken_chrono_Date_isSaturday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isSaturday());
  return 1;
}

static int
arken_chrono_Date_isValid( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isValid());
  return 1;
}

static int
arken_chrono_Date_year( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->year());
  return 1;
}

static int
arken_chrono_Date_mon( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->mon());
  return 1;
}

static int
arken_chrono_Date_month( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->month());
  return 1;
}

static int
arken_chrono_Date_day( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->day());
  return 1;
}

static int
arken_chrono_Date_mday( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->mday());
  return 1;
}

static int
arken_chrono_Date_wday( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->wday());
  return 1;
}

static int
arken_chrono_Date_dayOfWeek( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->dayOfWeek());
  return 1;
}

static int
arken_chrono_Date_yday( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->yday());
  return 1;
}

static int
arken_chrono_Date_dayOfYear( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushinteger(L, t->dayOfYear());
  return 1;
}

static int
arken_chrono_Date_daysInMonth( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushinteger(L, t->daysInMonth());
  return 1;
}

static int
arken_chrono_Date_time( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushnumber(L, t->time());
  return 1;
}

static int
arken_chrono_Date_lessEqual( lua_State *L ) {
  Date *dt1 = checkDate( L );
  Date *dt2 = *static_cast<Date **>(luaL_checkudata(L, 2, "arken.chrono.Date.metatable"));

  lua_pushboolean(L, (*dt1) <= (*dt2));
  return 1;
}

static int
arken_chrono_Date_lessThan( lua_State *L ) {
  Date *dt1 = checkDate( L );
  Date *dt2 = *static_cast<Date **>(luaL_checkudata(L, 2, "arken.chrono.Date.metatable"));

  lua_pushboolean(L, (*dt1) < (*dt2));
  return 1;
}

static int
arken_chrono_Date_equal( lua_State *L ) {
  Date *dt1 = checkDate( L );
  Date *dt2 = *static_cast<Date **>(luaL_checkudata(L, 2, "arken.chrono.Date.metatable"));
  lua_pushboolean(L, (*dt1) == (*dt2));
  return 1;
}

static int
arken_chrono_Date_concat( lua_State *L ) {
  Date *dt;
  string concat;
  const char * str;
  const char * format = "%Y-%m-%d";
  if(lua_isstring(L, 1)) {
    str = lua_tostring(L, 1);
    dt  = checkDate( L, 2 );
    concat = dt->strftime(format).prepend(str);
  } else {
    dt  = checkDate( L, 1 );
    str = lua_tostring(L, 2);
    concat = dt->strftime(format).append(str);
  }
  lua_pushstring(L, concat);
  return 1;
}

static const
luaL_reg arken_chrono_Date_metatable[] = {
  {"asctime",          arken_chrono_Date_asctime},
  {"strftime",         arken_chrono_Date_strftime},
  {"addYears",         arken_chrono_Date_addYears},
  {"addMonths",        arken_chrono_Date_addMonths},
  {"addDays",          arken_chrono_Date_addDays},
  {"beginningOfMonth", arken_chrono_Date_beginningOfMonth},
  {"endOfMonth",       arken_chrono_Date_endOfMonth},
  {"isLeapYear",       arken_chrono_Date_isLeapYear},
  {"isSunday",         arken_chrono_Date_isSunday},
  {"isMonday",         arken_chrono_Date_isMonday},
  {"isTuesday",        arken_chrono_Date_isTuesday},
  {"isWednesday",      arken_chrono_Date_isWednesday},
  {"isThursday",       arken_chrono_Date_isThursday},
  {"isFriday",         arken_chrono_Date_isFriday},
  {"isSaturday",       arken_chrono_Date_isSaturday},
  {"isValid",          arken_chrono_Date_isValid},
  {"year",             arken_chrono_Date_year},
  {"month",            arken_chrono_Date_month},
  {"mon",              arken_chrono_Date_mon},
  {"day",              arken_chrono_Date_day},
  {"mday",             arken_chrono_Date_mday},
  {"wday",             arken_chrono_Date_wday},
  {"yday",             arken_chrono_Date_yday},
  {"daysInMonth",      arken_chrono_Date_daysInMonth},
  {"dayOfWeek",        arken_chrono_Date_dayOfWeek},
  {"dayOfYear",        arken_chrono_Date_dayOfYear},
  {"time",             arken_chrono_Date_time},
  {"toString",         arken_chrono_Date_toString},
  {"__tostring",       arken_chrono_Date_toString},
  {"__gc",             arken_chrono_Date_gc},
  {"__lt",             arken_chrono_Date_lessThan},
  {"__le",             arken_chrono_Date_lessEqual},
  {"__eq",             arken_chrono_Date_equal},
  {"__concat",         arken_chrono_Date_concat},
  {nullptr, nullptr}
};

void static
register_arken_chrono_Date_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.chrono.Date.metatable");
  luaL_register(L, nullptr, arken_chrono_Date_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_chrono_Date( lua_State *L ) {
    register_arken_chrono_Date_metatable(L);
    register_arken_chrono_Date(L);
    return 1;
  }
}
