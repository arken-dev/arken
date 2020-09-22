// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>

#define QByteArrayList QList<QByteArray>

/**
 * checkQByteArrayList
 */

QByteArrayList *
checkQByteArrayList( lua_State *L ) {
  return *(QByteArrayList **) luaL_checkudata(L, 1, "QByteArrayList.metatable");
}

/**
 * ClassMethods
 */
/*
static int
lua_QByteArrayListClassMethodNew( lua_State *L ) {
  const char *str = (char *) lua_tostring(L, 1);
  QByteArrayList **ptr = (QByteArrayList **)lua_newuserdata(L, sizeof(QByteArrayList*));
  *ptr= new QByteArrayList(QByteArray(str));
  luaL_getmetatable(L, "QByteArrayList.metatable");
  lua_setmetatable(L, -2);
  return 1;
}
*/

static const luaL_reg QByteArrayListClassMethods[] = {
 // {"new", lua_QByteArrayListClassMethodNew},
  {NULL, NULL}
};

void static
registerQByteArrayListClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "QByteArrayList");
  luaL_register(L, NULL, QByteArrayListClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
lua_QByteArrayListInstanceMethodDestruct( lua_State *L ) {
  QByteArrayList *udata = checkQByteArrayList( L );
  delete udata;
  return 0;
}

static int
lua_QByteArrayListInstanceMethodAt( lua_State *L ) {
  QByteArrayList *stringList = checkQByteArrayList( L );
  int i = luaL_checkinteger(L, 2);
  QByteArray value = stringList->at(i-1);;
  lua_pushstring(L, value);
  return 1;
}

static int
lua_QByteArrayListInstanceMethodSize( lua_State *L ) {
  QByteArrayList *stringList = checkQByteArrayList( L );
  lua_pushinteger(L, stringList->size());
  return 1;
}

static int
lua_QByteArrayListInstanceMethodContains( lua_State *L ) {
  QByteArrayList * stringList = checkQByteArrayList( L );
  char const  * str = luaL_checkstring(L, 2);
  lua_pushboolean(L, stringList->contains(QByteArray(str)));
  return 1;
}

static const
luaL_reg QByteArrayListInstanceMethods[] = {
  {"contains", lua_QByteArrayListInstanceMethodContains},
  {"size", lua_QByteArrayListInstanceMethodSize},
  {"at", lua_QByteArrayListInstanceMethodAt},
  {"__gc", lua_QByteArrayListInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQByteArrayListInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QByteArrayList.metatable");
  luaL_register(L, NULL, QByteArrayListInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QByteArrayList( lua_State *L ) {
    registerQByteArrayListInstanceMethods(L);
    registerQByteArrayListClassMethods(L);
    return 1;
  }
}
