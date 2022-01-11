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
  return *(Date **) luaL_checkudata(L, i, "arken.chrono.Date.metatable");
}


/**
 * ClassMethods
 */

static int
arken_DateClassMethodToday( lua_State *L ) {
  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr = new Date(Date::today());
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_DateClassMethodCurrentDate( lua_State *L ) {
  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr = new Date(Date::currentDate());
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_DateClassMethodParse( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  Date dt;
  if( lua_gettop(L) == 1 ) { // number of arguments
    dt = Date::parse(string);
  } else {
    const char *format = luaL_checkstring(L, 2);
    dt = Date::parse(string, format);
  }
  if( dt.isValid() ) {
    Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
    *ptr = new Date(dt);
    luaL_getmetatable(L, "arken.chrono.Date.metatable");
    lua_setmetatable(L, -2);
  } else {
    lua_pushnil(L);
  }
  return 1;
}

static const luaL_reg DateClassMethods[] = {
  {"today",       arken_DateClassMethodToday},
  {"currentDate", arken_DateClassMethodCurrentDate},
  {"parse",       arken_DateClassMethodParse},
  {NULL, NULL}
};

void static
registerDateClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.chrono.Date");
  luaL_register(L, NULL, DateClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_DateInstanceMethodDestruct( lua_State *L ) {
  Date *udata = checkDate( L );
  delete udata;
  return 0;
}

static int
arken_DateInstanceMethodAsctime( lua_State *L ) {
  Date * udata  = checkDate( L );
  string result = udata->asctime();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_DateInstanceMethodStrftime( lua_State *L ) {
  Date * udata  = checkDate( L );
  const char * format = luaL_checkstring(L, 2);
  string result = udata->strftime(format);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_DateInstanceMethodToString( lua_State *L ) {
  Date * udata  = checkDate( L );
  string result = udata->toString();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_DateInstanceMethodAddYears( lua_State *L ) {
  Date *t   = checkDate( L );
  int years = luaL_checkinteger(L, 2);
  Date other = t->addYears(years);

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(other);
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateInstanceMethodAddMonths( lua_State *L ) {
  Date *t    = checkDate( L );
  int months = luaL_checkinteger(L, 2);
  Date other = t->addMonths(months);

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(other);
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateInstanceMethodAddDays( lua_State *L ) {
  Date *t   = checkDate( L );
  int days  = luaL_checkinteger(L, 2);
  Date other = t->addDays(days);

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(other);
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateInstanceMethodBeginningOfMonth( lua_State *L ) {
  Date *t     = checkDate( L );
  Date result = t->beginningOfMonth();

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(result);
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateInstanceMethodEndOfMonth( lua_State *L ) {
  Date *t     = checkDate( L );
  Date result = t->endOfMonth();

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(result);
  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateInstanceMethodIsLeapYear( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isLeapYear());
  return 1;
}

static int
arken_DateInstanceMethodIsSunday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isSunday());
  return 1;
}

static int
arken_DateInstanceMethodIsMonday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isMonday());
  return 1;
}

static int
arken_DateInstanceMethodIsTuesday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isTuesday());
  return 1;
}

static int
arken_DateInstanceMethodIsWednesday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isWednesday());
  return 1;
}

static int
arken_DateInstanceMethodIsThursday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isThursday());
  return 1;
}

static int
arken_DateInstanceMethodIsFriday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isFriday());
  return 1;
}

static int
arken_DateInstanceMethodIsSaturday( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isSaturday());
  return 1;
}

static int
arken_DateInstanceMethodIsValid( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushboolean(L, t->isValid());
  return 1;
}

static int
arken_DateInstanceMethodYear( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->year());
  return 1;
}

static int
arken_DateInstanceMethodMon( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->mon());
  return 1;
}

static int
arken_DateInstanceMethodMonth( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->month());
  return 1;
}

static int
arken_DateInstanceMethodDay( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->day());
  return 1;
}

static int
arken_DateInstanceMethodMday( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->mday());
  return 1;
}

static int
arken_DateInstanceMethodWday( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->wday());
  return 1;
}

static int
arken_DateInstanceMethodDayOfWeek( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->dayOfWeek());
  return 1;
}

