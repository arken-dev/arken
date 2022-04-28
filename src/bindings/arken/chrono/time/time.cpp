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
  return *static_cast<Time **>(luaL_checkudata(L, i, "arken.chrono.Time.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_chrono_Time_today( lua_State *L ) {
  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr = new Time((Time) Time::today());
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_chrono_Time_now( lua_State *L ) {
  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr = new Time(Time::now());
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_chrono_Time_currentDateTime( lua_State *L ) {
  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr = new Time(Time::currentDateTime());
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_chrono_Time_parse( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  Time time;
  if( lua_gettop(L) == 1 ) { // number of arguments
    time = Time::parse(string);
  } else {
    const char *format = luaL_checkstring(L, 2);
    time = Time::parse(string, format);
  }
  if( time.isValid() ) {
    Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
    *ptr = new Time(time);
    luaL_getmetatable(L, "arken.chrono.Time.metatable");
    lua_setmetatable(L, -2);
  } else {
    lua_pushnil(L);
  }
  return 1;
}

static const luaL_reg arken_chrono_Time[] = {
  {"now",             arken_chrono_Time_now},
  {"currentDateTime", arken_chrono_Time_currentDateTime},
  {"today",           arken_chrono_Time_today},
  {"parse",           arken_chrono_Time_parse},
  {nullptr, nullptr}
};

void static
register_arken_chrono_Time( lua_State *L ) {
  luaL_newmetatable(L,  "arken.chrono.Time");
  luaL_register(L, nullptr, arken_chrono_Time);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_chrono_Time_gc( lua_State *L ) {
  Time *udata = checkTime( L );
  delete udata;
  return 0;
}

static int
arken_chrono_Time_asctime( lua_State *L ) {
  Time * udata  = checkTime( L );
  string result = udata->asctime();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_chrono_Time_strftime( lua_State *L ) {
  Time * udata  = checkTime( L );
  const char * format = luaL_checkstring(L, 2);
  string result = udata->strftime(format);
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_chrono_Time_toString( lua_State *L ) {
  Time * udata  = checkTime( L );
  string result = udata->toString();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_chrono_Time_addYears( lua_State *L ) {
  Time *t   = checkTime( L );
  int years = luaL_checkinteger(L, 2);
  Time other = t->addYears(years);

  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Time_addMonths( lua_State *L ) {
  Time *t   = checkTime( L );
  int months = luaL_checkinteger(L, 2);
  Time other = t->addMonths(months);

  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Time_addDays( lua_State *L ) {
  Time *t   = checkTime( L );
  int days  = luaL_checkinteger(L, 2);
  Time other = t->addDays(days);

  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Time_addHours( lua_State *L ) {
  Time *t   = checkTime( L );
  int hours = luaL_checkinteger(L, 2);
  Time other = t->addHours(hours);

  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Time_addMinutes( lua_State *L ) {
  Time *t     = checkTime( L );
  int minutes = luaL_checkinteger(L, 2);
  Time other  = t->addMinutes(minutes);

  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}


static int
arken_chrono_Time_addSecs( lua_State *L ) {
  Time *t   = checkTime( L );
  int secs  = luaL_checkinteger(L, 2);
  Time other = t->addSecs(secs);

  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr= new Time(other);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Time_beginningOfDay( lua_State *L ) {
  Time *t     = checkTime( L );
  Time result = t->beginningOfDay();

  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr= new Time(result);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Time_endOfDay( lua_State *L ) {
  Time *t     = checkTime( L );
  Time result = t->endOfDay();

  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr= new Time(result);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Time_beginningOfMonth( lua_State *L ) {
  Time *t     = checkTime( L );
  Time result = t->beginningOfMonth();

  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr= new Time(result);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Time_endOfMonth( lua_State *L ) {
  Time *t     = checkTime( L );
  Time result = t->endOfMonth();

  Time **ptr = static_cast<Time **>(lua_newuserdata(L, sizeof(Time*)));
  *ptr= new Time(result);
  luaL_getmetatable(L, "arken.chrono.Time.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_chrono_Time_isLeapYear( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isLeapYear());
  return 1;
}

static int
arken_chrono_Time_isSunday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isSunday());
  return 1;
}

static int
arken_chrono_Time_isMonday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isMonday());
  return 1;
}

static int
arken_chrono_Time_isTuesday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isTuesday());
  return 1;
}

static int
arken_chrono_Time_isWednesday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isWednesday());
  return 1;
}

static int
arken_chrono_Time_isThursday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isThursday());
  return 1;
}

static int
arken_chrono_Time_isFriday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isFriday());
  return 1;
}

static int
arken_chrono_Time_isSaturday( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isSaturday());
  return 1;
}

static int
arken_chrono_Time_isValid( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushboolean(L, t->isValid());
  return 1;
}

static int
arken_chrono_Time_year( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->year());
  return 1;
}

static int
arken_chrono_Time_mon( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->mon());
  return 1;
}

static int
arken_chrono_Time_month( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->month());
  return 1;
}

static int
arken_chrono_Time_day( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->day());
  return 1;
}

static int
arken_chrono_Time_hour( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->hour());
  return 1;
}

static int
arken_chrono_Time_min( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->min());
  return 1;
}


