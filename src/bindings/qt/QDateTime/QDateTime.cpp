#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>

/**
 * checkQDateTime
 */

QDateTime *
checkQDateTime( lua_State *L ) {
  return *(QDateTime **) luaL_checkudata(L, 1, "QDateTime.metatable");
}

/**
 * ClassMethods
 */

static int
lua_QDateTimeClassMethodCurrentMSecsSinceEpoch( lua_State *L )
{
  qint64 mSecs  = QDateTime::currentMSecsSinceEpoch();
  lua_pushnumber(L, mSecs);
  return 1;
}

static int
lua_QDateTimeClassMethodCurrentDateTime( lua_State *L ) {
  QDateTime **ptr = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
  *ptr= new QDateTime(QDateTime::currentDateTime());
  luaL_getmetatable(L, "QDateTime.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
lua_QDateTimeClassMethodFromString( lua_State *L ) {
  const char * string = luaL_checkstring(L, 1);
  const char * format = luaL_checkstring(L, 2);
  QDateTime **ptr = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
  *ptr= new QDateTime(QDateTime::fromString(QString(string), QString(format)));
  luaL_getmetatable(L, "QDateTime.metatable");
  lua_setmetatable(L, -2);
  return 1;
}


static const luaL_reg QDateTimeClassMethods[] = {
  {"fromString", lua_QDateTimeClassMethodFromString},
  {"currentDateTime", lua_QDateTimeClassMethodCurrentDateTime},
  {"currentMSecsSinceEpoch", lua_QDateTimeClassMethodCurrentMSecsSinceEpoch},
  {NULL, NULL}
};

void static
registerQDateTimeClassMethods( lua_State *L ) {
  luaL_register(L, "QDateTime", QDateTimeClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_QDateTimeInstanceMethodDestruct( lua_State *L ) {
  QDateTime *udata = checkQDateTime( L );
  delete udata;
  return 0;
}

static int
lua_QDateTimeInstanceMethodToString( lua_State *L ) {
  QDateTime *dt    = checkQDateTime( L );
  const char * str;
  if(lua_isstring(L, 2)) {
    str = luaL_checkstring(L, 2);
  } else {
    str = "yyyy/MM/dd hh:mm:ss.zzz";
  }

  lua_pushstring(L, dt->toString(str).toLocal8Bit().data());
  return 1;
}

static int
lua_QDateTimeInstanceMethodEqual( lua_State *L ) {
  QDateTime *dt1 = checkQDateTime( L );
  QDateTime *dt2 = *(QDateTime **) luaL_checkudata(L, 2, "QDateTime.metatable");

  lua_pushboolean(L, dt1->toMSecsSinceEpoch() == dt2->toMSecsSinceEpoch());
  return 1;
}

static int
lua_QDateTimeInstanceMethodLessThan( lua_State *L ) {
  QDateTime *dt1 = checkQDateTime( L );
  QDateTime *dt2 = *(QDateTime **) luaL_checkudata(L, 2, "QDateTime.metatable");

  lua_pushboolean(L, dt1->toMSecsSinceEpoch() < dt2->toMSecsSinceEpoch());
  return 1;
}

static int
lua_QDateTimeInstanceMethodAddDays( lua_State *L ) {
  QDateTime *dt   = checkQDateTime( L );
  qint64 days     = luaL_checkinteger(L, 2);
  QDateTime other = dt->addDays(days);

  QDateTime **ptr = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
  *ptr= new QDateTime(other);
  luaL_getmetatable(L, "QDateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_QDateTimeInstanceMethodAddMSecs( lua_State *L ) {
  QDateTime *dt   = checkQDateTime( L );
  qint64 days     = luaL_checkinteger(L, 2);
  QDateTime other = dt->addMSecs(days);

  QDateTime **ptr = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
  *ptr= new QDateTime(other);
  luaL_getmetatable(L, "QDateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_QDateTimeInstanceMethodAddMonths( lua_State *L ) {
  QDateTime *dt   = checkQDateTime( L );
  int     months  = luaL_checkinteger(L, 2);
  QDateTime other = dt->addMonths(months);

  QDateTime **ptr = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
  *ptr= new QDateTime(other);
  luaL_getmetatable(L, "QDateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_QDateTimeInstanceMethodAddSecs( lua_State *L ) {
  QDateTime *dt   = checkQDateTime( L );
  qint64 secs     = luaL_checkinteger(L, 2);
  QDateTime other = dt->addSecs(secs);

  QDateTime **ptr = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
  *ptr= new QDateTime(other);
  luaL_getmetatable(L, "QDateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_QDateTimeInstanceMethodAddYears( lua_State *L ) {
  QDateTime *dt   = checkQDateTime( L );
  int    years    = luaL_checkinteger(L, 2);
  QDateTime other = dt->addYears(years);

  QDateTime **ptr = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
  *ptr= new QDateTime(other);
  luaL_getmetatable(L, "QDateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_QDateTimeInstanceMethodIsDaylightTime( lua_State *L ) {
  QDateTime *dt   = checkQDateTime( L );
  lua_pushboolean(L, dt->isDaylightTime());
  return 1;
}

static int
lua_QDateTimeInstanceMethodIsNull( lua_State *L ) {
  QDateTime *dt   = checkQDateTime( L );
  lua_pushboolean(L, dt->isNull());
  return 1;
}

static int
lua_QDateTimeInstanceMethodIsValid( lua_State *L ) {
  QDateTime *dt   = checkQDateTime( L );
  lua_pushboolean(L, dt->isValid());
  return 1;
}

static int
lua_QDateTimeInstanceMethodOffsetFromUtc( lua_State *L ) {
  QDateTime *dt   = checkQDateTime( L );
  lua_pushinteger(L, dt->offsetFromUtc());
  return 1;
}

static int
lua_QDateTimeInstanceMethodSetMSecsSinceEpoch( lua_State *L ) {
  QDateTime *dt   = checkQDateTime( L );
  qint64 msecs    = luaL_checkinteger(L, 2);
  dt->setMSecsSinceEpoch(msecs);

  return 0;
}

static int
lua_QDateTimeInstanceMethodSetOffsetFromUtc( lua_State *L ) {
  QDateTime *dateTime = checkQDateTime( L );
  int offsetSeconds   = luaL_checkinteger(L, 2);
  dateTime->setOffsetFromUtc(offsetSeconds);

  return 0;
}

static int
lua_QDateTimeInstanceMethodSetTime_t( lua_State *L ) {
  QDateTime *dateTime  = checkQDateTime( L );
  unsigned int seconds = luaL_checkinteger(L, 2);
  dateTime->setTime_t(seconds);

  return 0;
}

static int
lua_QDateTimeInstanceMethodTimeZoneAbbreviation( lua_State *L ) {
  QDateTime *dateTime  = checkQDateTime( L );
  lua_pushstring(L, dateTime->timeZoneAbbreviation().toLocal8Bit().data());

  return 1;
}

static int
lua_QDateTimeInstanceMethodToMSecsSinceEpoch( lua_State *L ) {
  QDateTime *dateTime  = checkQDateTime( L );
  qint64 msecs         = dateTime->toMSecsSinceEpoch();
  lua_pushnumber(L, msecs);

  return 1;
}

static int
lua_QDateTimeInstanceMethodToOffsetFromUtc( lua_State *L ) {
  QDateTime *dateTime = checkQDateTime( L );
  int offsetSeconds   = luaL_checkinteger(L , 2);
  QDateTime other     = dateTime->toOffsetFromUtc(offsetSeconds);
  QDateTime **ptr = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
  *ptr= new QDateTime(other);
  luaL_getmetatable(L, "QDateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_QDateTimeInstanceMethodToTime_t( lua_State *L ) {
  QDateTime *dateTime  = checkQDateTime( L );
  uint time            = dateTime->toTime_t();
  lua_pushnumber(L, time);

  return 1;
}

static int
lua_QDateTimeInstanceMethodToUTC( lua_State *L ) {
  QDateTime *dateTime = checkQDateTime( L );
  QDateTime other     = dateTime->toUTC();
  QDateTime **ptr     = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
  *ptr= new QDateTime(other);
  luaL_getmetatable(L, "QDateTime.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const
luaL_reg QDateTimeInstanceMethods[] = {
  {"toUTC", lua_QDateTimeInstanceMethodToUTC},
  {"toTime_t", lua_QDateTimeInstanceMethodToTime_t},
  {"toOffsetFromUtc", lua_QDateTimeInstanceMethodToOffsetFromUtc},
  {"toMSecsSinceEpoch", lua_QDateTimeInstanceMethodToMSecsSinceEpoch},
  {"timeZoneAbbreviation", lua_QDateTimeInstanceMethodTimeZoneAbbreviation},
  {"setTime_t", lua_QDateTimeInstanceMethodSetTime_t},
  {"setOffsetFromUtc", lua_QDateTimeInstanceMethodSetOffsetFromUtc},
  {"setMSecsSinceEpoch", lua_QDateTimeInstanceMethodSetMSecsSinceEpoch},
  {"offsetFromUtc", lua_QDateTimeInstanceMethodOffsetFromUtc},
  {"isValid", lua_QDateTimeInstanceMethodIsValid},
  {"isNull", lua_QDateTimeInstanceMethodIsNull},
  {"isDaylightTime", lua_QDateTimeInstanceMethodIsDaylightTime},
  {"addYears", lua_QDateTimeInstanceMethodAddYears},
  {"addSecs", lua_QDateTimeInstanceMethodAddSecs},
  {"addMonths", lua_QDateTimeInstanceMethodAddMonths},
  {"addMSecs", lua_QDateTimeInstanceMethodAddMSecs},
  {"addDays", lua_QDateTimeInstanceMethodAddDays},
  {"toString", lua_QDateTimeInstanceMethodToString},
  {"__lt", lua_QDateTimeInstanceMethodLessThan},
  {"__eq", lua_QDateTimeInstanceMethodEqual},
  {"__tostring", lua_QDateTimeInstanceMethodToString},
  {"__gc", lua_QDateTimeInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQDateTimeInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QDateTime.metatable");
  luaL_register(L, NULL, QDateTimeInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QDateTime( lua_State *L ) {
    registerQDateTimeInstanceMethods(L);
    registerQDateTimeClassMethods(L);
    return 1;
  }
}
