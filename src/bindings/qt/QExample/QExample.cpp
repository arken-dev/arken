// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>

/**
 * checkQExample
 */

QExample *
checkQExample( lua_State *L ) {
  return *(QExample **) luaL_checkudata(L, 1, "QExample.metatable");
}

/**
 * ClassMethods
 */

static int
lua_QExampleClassMethodNew( lua_State *L ) {
  const char *str = (char *) lua_tostring(L, 1);
  QExample **ptr = (QExample **)lua_newuserdata(L, sizeof(QExample*));
  *ptr= new QExample(QString(str));
  luaL_getmetatable(L, "QExample.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg QExampleClassMethods[] = {
  {"new", lua_QExampleClassMethodNew},
  {NULL, NULL}
};

void static
registerQExampleClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "QExample");
  luaL_register(L, NULL, QExampleClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
lua_QExampleInstanceMethodDestruct( lua_State *L ) {
  QExample *udata = checkQExample( L );
  delete udata;
  return 0;
}

static const
luaL_reg QExampleInstanceMethods[] = {
  {"__gc", lua_QExampleInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQExampleInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QExample.metatable");
  luaL_register(L, NULL, QExampleInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QExample( lua_State *L ) {
    registerQExampleInstanceMethods(L);
    registerQExampleClassMethods(L);
    return 1;
  }
}