static int
arken_DateInstanceMethodYday( lua_State *L ) {
  Date *t   = checkDate( L );
  lua_pushinteger(L, t->yday());
  return 1;
}

static int
arken_DateInstanceMethodDayOfYear( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushinteger(L, t->dayOfYear());
  return 1;
}

static int
arken_DateInstanceMethodDaysInMonth( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushinteger(L, t->daysInMonth());
  return 1;
}

static int
arken_DateInstanceMethodDate( lua_State *L ) {
  Date *t = checkDate( L );
  lua_pushnumber(L, t->time());
  return 1;
}

static int
arken_DateInstanceMethodLessEqual( lua_State *L ) {
  Date *dt1 = checkDate( L );
  Date *dt2 = *(Date **) luaL_checkudata(L, 2, "arken.chrono.Date.metatable");

  lua_pushboolean(L, (*dt1) <= (*dt2));
  return 1;
}

static int
arken_DateInstanceMethodLessThan( lua_State *L ) {
  Date *dt1 = checkDate( L );
  Date *dt2 = *(Date **) luaL_checkudata(L, 2, "arken.chrono.Date.metatable");

  lua_pushboolean(L, (*dt1) < (*dt2));
  return 1;
}

static int
arken_DateInstanceMethodEqual( lua_State *L ) {
  Date *dt1 = checkDate( L );
  Date *dt2 = *(Date **) luaL_checkudata(L, 2, "arken.chrono.Date.metatable");
  lua_pushboolean(L, (*dt1) == (*dt2));
  return 1;
}

static int
arken_DateInstanceMethodConcat( lua_State *L ) {
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
luaL_reg DateInstanceMethods[] = {
  {"asctime",          arken_DateInstanceMethodAsctime},
  {"strftime",         arken_DateInstanceMethodStrftime},
  {"addYears",         arken_DateInstanceMethodAddYears},
  {"addMonths",        arken_DateInstanceMethodAddMonths},
  {"addDays",          arken_DateInstanceMethodAddDays},
  {"beginningOfMonth", arken_DateInstanceMethodBeginningOfMonth},
  {"endOfMonth",       arken_DateInstanceMethodEndOfMonth},
  {"isLeapYear",       arken_DateInstanceMethodIsLeapYear},
  {"isSunday",         arken_DateInstanceMethodIsSunday},
  {"isMonday",         arken_DateInstanceMethodIsMonday},
  {"isTuesday",        arken_DateInstanceMethodIsTuesday},
  {"isWednesday",      arken_DateInstanceMethodIsWednesday},
  {"isThursday",       arken_DateInstanceMethodIsThursday},
  {"isFriday",         arken_DateInstanceMethodIsFriday},
  {"isSaturday",       arken_DateInstanceMethodIsSaturday},
  {"isValid",          arken_DateInstanceMethodIsValid},
  {"year",             arken_DateInstanceMethodYear},
  {"month",            arken_DateInstanceMethodMonth},
  {"mon",              arken_DateInstanceMethodMon},
  {"day",              arken_DateInstanceMethodDay},
  {"mday",             arken_DateInstanceMethodMday},
  {"wday",             arken_DateInstanceMethodWday},
  {"yday",             arken_DateInstanceMethodYday},
  {"daysInMonth",      arken_DateInstanceMethodDaysInMonth},
  {"dayOfWeek",        arken_DateInstanceMethodDayOfWeek},
  {"dayOfYear",        arken_DateInstanceMethodDayOfYear},
  {"time",             arken_DateInstanceMethodDate},
  {"toString",         arken_DateInstanceMethodToString},
  {"__tostring",       arken_DateInstanceMethodToString},
  {"__gc",             arken_DateInstanceMethodDestruct},
  {"__lt",             arken_DateInstanceMethodLessThan},
  {"__le",             arken_DateInstanceMethodLessEqual},
  {"__eq",             arken_DateInstanceMethodEqual},
  {"__concat",         arken_DateInstanceMethodConcat},
  {NULL, NULL}
};

void static
registerDateInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.chrono.Date.metatable");
  luaL_register(L, NULL, DateInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_chrono_Date( lua_State *L ) {
    registerDateInstanceMethods(L);
    registerDateClassMethods(L);
    return 1;
  }
}