static int
arken_chrono_Time_sec( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->sec());
  return 1;
}

static int
arken_chrono_Time_mday( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->mday());
  return 1;
}

static int
arken_chrono_Time_wday( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->wday());
  return 1;
}

static int
arken_chrono_Time_dayOfWeek( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->dayOfWeek());
  return 1;
}

static int
arken_chrono_Time_yday( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->yday());
  return 1;
}

static int
arken_chrono_Time_dayOfYear( lua_State *L ) {
  Time *t   = checkTime( L );
  lua_pushinteger(L, t->dayOfYear());
  return 1;
}

static int
arken_chrono_Time_daysInMonth( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushinteger(L, t->daysInMonth());
  return 1;
}

static int
arken_chrono_Time_time( lua_State *L ) {
  Time *t = checkTime( L );
  lua_pushnumber(L, t->time());
  return 1;
}

static int
arken_chrono_Time_date( lua_State *L ) {
  Time *t = checkTime( L );

  Date **ptr = static_cast<Date **>(lua_newuserdata(L, sizeof(Date*)));

  *ptr= new Date(t->date());

  luaL_getmetatable(L, "arken.chrono.Date.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_chrono_Time_lessEqual( lua_State *L ) {
  Time *t1 = checkTime( L );
  Time *t2 = *static_cast<Time **>(luaL_checkudata(L, 2, "arken.chrono.Time.metatable"));

  lua_pushboolean(L, (*t1) <= (*t2));
  return 1;
}

static int
arken_chrono_Time_lessThan( lua_State *L ) {
  Time *dt1 = checkTime( L );
  Time *dt2 = *static_cast<Time **>(luaL_checkudata(L, 2, "arken.chrono.Time.metatable"));

  lua_pushboolean(L, (*dt1) < (*dt2));
  return 1;
}

static int
arken_chrono_Time_equal( lua_State *L ) {
  Time *t1 = checkTime( L );
  Time *t2 = *static_cast<Time **>(luaL_checkudata(L, 2, "arken.chrono.Time.metatable"));
  lua_pushboolean(L, (*t1) == (*t2));
  return 1;
}

static int
arken_chrono_Time_concat( lua_State *L ) {
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
luaL_reg arken_chrono_Time_metatable[] = {
  {"asctime",          arken_chrono_Time_asctime},
  {"strftime",         arken_chrono_Time_strftime},
  {"addYears",         arken_chrono_Time_addYears},
  {"addMonths",        arken_chrono_Time_addMonths},
  {"addDays",          arken_chrono_Time_addDays},
  {"addHours",         arken_chrono_Time_addHours},
  {"addMinutes",       arken_chrono_Time_addMinutes},
  {"addSecs",          arken_chrono_Time_addSecs},
  {"beginningOfDay",   arken_chrono_Time_beginningOfDay},
  {"endOfDay",         arken_chrono_Time_endOfDay},
  {"beginningOfMonth", arken_chrono_Time_beginningOfMonth},
  {"endOfMonth",       arken_chrono_Time_endOfMonth},
  {"isLeapYear",       arken_chrono_Time_isLeapYear},
  {"isSunday",         arken_chrono_Time_isSunday},
  {"isMonday",         arken_chrono_Time_isMonday},
  {"isTuesday",        arken_chrono_Time_isTuesday},
  {"isWednesday",      arken_chrono_Time_isWednesday},
  {"isThursday",       arken_chrono_Time_isThursday},
  {"isFriday",         arken_chrono_Time_isFriday},
  {"isSaturday",       arken_chrono_Time_isSaturday},
  {"isValid",          arken_chrono_Time_isValid},
  {"year",             arken_chrono_Time_year},
  {"month",            arken_chrono_Time_month},
  {"mon",              arken_chrono_Time_mon},
  {"day",              arken_chrono_Time_day},
  {"hour",             arken_chrono_Time_hour},
  {"min",              arken_chrono_Time_min},
  {"sec",              arken_chrono_Time_sec},
  {"mday",             arken_chrono_Time_mday},
  {"wday",             arken_chrono_Time_wday},
  {"yday",             arken_chrono_Time_yday},
  {"dayofWeek",        arken_chrono_Time_dayOfWeek},
  {"dayOfYear",        arken_chrono_Time_dayOfYear},
  {"daysInMonth",      arken_chrono_Time_daysInMonth},
  {"time",             arken_chrono_Time_time},
  {"date",             arken_chrono_Time_date},
  {"toString",         arken_chrono_Time_toString},
  {"__tostring",       arken_chrono_Time_toString},
  {"__gc",             arken_chrono_Time_gc},
  {"__lt",             arken_chrono_Time_lessThan},
  {"__le",             arken_chrono_Time_lessEqual},
  {"__eq",             arken_chrono_Time_equal},
  {"__concat",         arken_chrono_Time_concat},
  {nullptr, nullptr}
};

void static
register_arken_chrono_Time_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.chrono.Time.metatable");
  luaL_register(L, nullptr, arken_chrono_Time_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_chrono_Time( lua_State *L ) {
    register_arken_chrono_Time_metatable(L);
    register_arken_chrono_Time(L);
    return 1;
  }
}
