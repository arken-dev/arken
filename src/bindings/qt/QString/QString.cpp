// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>

/**
 * checkQString
 */

QString *
checkQString( lua_State *L ) {
  return *(QString **) luaL_checkudata(L, 1, "QString.metatable");
}

/**
 * ClassMethods
 */

static int
lua_QStringClassMethodNew( lua_State *L ) {
  const char *str = (char *) lua_tostring(L, 1);
  QString **ptr = (QString **)lua_newuserdata(L, sizeof(QString*));
  *ptr= new QString(QString(str));
  luaL_getmetatable(L, "QString.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg QStringClassMethods[] = {
  {"new", lua_QStringClassMethodNew},
  {NULL, NULL}
};

void static
registerQStringClassMethods( lua_State *L ) {
  luaL_register(L, "QString", QStringClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_QStringInstanceMethodDestruct( lua_State *L ) {
  QString *udata = checkQString( L );
  delete udata;
  return 0;
}

static int
lua_QStringInstanceMethodToString( lua_State *L ) {
  QString *udata = checkQString( L );
  lua_pushstring(L, udata->toLocal8Bit());
  return 1;
}

static int
lua_QStringInstanceMethodConcat( lua_State *L ) {
  QString *udata  = checkQString( L );
  const char *str = lua_tostring(L, 2);
  udata->append(str);
  lua_pushstring(L, udata->toLocal8Bit());
  return 1;
}

static int
lua_QStringInstanceMethodReplace( lua_State *L ) {
  QString    * udata  = checkQString( L );
  const char * before = lua_tostring(L, 2);
  const char * after  = lua_tostring(L, 3);
  QString result = udata->replace(QString(before), QString(after));
  lua_pushstring(L, result.toLocal8Bit());
  return 1;
}

static int
lua_QStringInstanceMethodSplit( lua_State *L ) {
  QString *udata  = checkQString( L );
  if(lua_isuserdata(L, 2)) {
    QRegExp *regex = *(QRegExp **) luaL_checkudata(L, 2, "QRegExp.metatable");
    QStringList **ptr = (QStringList **)lua_newuserdata(L, sizeof(QStringList*));
    *ptr = new QStringList(udata->split(*regex));
    luaL_getmetatable(L, "QStringList.metatable");
    lua_setmetatable(L, -2);
  } else {
    const char *str = lua_tostring(L, 2);
    QString sep(str);
    QStringList **ptr = (QStringList **)lua_newuserdata(L, sizeof(QStringList*));
    *ptr = new QStringList(udata->split(sep));
    luaL_getmetatable(L, "QStringList.metatable");
    lua_setmetatable(L, -2);
  }
  return 1;
}

static int
lua_QStringInstanceMethodCount( lua_State *L ) {
  QString *udata  = checkQString( L );
  if(lua_isuserdata(L, 2)) {
    QRegExp *regex = *(QRegExp **) luaL_checkudata(L, 2, "QRegExp.metatable");
    lua_pushinteger(L, udata->count(*regex));
  } else {
    const char *str = lua_tostring(L, 2);
    QString sep(str);
    lua_pushinteger(L, udata->count(sep));
  }
  return 1;
}

static int
lua_QStringInstanceMethodContains( lua_State *L ) {
  QString *udata  = checkQString( L );
  if(lua_isuserdata(L, 2)) {
    QRegExp *regex = *(QRegExp **) luaL_checkudata(L, 2, "QRegExp.metatable");
    lua_pushboolean(L, udata->contains(*regex));
  } else {
    const char *str = lua_tostring(L, 2);
    QString sep(str);
    lua_pushboolean(L, udata->contains(sep));
  }
  return 1;
}

static int
lua_QStringInstanceMethodToUpper( lua_State *L ) {
  QString *udata  = checkQString( L );
  lua_pushstring(L, udata->toUpper().toLocal8Bit());
  return 1;
}

static const
luaL_reg QStringInstanceMethods[] = {
  {"toUpper", lua_QStringInstanceMethodToUpper},
  {"contains", lua_QStringInstanceMethodContains},
  {"count", lua_QStringInstanceMethodCount},
  {"replace", lua_QStringInstanceMethodReplace},
  {"split", lua_QStringInstanceMethodSplit},
  {"__concat", lua_QStringInstanceMethodConcat},
  {"__tostring", lua_QStringInstanceMethodToString},
  {"__gc", lua_QStringInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQStringInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QString.metatable");
  luaL_register(L, NULL, QStringInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QString( lua_State *L ) {
    registerQStringClassMethods(L);
    registerQStringInstanceMethods(L);
    return 1;
  }
}
