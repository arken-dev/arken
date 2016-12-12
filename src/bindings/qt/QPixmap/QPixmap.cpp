// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QPixmap>
#include <iostream>

/**
 * checkQPixmap
 */

QPixmap *
checkQPixmap( lua_State *L ) {
  return *(QPixmap **) luaL_checkudata(L, 1, "QPixmap.metatable");
}

/**
 * ClassMethods
 */

static int
lua_QPixmapClassMethodNew( lua_State *L ) {
  const char *str = (char *) lua_tostring(L, 1);
  QPixmap **ptr = (QPixmap **)lua_newuserdata(L, sizeof(QPixmap*));
  *ptr= new QPixmap(QString(str));
  luaL_getmetatable(L, "QPixmap.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg QPixmapClassMethods[] = {
  {"new", lua_QPixmapClassMethodNew},
  {NULL, NULL}
};

void static
registerQPixmapClassMethods( lua_State *L ) {
  luaL_register(L, "QPixmap", QPixmapClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_QPixmapInstanceMethodDestruct( lua_State *L ) {
  QPixmap *udata = checkQPixmap( L );
  delete udata;
  return 0;
}

static const
luaL_reg QPixmapInstanceMethods[] = {
  {"__gc", lua_QPixmapInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQPixmapInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QPixmap.metatable");
  luaL_register(L, NULL, QPixmapInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QPixmap( lua_State *L ) {
    registerQPixmapClassMethods(L);
    registerQPixmapInstanceMethods(L);
    return 1;
  }
}
