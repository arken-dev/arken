// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/net/httpbody.h>
#include <QtCore>
#include <iostream>
#include <QByteArray>

using charon::net::HttpBody;

/**
 * checkHttpBody
 */

HttpBody *
checkHttpBody( lua_State *L ) {
  return *(HttpBody **) luaL_checkudata(L, 1, "HttpBody.metatable");
}

/**
 * ClassMethods
 */

static int
charon_HttpBodyClassMethodLoadFile( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  HttpBody **ptr = (HttpBody **)lua_newuserdata(L, sizeof(HttpBody*));
  *ptr = HttpBody::loadFile(path);
  luaL_getmetatable(L, "HttpBody.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
charon_HttpBodyClassMethodLoadBuffer( lua_State *L ) {
  size_t size;
  const char * buffer = luaL_checklstring(L, 1, &size);
  HttpBody **ptr = (HttpBody **)lua_newuserdata(L, sizeof(HttpBody*));
  *ptr = HttpBody::loadBuffer(buffer, size);
  luaL_getmetatable(L, "HttpBody.metatable");
  lua_setmetatable(L, -2);
  return 1;
}


static const luaL_reg HttpBodyClassMethods[] = {
  {"loadFile",   charon_HttpBodyClassMethodLoadFile},
  {"loadBuffer", charon_HttpBodyClassMethodLoadBuffer},
  {NULL, NULL}
};

void static
registerHttpBodyClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "HttpBody");
  luaL_register(L, NULL, HttpBodyClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
charon_HttpBodyInstanceMethodRead( lua_State *L ) {
  HttpBody *udata  = checkHttpBody( L );
  const char * buffer = udata->read();
  lua_pushlstring(L, buffer, udata->size());
  return 1;
}

static int
charon_HttpBodyInstanceMethodSize( lua_State *L ) {
  HttpBody *udata  = checkHttpBody( L );
  lua_pushnumber(L, udata->size());
  return 1;
}

static int
charon_HttpBodyInstanceMethodRelease( lua_State *L ) {
  HttpBody *udata  = checkHttpBody( L );
  udata->release();
  return 0;
}


static int
charon_HttpBodyInstanceMethodDestruct( lua_State *L ) {
  HttpBody *udata = checkHttpBody( L );
  delete udata;
  return 0;
}

static const
luaL_reg HttpBodyInstanceMethods[] = {
  {"size",    charon_HttpBodyInstanceMethodSize},
  {"read",    charon_HttpBodyInstanceMethodRead},
  {"release", charon_HttpBodyInstanceMethodRelease},
  {"__gc",    charon_HttpBodyInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerHttpBodyInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "HttpBody.metatable");
  luaL_register(L, NULL, HttpBodyInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_charon_net_HttpBody( lua_State *L ) {
    registerHttpBodyInstanceMethods(L);
    registerHttpBodyClassMethods(L);
    return 1;
  }
}
