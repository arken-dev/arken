// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>

/**
 * checkQStringList
 */

QStringList *
checkQStringList( lua_State *L ) {
  return *(QStringList **) luaL_checkudata(L, 1, "QStringList.metatable");
}

/**
 * ClassMethods
 */

static int
lua_QStringListClassMethodNew( lua_State *L ) {
  const char *str = (char *) lua_tostring(L, 1);
  QStringList **ptr = (QStringList **)lua_newuserdata(L, sizeof(QStringList*));
  *ptr= new QStringList(QString(str));
  luaL_getmetatable(L, "QStringList.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg QStringListClassMethods[] = {
  {"new", lua_QStringListClassMethodNew},
  {NULL, NULL}
};

void static
registerQStringListClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "QStringList");
  luaL_register(L, NULL, QStringListClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");

}

/**
 * InstanceMethods
 */

static int
lua_QStringListInstanceMethodDestruct( lua_State *L ) {
  QStringList *udata = checkQStringList( L );
  delete udata;
  return 0;
}

static int
lua_QStringListInstanceMethodAt( lua_State *L ) {
  QStringList *stringList = checkQStringList( L );
  int i = luaL_checkinteger(L, 2);
  QString value = stringList->at(i-1);;

  QString **ptr = (QString **)lua_newuserdata(L, sizeof(QString*));
  *ptr = new QString(value);

  luaL_getmetatable(L, "QString.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_QStringListInstanceMethodSize( lua_State *L ) {
  QStringList *stringList = checkQStringList( L );
  lua_pushinteger(L, stringList->size());
  return 1;
}

static int
lua_QStringListInstanceMethodContains( lua_State *L ) {
  QStringList * stringList = checkQStringList( L );
  char const  * str = luaL_checkstring(L, 2);
  lua_pushboolean(L, stringList->contains(QString(str)));
  return 1;
}

static const
luaL_reg QStringListInstanceMethods[] = {
  {"contains", lua_QStringListInstanceMethodContains},
  {"size", lua_QStringListInstanceMethodSize},
  {"at", lua_QStringListInstanceMethodAt},
  {"__gc", lua_QStringListInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQStringListInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QStringList.metatable");
  luaL_register(L, NULL, QStringListInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QStringList( lua_State *L ) {
    registerQStringListInstanceMethods(L);
    registerQStringListClassMethods(L);
    return 1;
  }
}
