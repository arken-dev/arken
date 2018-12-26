// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>

using charon::time::DateTime;
using charon::time::Date;

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
charon_DateTimeClassMethodCurrentMSecsSinceEpoch( lua_State *L )
{
  qint64 mSecs = DateTime::currentMSecsSinceEpoch();
  lua_pushnumber(L, mSecs);
  return 1;
}

static int
charon_DateTimeClassMethodCurrentDateTime( lua_State *L ) {
  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(DateTime::currentDateTime());
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_DateTimeClassMethodFromString( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * format = luaL_checkstring(L, 2);
  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(DateTime::fromString(string, format));
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_DateTimeClassMethodParse( lua_State *L ) {
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
  {"parse", charon_DateTimeClassMethodParse},
  {"fromString", charon_DateTimeClassMethodFromString},
  {"currentDateTime", charon_DateTimeClassMethodCurrentDateTime},
  {"currentMSecsSinceEpoch", charon_DateTimeClassMethodCurrentMSecsSinceEpoch},
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
charon_DateTimeInstanceMethodDestruct( lua_State *L ) {
  DateTime *udata = checkDateTime( L );
  delete udata;
  return 0;
}

static int
charon_DateTimeInstanceMethodToString( lua_State *L ) {
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
charon_DateTimeInstanceMethodConcat( lua_State *L ) {
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
charon_DateTimeInstanceMethodEqual( lua_State *L ) {
  DateTime *dt1 = checkDateTime( L );
  DateTime *dt2 = *(DateTime **) luaL_checkudata(L, 2, "DateTime.metatable");

  lua_pushboolean(L, dt1->toString("dd.MM.yyyy HH:mm") == dt2->toString("dd.MM.yyyy HH:mm"));
  return 1;
}

static int
charon_DateTimeInstanceMethodLessThan( lua_State *L ) {
  DateTime *dt1 = checkDateTime( L );
  DateTime *dt2 = *(DateTime **) luaL_checkudata(L, 2, "DateTime.metatable");

  lua_pushboolean(L, dt1->toMSecsSinceEpoch() < dt2->toMSecsSinceEpoch());
  return 1;
}

static int
charon_DateTimeInstanceMethodAddDays( lua_State *L ) {
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
charon_DateTimeInstanceMethodDate( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );

  Date **ptr = (Date **)lua_newuserdata(L, sizeof(Date*));
  *ptr= new Date(dt->date());
  luaL_getmetatable(L, "Date.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
charon_DateTimeInstanceMethodAddMSecs( lua_State *L ) {
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
charon_DateTimeInstanceMethodAddMonths( lua_State *L ) {
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
charon_DateTimeInstanceMethodAddSecs( lua_State *L ) {
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
charon_DateTimeInstanceMethodAddYears( lua_State *L ) {
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
charon_DateTimeInstanceMethodBeginningOfDay( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->beginningOfDay();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
charon_DateTimeInstanceMethodEndOfDay( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->endOfDay();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
charon_DateTimeInstanceMethodBeginningOfMonth( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->beginningOfMonth();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
charon_DateTimeInstanceMethodEndOfMonth( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  DateTime other = dt->endOfMonth();

  DateTime **ptr = (DateTime **)lua_newuserdata(L, sizeof(DateTime*));
  *ptr= new DateTime(other);
  luaL_getmetatable(L, "DateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
charon_DateTimeInstanceMethodIsDaylightTime( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushboolean(L, dt->isDaylightTime());
  return 1;
}

static int
charon_DateTimeInstanceMethodIsNull( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushboolean(L, dt->isNull());
  return 1;
}

static int
charon_DateTimeInstanceMethodIsValid( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushboolean(L, dt->isValid());
  return 1;
}

static int
charon_DateTimeInstanceMethodOffsetFromUtc( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  lua_pushinteger(L, dt->offsetFromUtc());
  return 1;
}

static int
charon_DateTimeInstanceMethodSetMSecsSinceEpoch( lua_State *L ) {
  DateTime *dt   = checkDateTime( L );
  qint64 msecs    = luaL_checkinteger(L, 2);
  dt->setMSecsSinceEpoch(msecs);

  return 0;
}

static int
charon_DateTimeInstanceMethodSetOffsetFromUtc( lua_State *L ) {
  DateTime *dateTime = checkDateTime( L );
  int offsetSeconds   = luaL_checkinteger(L, 2);
  dateTime->setOffsetFromUtc(offsetSeconds);

  return 0;
}

static int
charon_DateTimeInstanceMethodSetTime_t( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  unsigned int seconds = luaL_checkinteger(L, 2);
  dateTime->setTime_t(seconds);

  return 0;
}

static int
charon_DateTimeInstanceMethodTimeZoneAbbreviation( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  lua_pushstring(L, dateTime->timeZoneAbbreviation().toLocal8Bit().data());

  return 1;
}

static int
charon_DateTimeInstanceMethodToMSecsSinceEpoch( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  qint64 msecs         = dateTime->toMSecsSinceEpoch();
  lua_pushnumber(L, msecs);

  return 1;
}

static int
charon_DateTimeInstanceMethodToOffsetFromUtc( lua_State *L ) {
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
charon_DateTimeInstanceMethodToTime_t( lua_State *L ) {
  DateTime *dateTime  = checkDateTime( L );
  uint time            = dateTime->toTime_t();
  lua_pushnumber(L, time);

  return 1;
}

static int
charon_DateTimeInstanceMethodToUTC( lua_State *L ) {
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
  {"toUTC", charon_DateTimeInstanceMethodToUTC},
  {"toTime_t", charon_DateTimeInstanceMethodToTime_t},
  {"toOffsetFromUtc", charon_DateTimeInstanceMethodToOffsetFromUtc},
  {"toMSecsSinceEpoch", charon_DateTimeInstanceMethodToMSecsSinceEpoch},
  {"timeZoneAbbreviation", charon_DateTimeInstanceMethodTimeZoneAbbreviation},
  {"setTime_t", charon_DateTimeInstanceMethodSetTime_t},
  {"setOffsetFromUtc", charon_DateTimeInstanceMethodSetOffsetFromUtc},
  {"setMSecsSinceEpoch", charon_DateTimeInstanceMethodSetMSecsSinceEpoch},
  {"offsetFromUtc", charon_DateTimeInstanceMethodOffsetFromUtc},
  {"isValid", charon_DateTimeInstanceMethodIsValid},
  {"isNull", charon_DateTimeInstanceMethodIsNull},
  {"isDaylightTime", charon_DateTimeInstanceMethodIsDaylightTime},
  {"addYears", charon_DateTimeInstanceMethodAddYears},
  {"addSecs", charon_DateTimeInstanceMethodAddSecs},
  {"addMonths", charon_DateTimeInstanceMethodAddMonths},
  {"addMSecs", charon_DateTimeInstanceMethodAddMSecs},
  {"addDays", charon_DateTimeInstanceMethodAddDays},
  {"date", charon_DateTimeInstanceMethodDate},
  {"beginningOfDay", charon_DateTimeInstanceMethodBeginningOfDay},
  {"endOfDay", charon_DateTimeInstanceMethodEndOfDay},
  {"beginningOfMonth", charon_DateTimeInstanceMethodBeginningOfMonth},
  {"endOfMonth", charon_DateTimeInstanceMethodEndOfMonth},
  {"toString", charon_DateTimeInstanceMethodToString},
  {"__lt", charon_DateTimeInstanceMethodLessThan},
  {"__eq", charon_DateTimeInstanceMethodEqual},
  {"__tostring", charon_DateTimeInstanceMethodToString},
  {"__concat", charon_DateTimeInstanceMethodConcat},
  {"__gc", charon_DateTimeInstanceMethodDestruct},
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
