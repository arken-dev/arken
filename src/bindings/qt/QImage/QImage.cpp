// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QImage>
#include <iostream>

/**
 * checkQImage
 */

QImage *
checkQImage( lua_State *L ) {
  return *(QImage **) luaL_checkudata(L, 1, "QImage.metatable");
}

/**
 * ClassMethods
 */

static int
lua_QImageClassMethodNew( lua_State *L ) {
  const char *str = (char *) lua_tostring(L, 1);
  QImage **ptr = (QImage **)lua_newuserdata(L, sizeof(QImage*));
  *ptr= new QImage(QString(str));
  luaL_getmetatable(L, "QImage.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg QImageClassMethods[] = {
  {"new", lua_QImageClassMethodNew},
  {NULL, NULL}
};

void static
registerQImageClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "QImage");
  luaL_register(L, NULL, QImageClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");

}

/**
 * InstanceMethods
 */

static int
lua_QImageInstanceMethodDestruct( lua_State *L ) {
  QImage *udata = checkQImage( L );
  delete udata;
  return 0;
}

static const
luaL_reg QImageInstanceMethods[] = {
  {"__gc", lua_QImageInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQImageInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QImage.metatable");
  luaL_register(L, NULL, QImageInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QImage( lua_State *L ) {
    registerQImageInstanceMethods(L);
    registerQImageClassMethods(L);
    return 1;
  }
}
