// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/chrono/time.h>

using arken::chrono::Date;
using arken::chrono::Time;
using arken::string;

/**
 * checkTime
 */

Time *
checkTime( lua_State *L , int i = 1) {
  return *(Time **) luaL_checkudata(L, i, "arken.chrono.Time.metatable");
}

/**
 * ClassMethods
 */

static int
arken_TimeClassMethodToday( lua_State *L ) {
  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr = new Time((Time) Time::today());
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_TimeClassMethodNow( lua_State *L ) {
  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr = new Time(Time::now());
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_TimeClassMethodCurrentDateTime( lua_State *L ) {
  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr = new Time(Time::currentDateTime());
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_TimeClassMethodParse( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  Time time;
  if( lua_gettop(L) == 1 ) { // number of arguments
    time = Time::parse(string);
  } else {
    const char *format = luaL_checkstring(L, 2);
    time = Time::parse(string, format);
  }
  if( time.isValid() ) {
    Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
    *ptr = new Time(time);
    luaL_getmetatable(L, "arken.chrono.Time.metatable");
    lua_setmetatable(L, -2);
  } else {
    lua_pushnil(L);
  }
  return 1;
}

static const luaL_reg TimeClassMethods[] = {
  {"now",             arken_TimeClassMethodNow},
  {"currentDateTime", arken_TimeClassMethodCurrentDateTime},
  {"today",           arken_TimeClassMethodToday},
  {"parse",           arken_TimeClassMethodParse},
  {NULL, NULL}
};

void static
registerTimeClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.chrono.Time");
  luaL_register(L, NULL, TimeClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_TimeInstanceMethodDestruct( lua_State *L ) {
  Time *udata = checkTime( L );
  delete udata;
  return 0;
}

static int
arken_TimeInstanceMethodAsctime( lua_State *L ) {
  Time * udata  = checkTime( L );
  string result = udata->asctime();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_TimeInstanceMethodStrftime( lua_State *L ) {
  Time * udata  = checkTime( L );
  const char * format = luaL_checkstring(L, 2);
  string result = udata->strftime(format);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_TimeInstanceMethodToString( lua_State *L ) {
  Time * udata  = checkTime( L );
  string result = udata->toString();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_TimeInstanceMethodAddYears( lua_State *L ) {
  Time *t   = checkTime( L );
  int years = luaL_checkinteger(L, 2);
  Time other = t->addYears(years);

  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_TimeInstanceMethodAddMonths( lua_State *L ) {
  Time *t   = checkTime( L );
  int months = luaL_checkinteger(L, 2);
  Time other = t->addMonths(months);

  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_TimeInstanceMethodAddDays( lua_State *L ) {
  Time *t   = checkTime( L );
  int days  = luaL_checkinteger(L, 2);
  Time other = t->addDays(days);

  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_TimeInstanceMethodAddHours( lua_State *L ) {
  Time *t   = checkTime( L );
  int hours = luaL_checkinteger(L, 2);
  Time other = t->addHours(hours);

  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_TimeInstanceMethodAddMinutes( lua_State *L ) {
  Time *t     = checkTime( L );
  int minutes = luaL_checkinteger(L, 2);
  Time other  = t->addMinutes(minutes);

  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}


static int
arken_TimeInstanceMethodAddSecs( lua_State *L ) {
  Time *t   = checkTime( L );
  int secs  = luaL_checkinteger(L, 2);
  Time other = t->addSecs(secs);

  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_TimeInstanceMethodBeginningOfDay( lua_State *L ) {
  Time *t     = checkTime( L );
  Time result = t->beginningOfDay();

  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr= new Time(result);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_TimeInstanceMethodEndOfDay( lua_State *L ) {
  Time *t     = checkTime( L );
  Time result = t->endOfDay();

  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr= new Time(result);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_TimeInstanceMethodBeginningOfMonth( lua_State *L ) {
  Time *t     = checkTime( L );
  Time result = t->beginningOfMonth();

  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr= new Time(result);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_TimeInstanceMethodEndOfMonth( lua_State *L ) {
  Time *t     = checkTime( L );
  Time result = t->endOfMonth();

  Time **ptr = (Time **)lua_newuserdata(L, sizeof(Time*));
  *ptr= new Time(result);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_TimeInstanceMethodIsLeapYear( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isLeapYear());
  return 1;
}

static int
arken_TimeInstanceMethodIsSunday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isSunday());
  return 1;
}

static int
arken_TimeInstanceMethodIsMonday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isMonday());
  return 1;
}

static int
arken_TimeInstanceMethodIsTuesday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isTuesday());
  return 1;
}

static int
arken_TimeInstanceMethodIsWednesday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isWednesday());
  return 1;
}

static int
arken_TimeInstanceMethodIsThursday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isThursday());
  return 1;
}

static int
arken_TimeInstanceMethodIsFriday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isFriday());
  return 1;
}

static int
arken_TimeInstanceMethodIsSaturday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isSaturday());
  return 1;
}

static int
arken_TimeInstanceMethodIsValid( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isValid());
  return 1;
}

static int
arken_TimeInstanceMethodYear( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->year());
  return 1;
}

static int
arken_TimeInstanceMethodMon( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->mon());
  return 1;
}

static int
arken_TimeInstanceMethodMonth( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->month());
  return 1;
}

static int
arken_TimeInstanceMethodDay( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->day());
  return 1;
}

static int
arken_TimeInstanceMethodHour( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->hour());
  return 1;
}

static int
arken_TimeInstanceMethodMin( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->min());
  return 1;
}


static int
arken_TimeInstanceMethodSec( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->sec());
  return 1;
}

