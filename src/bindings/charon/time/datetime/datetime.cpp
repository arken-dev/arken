// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>

using charon::time::DateTime;

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

static int
lua_DateTimeClassMethodCurrentMSecsSinceEpoch( lua_State *L )
{
  qint64 mSecs = DateTime::currentMSecsSinceEpoch();
  lua_pushnumber(L, mSecs);
  return 1;
}

static int
lua_DateTimeClassMethodCurrentDateTime( lua_State *L ) {
  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(DateTime::currentDateTime());
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
lua_DateTimeClassMethodFromString( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * format = luaL_checkstring(L, 2);
  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(DateTime::fromString(string, format));
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
lua_DateTimeClassMethodParse( lua_State *L ) {
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
  {"parse", lua_DateTimeClassMethodParse},
  {"fromString", lua_DateTimeClassMethodFromString},
  {"currentDateTime", lua_DateTimeClassMethodCurrentDateTime},
  {"currentMSecsSinceEpoch", lua_DateTimeClassMethodCurrentMSecsSinceEpoch},
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
lua_DateTimeInstanceMethodDestruct( lua_State *L ) {
  DateTime *udata = checkDateTime( L );
  delete udata;
  return 0;
}

static int
lua_DateTimeInstanceMethodToString( lua_State *L ) {
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
lua_DateTimeInstanceMethodConcat( lua_State *L ) {
  DateTime *dt;
  QString concat;
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
  lua_pushstring(L, concat.toLocal8Bit().data());
  return 1;
}

static int
lua_DateTimeInstanceMethodEqual( lua_State *L ) {
  DateTime *dt1 = checkDateTime( L );
  DateTime *dt2 = *(DateTime **) luaL_checkudata(L, 2, "DateTime.metatable");

  lua_pushboolean(L, dt1->toMSecsSinceEpoch() == dt2->toMSecsSinceEpoch());
  return 1;
}

static int
lua_DateTimeInstanceMethodLessThan( lua_State *L ) {
  DateTime *dt1 = checkDateTime( L );
  DateTime *dt2 = *(DateTime **) luaL_checkudata(L, 2, "DateTime.metatable");

  lua_pushboolean(L, dt1->toMSecsSinceEpoch() < dt2->toMSecsSinceEpoch());
  return 1;
}

static int
lua_DateTimeInstanceMethodAddDays( lua_State *L ) {
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
lua_DateTimeInstanceMethodAddMSecs( lua_State *L ) {
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
lua_DateTimeInstanceMethodAddMonths( lua_State *L ) {
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
lua_DateTimeInstanceMethodAddSecs( lua_State *L ) {
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
lua_DateTimeInstanceMethodAddYears( lua_State *L ) {
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
lua_DateTimeInstanceMethodBeginningOfDay( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->beginningOfDay();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_DateTimeInstanceMethodEndOfDay( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->endOfDay();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_DateTimeInstanceMethodBeginningOfMonth( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->beginningOfMonth();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_DateTimeInstanceMethodEndOfMonth( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->endOfMonth();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_DateTimeInstanceMethodIsDaylightTime( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushboolean(L, dt->isDaylightTime());
  return 1;
}

static int
lua_DateTimeInstanceMethodIsNull( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushboolean(L, dt->isNull());
  return 1;
}

static int
lua_DateTimeInstanceMethodIsValid( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushboolean(L, dt->isValid());
  return 1;
}

static int
lua_DateTimeInstanceMethodOffsetFromUtc( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushinteger(L, dt->offsetFromUtc());
  return 1;
}

static int
lua_DateTimeInstanceMethodSetMSecsSinceEpoch( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  qint64 msecs    = luaL_checkinteger(L, 2);
  dt->setMSecsSinceEpoch(msecs);

  return 0;
}

static int
lua_DateTimeInstanceMethodSetOffsetFromUtc( lua_State *L ) {
  DateTime *dateTime = checkDateTime( L );
  int offsetSeconds   = luaL_checkinteger(L, 2);
  dateTime->setOffsetFromUtc(offsetSeconds);

  return 0;
}

static int
lua_DateTimeInstanceMethodSetTime_t( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  unsigned int seconds = luaL_checkinteger(L, 2);
  dateTime->setTime_t(seconds);

  return 0;
}

static int
lua_DateTimeInstanceMethodTimeZoneAbbreviation( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  lua_pushstring(L, dateTime->timeZoneAbbreviation().toLocal8Bit().data());

  return 1;
}

static int
lua_DateTimeInstanceMethodToMSecsSinceEpoch( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  qint64 msecs         = dateTime->toMSecsSinceEpoch();
  lua_pushnumber(L, msecs);

  return 1;
}

static int
lua_DateTimeInstanceMethodToOffsetFromUtc( lua_State *L ) {
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
lua_DateTimeInstanceMethodToTime_t( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  uint time            = dateTime->toTime_t();
  lua_pushnumber(L, time);

  return 1;
}

static int
lua_DateTimeInstanceMethodToUTC( lua_State *L ) {
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
  {"toUTC", lua_DateTimeInstanceMethodToUTC},
  {"toTime_t", lua_DateTimeInstanceMethodToTime_t},
  {"toOffsetFromUtc", lua_DateTimeInstanceMethodToOffsetFromUtc},
  {"toMSecsSinceEpoch", lua_DateTimeInstanceMethodToMSecsSinceEpoch},
  {"timeZoneAbbreviation", lua_DateTimeInstanceMethodTimeZoneAbbreviation},
  {"setTime_t", lua_DateTimeInstanceMethodSetTime_t},
  {"setOffsetFromUtc", lua_DateTimeInstanceMethodSetOffsetFromUtc},
  {"setMSecsSinceEpoch", lua_DateTimeInstanceMethodSetMSecsSinceEpoch},
  {"offsetFromUtc", lua_DateTimeInstanceMethodOffsetFromUtc},
  {"isValid", lua_DateTimeInstanceMethodIsValid},
  {"isNull", lua_DateTimeInstanceMethodIsNull},
  {"isDaylightTime", lua_DateTimeInstanceMethodIsDaylightTime},
  {"addYears", lua_DateTimeInstanceMethodAddYears},
  {"addSecs", lua_DateTimeInstanceMethodAddSecs},
  {"addMonths", lua_DateTimeInstanceMethodAddMonths},
  {"addMSecs", lua_DateTimeInstanceMethodAddMSecs},
  {"addDays", lua_DateTimeInstanceMethodAddDays},
  {"beginningOfDay", lua_DateTimeInstanceMethodBeginningOfDay},
  {"endOfDay", lua_DateTimeInstanceMethodEndOfDay},
  {"beginningOfMonth", lua_DateTimeInstanceMethodBeginningOfMonth},
  {"endOfMonth", lua_DateTimeInstanceMethodEndOfMonth},
  {"toString", lua_DateTimeInstanceMethodToString},
  {"__lt", lua_DateTimeInstanceMethodLessThan},
  {"__eq", lua_DateTimeInstanceMethodEqual},
  {"__tostring", lua_DateTimeInstanceMethodToString},
  {"__concat", lua_DateTimeInstanceMethodConcat},
  {"__gc", lua_DateTimeInstanceMethodDestruct},
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
  luaopen_charon_time_DateTime( lua_State *L ) {
    registerDateTimeInstanceMethods(L);
    registerDateTimeClassMethods(L);
    return 1;
  }
}
