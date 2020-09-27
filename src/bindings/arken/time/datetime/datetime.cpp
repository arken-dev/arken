// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>

using arken::time::DateTime;
using arken::time::Date;

/**
 * checkDateTime
 */

DateTime *
checkDateTime( lua_State *L ) {
  return *(DateTime **) luaL_checkudata(L, 1, "DateTime.metatable");
}

DateTime *
checkDateTime( lua_State *L, int i ) {
  return *(DateTime **) luaL_checkudata(L, i, "DateTime.metatable");
}

/**
 * ClassMethods
 */

// desabilitar esse metodo ! e colocar ele como implementação alternativa de microtime ?
static int
arken_DateTimeClassMethodCurrentMSecsSinceEpoch( lua_State *L )
{
  qint64 mSecs = DateTime::currentMSecsSinceEpoch();
  lua_pushnumber(L, mSecs);
  return 1;
}

static int
arken_DateTimeClassMethodCurrentDateTime( lua_State *L ) {
  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(DateTime::currentDateTime());
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_DateTimeClassMethodFromString( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * format = luaL_checkstring(L, 2);
  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(DateTime::fromString(string, format));
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_DateTimeClassMethodParse( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  DateTime * dt = DateTime::parse(string);
  if( dt == 0 ) {
    lua_pushnil(L);
  } else {
    DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
    *ptr = dt;
    luaL_getmetatable(L, "DateTime.metatable");
    lua_setmetatable(L, -2);
  }
  return 1;
}


static const luaL_reg DateTimeClassMethods[] = {
  {"parse", arken_DateTimeClassMethodParse},
  {"fromString", arken_DateTimeClassMethodFromString},
  {"currentDateTime", arken_DateTimeClassMethodCurrentDateTime},
  {"currentMSecsSinceEpoch", arken_DateTimeClassMethodCurrentMSecsSinceEpoch},
  {NULL, NULL}
};

void static
registerDateTimeClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "DateTime");
  luaL_register(L, NULL, DateTimeClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_DateTimeInstanceMethodDestruct( lua_State *L ) {
  DateTime *udata = checkDateTime( L );
  delete udata;
  return 0;
}

static int
arken_DateTimeInstanceMethodToString( lua_State *L ) {
  DateTime *dt    = checkDateTime( L );
  const char * str;
  if(lua_isstring(L, 2)) {
    str = luaL_checkstring(L, 2);
  } else {
    str = "yyyy-MM-dd hh:mm:ss.zzz";
  }

  lua_pushstring(L, dt->toString(str));
  return 1;
}

static int
arken_DateTimeInstanceMethodConcat( lua_State *L ) {
  DateTime *dt;
  string concat;
  const char * str;
  const char * format = "yyyy-MM-dd hh:mm:ss";
  if(lua_isstring(L, 1)) {
    str = lua_tostring(L, 1);
    dt  = checkDateTime( L, 2 );
    concat = dt->toString(format).prepend(str);
  } else {
    dt  = checkDateTime( L, 1 );
    str = lua_tostring(L, 2);
    concat = dt->toString(format).append(str);
  }
  lua_pushstring(L, concat);
  return 1;
}

static int
arken_DateTimeInstanceMethodEqual( lua_State *L ) {
  DateTime *dt1 = checkDateTime( L );
  DateTime *dt2 = *(DateTime **) luaL_checkudata(L, 2, "DateTime.metatable");

  lua_pushboolean(L, dt1->toString("dd.MM.yyyy HH:mm") == dt2->toString("dd.MM.yyyy HH:mm"));
  return 1;
}

static int
arken_DateTimeInstanceMethodLessThan( lua_State *L ) {
  DateTime *dt1 = checkDateTime( L );
  DateTime *dt2 = *(DateTime **) luaL_checkudata(L, 2, "DateTime.metatable");

  lua_pushboolean(L, dt1->toMSecsSinceEpoch() < dt2->toMSecsSinceEpoch());
  return 1;
}

static int
arken_DateTimeInstanceMethodAddDays( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  qint64 days     = luaL_checkinteger(L, 2);
  DateTime other = dt->addDays(days);

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateTimeInstanceMethodDate( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(dt->date());
  luaL_getmetatable(L, "Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateTimeInstanceMethodAddMSecs( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  qint64 days     = luaL_checkinteger(L, 2);
  DateTime other = dt->addMSecs(days);

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateTimeInstanceMethodAddMonths( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  int     months  = luaL_checkinteger(L, 2);
  DateTime other = dt->addMonths(months);

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateTimeInstanceMethodAddSecs( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  qint64 secs     = luaL_checkinteger(L, 2);
  DateTime other = dt->addSecs(secs);

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateTimeInstanceMethodAddYears( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  int    years    = luaL_checkinteger(L, 2);
  DateTime other = dt->addYears(years);

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateTimeInstanceMethodBeginningOfDay( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->beginningOfDay();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateTimeInstanceMethodEndOfDay( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->endOfDay();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateTimeInstanceMethodBeginningOfMonth( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->beginningOfMonth();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateTimeInstanceMethodEndOfMonth( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->endOfMonth();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateTimeInstanceMethodIsDaylightTime( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushboolean(L, dt->isDaylightTime());
  return 1;
}

static int
arken_DateTimeInstanceMethodIsNull( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushboolean(L, dt->isNull());
  return 1;
}

static int
arken_DateTimeInstanceMethodIsValid( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushboolean(L, dt->isValid());
  return 1;
}

static int
arken_DateTimeInstanceMethodOffsetFromUtc( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushinteger(L, dt->offsetFromUtc());
  return 1;
}

static int
arken_DateTimeInstanceMethodSetMSecsSinceEpoch( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  qint64 msecs    = luaL_checkinteger(L, 2);
  dt->setMSecsSinceEpoch(msecs);

  return 0;
}

static int
arken_DateTimeInstanceMethodSetOffsetFromUtc( lua_State *L ) {
  DateTime *dateTime = checkDateTime( L );
  int offsetSeconds   = luaL_checkinteger(L, 2);
  dateTime->setOffsetFromUtc(offsetSeconds);

  return 0;
}

static int
arken_DateTimeInstanceMethodSetTime_t( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  unsigned int seconds = luaL_checkinteger(L, 2);
  dateTime->setTime_t(seconds);

  return 0;
}

static int
arken_DateTimeInstanceMethodTimeZoneAbbreviation( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  lua_pushstring(L, dateTime->timeZoneAbbreviation().toLocal8Bit().data());

  return 1;
}

static int
arken_DateTimeInstanceMethodToMSecsSinceEpoch( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  qint64 msecs         = dateTime->toMSecsSinceEpoch();
  lua_pushnumber(L, msecs);

  return 1;
}

static int
arken_DateTimeInstanceMethodToOffsetFromUtc( lua_State *L ) {
  DateTime *dateTime = checkDateTime( L );
  int offsetSeconds   = luaL_checkinteger(L , 2);
  DateTime other     = dateTime->toOffsetFromUtc(offsetSeconds);
  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_DateTimeInstanceMethodToTime_t( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  uint time            = dateTime->toTime_t();
  lua_pushnumber(L, time);

  return 1;
}

static int
arken_DateTimeInstanceMethodToUTC( lua_State *L ) {
  DateTime *dateTime = checkDateTime( L );
  DateTime other     = dateTime->toUTC();
  DateTime **ptr     = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const
luaL_reg DateTimeInstanceMethods[] = {
  {"toUTC", arken_DateTimeInstanceMethodToUTC},
  {"toTime_t", arken_DateTimeInstanceMethodToTime_t},
  {"toOffsetFromUtc", arken_DateTimeInstanceMethodToOffsetFromUtc},
  {"toMSecsSinceEpoch", arken_DateTimeInstanceMethodToMSecsSinceEpoch},
  {"timeZoneAbbreviation", arken_DateTimeInstanceMethodTimeZoneAbbreviation},
  {"setTime_t", arken_DateTimeInstanceMethodSetTime_t},
  {"setOffsetFromUtc", arken_DateTimeInstanceMethodSetOffsetFromUtc},
  {"setMSecsSinceEpoch", arken_DateTimeInstanceMethodSetMSecsSinceEpoch},
  {"offsetFromUtc", arken_DateTimeInstanceMethodOffsetFromUtc},
  {"isValid", arken_DateTimeInstanceMethodIsValid},
  {"isNull", arken_DateTimeInstanceMethodIsNull},
  {"isDaylightTime", arken_DateTimeInstanceMethodIsDaylightTime},
  {"addYears", arken_DateTimeInstanceMethodAddYears},
  {"addSecs", arken_DateTimeInstanceMethodAddSecs},
  {"addMonths", arken_DateTimeInstanceMethodAddMonths},
  {"addMSecs", arken_DateTimeInstanceMethodAddMSecs},
  {"addDays", arken_DateTimeInstanceMethodAddDays},
  {"date", arken_DateTimeInstanceMethodDate},
  {"beginningOfDay", arken_DateTimeInstanceMethodBeginningOfDay},
  {"endOfDay", arken_DateTimeInstanceMethodEndOfDay},
  {"beginningOfMonth", arken_DateTimeInstanceMethodBeginningOfMonth},
  {"endOfMonth", arken_DateTimeInstanceMethodEndOfMonth},
  {"toString", arken_DateTimeInstanceMethodToString},
  {"__lt", arken_DateTimeInstanceMethodLessThan},
  {"__eq", arken_DateTimeInstanceMethodEqual},
  {"__tostring", arken_DateTimeInstanceMethodToString},
  {"__concat", arken_DateTimeInstanceMethodConcat},
  {"__gc", arken_DateTimeInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerDateTimeInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "DateTime.metatable");
  luaL_register(L, NULL, DateTimeInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_time_DateTime( lua_State *L ) {
    registerDateTimeInstanceMethods(L);
    registerDateTimeClassMethods(L);
    return 1;
  }
}