static int
arken_TimeInstanceMethodMday( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->mday());
  return 1;
}

static int
arken_TimeInstanceMethodWday( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->wday());
  return 1;
}

static int
arken_TimeInstanceMethodDayOfWeek( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->dayOfWeek());
  return 1;
}

static int
arken_TimeInstanceMethodYday( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->yday());
  return 1;
}

static int
arken_TimeInstanceMethodDayOfYear( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->dayOfYear());
  return 1;
}

static int
arken_TimeInstanceMethodDaysInMonth( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushinteger(L, t->daysInMonth());
  return 1;
}

static int
arken_TimeInstanceMethodTime( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushnumber(L, t->time());
  return 1;
}

static int
arken_TimeInstanceMethodDate( lua_State *L ) {
  Time *t = checkTime( L );

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(t->date());

  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_TimeInstanceMethodLessEqual( lua_State *L ) {
  Time *t1 = checkTime( L );
  Time *t2 = *(Time **) luaL_checkudata(L, 2, "arken.chrono.Time.metatable");

  lua_pushboolean(L, (*t1) <= (*t2));
  return 1;
}

static int
arken_TimeInstanceMethodLessThan( lua_State *L ) {
  Time *dt1 = checkTime( L );
  Time *dt2 = *(Time **) luaL_checkudata(L, 2, "arken.chrono.Time.metatable");

  lua_pushboolean(L, (*dt1) < (*dt2));
  return 1;
}

static int
arken_TimeInstanceMethodEqual( lua_State *L ) {
  Time *t1 = checkTime( L );
  Time *t2 = *(Time **) luaL_checkudata(L, 2, "arken.chrono.Time.metatable");
  lua_pushboolean(L, (*t1) == (*t2));
  return 1;
}

static int
arken_TimeInstanceMethodConcat( lua_State *L ) {
  Time *dt;
  string concat;
  const char * str;
  const char * format = "%Y-%m-%d %H:%M:%S";
  if(lua_isstring(L, 1)) {
    str = lua_tostring(L, 1);
    dt  = checkTime( L, 2 );
    concat = dt->strftime(format).prepend(str);
  } else {
    dt  = checkTime( L, 1 );
    str = lua_tostring(L, 2);
    concat = dt->strftime(format).append(str);
  }
  lua_pushstring(L, concat);
  return 1;
}

static const
luaL_reg TimeInstanceMethods[] = {
  {"asctime",          arken_TimeInstanceMethodAsctime},
  {"strftime",         arken_TimeInstanceMethodStrftime},
  {"addYears",         arken_TimeInstanceMethodAddYears},
  {"addMonths",        arken_TimeInstanceMethodAddMonths},
  {"addDays",          arken_TimeInstanceMethodAddDays},
  {"addHours",         arken_TimeInstanceMethodAddHours},
  {"addMinutes",       arken_TimeInstanceMethodAddMinutes},
  {"addSecs",          arken_TimeInstanceMethodAddSecs},
  {"beginningOfDay",   arken_TimeInstanceMethodBeginningOfDay},
  {"endOfDay",         arken_TimeInstanceMethodEndOfDay},
  {"beginningOfMonth", arken_TimeInstanceMethodBeginningOfMonth},
  {"endOfMonth",       arken_TimeInstanceMethodEndOfMonth},
  {"isLeapYear",       arken_TimeInstanceMethodIsLeapYear},
  {"isSunday",         arken_TimeInstanceMethodIsSunday},
  {"isMonday",         arken_TimeInstanceMethodIsMonday},
  {"isTuesday",        arken_TimeInstanceMethodIsTuesday},
  {"isWednesday",      arken_TimeInstanceMethodIsWednesday},
  {"isThursday",       arken_TimeInstanceMethodIsThursday},
  {"isFriday",         arken_TimeInstanceMethodIsFriday},
  {"isSaturday",       arken_TimeInstanceMethodIsSaturday},
  {"isValid",          arken_TimeInstanceMethodIsValid},
  {"year",             arken_TimeInstanceMethodYear},
  {"month",            arken_TimeInstanceMethodMonth},
  {"mon",              arken_TimeInstanceMethodMon},
  {"day",              arken_TimeInstanceMethodDay},
  {"hour",             arken_TimeInstanceMethodHour},
  {"min",              arken_TimeInstanceMethodMin},
  {"sec",              arken_TimeInstanceMethodSec},
  {"mday",             arken_TimeInstanceMethodMday},
  {"wday",             arken_TimeInstanceMethodWday},
  {"yday",             arken_TimeInstanceMethodYday},
  {"dayofWeek",        arken_TimeInstanceMethodDayOfWeek},
  {"dayOfYear",        arken_TimeInstanceMethodDayOfYear},
  {"daysInMonth",      arken_TimeInstanceMethodDaysInMonth},
  {"time",             arken_TimeInstanceMethodTime},
  {"date",             arken_TimeInstanceMethodDate},
  {"toString",         arken_TimeInstanceMethodToString},
  {"__tostring",       arken_TimeInstanceMethodToString},
  {"__gc",             arken_TimeInstanceMethodDestruct},
  {"__lt",             arken_TimeInstanceMethodLessThan},
  {"__le",             arken_TimeInstanceMethodLessEqual},
  {"__eq",             arken_TimeInstanceMethodEqual},
  {"__concat",         arken_TimeInstanceMethodConcat},
  {NULL, NULL}
};

void static
registerTimeInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.chrono.Time.metatable");
  luaL_register(L, NULL, TimeInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_chrono_Time( lua_State *L ) {
    registerTimeInstanceMethods(L);
    registerTimeClassMethods(L);
    return 1;
  }
}
