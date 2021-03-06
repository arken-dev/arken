// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>

using arken::time::Date;

/**
 * checkDate
 */

Date *
checkDate( lua_State *L ) {
  return *(Date **) luaL_checkudata(L, 1, "Date.metatable");
}

Date *
checkDate( lua_State *L, int i ) {
  return *(Date **) luaL_checkudata(L, i, "Date.metatable");
}

/**
 * ClassMethods
 */

static int
arken_DateClassMethodCurrentDate( lua_State *L ) {
  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(Date::currentDate());
  luaL_getmetatable(L, "Date.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_DateClassMethodFromString( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * format = luaL_checkstring(L, 2);
  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(Date::fromString(string, format));
  luaL_getmetatable(L, "Date.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_DateClassMethodParse( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  Date * dt = Date::parse(string);
  if( dt == 0 ) {
    lua_pushnil(L);
  } else {
    Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
    *ptr = dt;
    luaL_getmetatable(L, "Date.metatable");
    lua_setmetatable(L, -2);
  }
  return 1;
}


static const luaL_reg DateClassMethods[] = {
  {"parse",       arken_DateClassMethodParse},
  {"fromString",  arken_DateClassMethodFromString},
  {"currentDate", arken_DateClassMethodCurrentDate},
  {NULL, NULL}
};

void static
registerDateClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "Date");
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
arken_DateInstanceMethodToString( lua_State *L ) {
  Date *dt    = checkDate( L );
  const char * str;
  if(lua_isstring(L, 2)) {
    str = luaL_checkstring(L, 2);
  } else {
    str = "yyyy-MM-dd";
  }

  lua_pushstring(L, dt->toString(str));
  return 1;
}

static int
arken_DateInstanceMethodConcat( lua_State *L ) {
  Date *dt;
  string concat;
  const char * str;
  const char * format = "yyyy-MM-dd";
  if(lua_isstring(L, 1)) {
    str = lua_tostring(L, 1);
    dt  = checkDate( L, 2 );
    concat = dt->toString(format).prepend(str);
  } else {
    dt  = checkDate( L, 1 );
    str = lua_tostring(L, 2);
    concat = dt->toString(format).append(str);
  }
  lua_pushstring(L, concat);
  return 1;
}

static int
arken_DateInstanceMethodEqual( lua_State *L ) {
  Date *dt1 = checkDate( L );
  Date *dt2 = *(Date **) luaL_checkudata(L, 2, "Date.metatable");

  lua_pushboolean(L, dt1 == dt2);
  return 1;
}

static int
arken_DateInstanceMethodLessThan( lua_State *L ) {
  Date *dt1 = checkDate( L );
  Date *dt2 = *(Date **) luaL_checkudata(L, 2, "Date.metatable");

  lua_pushboolean(L, dt1 < dt2);
  return 1;
}

static int
arken_DateInstanceMethodAddDays( lua_State *L ) {
  Date *dt   = checkDate( L );
  qint64 days     = luaL_checkinteger(L, 2);
  Date other = dt->addDays(days);

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(other);
  luaL_getmetatable(L, "Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateInstanceMethodAddMonths( lua_State *L ) {
  Date *dt   = checkDate( L );
  int     months  = luaL_checkinteger(L, 2);
  Date other = dt->addMonths(months);

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(other);
  luaL_getmetatable(L, "Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateInstanceMethodAddYears( lua_State *L ) {
  Date *dt   = checkDate( L );
  int    years    = luaL_checkinteger(L, 2);
  Date other = dt->addYears(years);

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(other);
  luaL_getmetatable(L, "Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateInstanceMethodBeginningOfMonth( lua_State *L ) {
  Date *dt   = checkDate( L );
  Date other = dt->beginningOfMonth();

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(other);
  luaL_getmetatable(L, "Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateInstanceMethodEndOfMonth( lua_State *L ) {
  Date *dt   = checkDate( L );
  Date other = dt->endOfMonth();

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(other);
  luaL_getmetatable(L, "Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateInstanceMethodDayOfWeek( lua_State *L ) {
  Date *dt = checkDate( L );
  lua_pushinteger(L, dt->dayOfWeek());

  return 1;
}

static int
arken_DateInstanceMethodIsNull( lua_State *L ) {
  Date *dt   = checkDate( L );
  lua_pushboolean(L, dt->isNull());
  return 1;
}

static int
arken_DateInstanceMethodIsValid( lua_State *L ) {
  Date *dt   = checkDate( L );
  lua_pushboolean(L, dt->isValid());
  return 1;
}

static int
arken_DateInstanceMethodDay( lua_State *L ) {
  Date *dt = checkDate( L );
  lua_pushinteger(L, dt->day());
  return 1;
}

static int
arken_DateInstanceMethodMonth( lua_State *L ) {
  Date *dt   = checkDate( L );
  lua_pushinteger(L, dt->month());
  return 1;
}

static int
arken_DateInstanceMethodYear( lua_State *L ) {
  Date *dt   = checkDate( L );
  lua_pushinteger(L, dt->year());
  return 1;
}

static const
luaL_reg DateInstanceMethods[] = {
  {"isValid", arken_DateInstanceMethodIsValid},
  {"isNull", arken_DateInstanceMethodIsNull},
  {"addYears", arken_DateInstanceMethodAddYears},
  {"addMonths", arken_DateInstanceMethodAddMonths},
  {"addDays", arken_DateInstanceMethodAddDays},
  {"beginningOfMonth", arken_DateInstanceMethodBeginningOfMonth},
  {"day", arken_DateInstanceMethodDay},
  {"month", arken_DateInstanceMethodMonth},
  {"year", arken_DateInstanceMethodYear},
  {"dayOfWeek", arken_DateInstanceMethodDayOfWeek},
  {"endOfMonth", arken_DateInstanceMethodEndOfMonth},
  {"toString", arken_DateInstanceMethodToString},
  {"__lt", arken_DateInstanceMethodLessThan},
  {"__eq", arken_DateInstanceMethodEqual},
  {"__tostring", arken_DateInstanceMethodToString},
  {"__concat", arken_DateInstanceMethodConcat},
  {"__gc", arken_DateInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerDateInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "Date.metatable");
  luaL_register(L, NULL, DateInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_time_Date( lua_State *L ) {
    registerDateInstanceMethods(L);
    registerDateClassMethods(L);
    return 1;
  }
}
