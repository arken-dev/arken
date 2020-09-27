// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>

#define QFileInfoList QList<QFileInfo>

/**
 * checkQFileInfoList
 */

QFileInfoList *
checkQFileInfoList( lua_State *L ) {
  return *(QFileInfoList **) luaL_checkudata(L, 1, "QFileInfoList.metatable");
}

/**
 * ClassMethods
 */

/*
static int
lua_QFileInfoListClassMethodNew( lua_State *L ) {
  const char *str = (char *) lua_tostring(L, 1);
  QFileInfoList **ptr = (QFileInfoList **)lua_newuserdata(L, sizeof(QFileInfoList*));
  *ptr= new QFileInfoList(QFileInfo(str));
  luaL_getmetatable(L, "QFileInfoList.metatable");
  lua_setmetatable(L, -2);
  return 1;
}
*/

static const luaL_reg QFileInfoListClassMethods[] = {
  //{"new", lua_QFileInfoListClassMethodNew},
  {NULL, NULL}
};

void static
registerQFileInfoListClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "QFileInfoList");
  luaL_register(L, NULL, QFileInfoListClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");

}

/**
 * InstanceMethods
 */

static int
lua_QFileInfoListInstanceMethodDestruct( lua_State *L ) {
  QFileInfoList *udata = checkQFileInfoList( L );
  delete udata;
  return 0;
}

static int
lua_QFileInfoListInstanceMethodAt( lua_State *L ) {
  QFileInfoList *list = checkQFileInfoList( L );
  int i = luaL_checkinteger(L, 2);
  QFileInfo value = list->at(i-1);

  QFileInfo **ptr = (QFileInfo **)lua_newuserdata(L, sizeof(QFileInfo*));
  *ptr = new QFileInfo(value);

  luaL_getmetatable(L, "QFileInfo.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_QFileInfoListInstanceMethodSize( lua_State *L ) {
  QFileInfoList *stringList = checkQFileInfoList( L );
  lua_pushinteger(L, stringList->size());
  return 1;
}



static const
luaL_reg QFileInfoListInstanceMethods[] = {
  {"size", lua_QFileInfoListInstanceMethodSize},
  {"at", lua_QFileInfoListInstanceMethodAt},
  {"__gc", lua_QFileInfoListInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQFileInfoListInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QFileInfoList.metatable");
  luaL_register(L, NULL, QFileInfoListInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QFileInfoList( lua_State *L ) {
    registerQFileInfoListInstanceMethods(L);
    registerQFileInfoListClassMethods(L);
    return 1;
  }
}
