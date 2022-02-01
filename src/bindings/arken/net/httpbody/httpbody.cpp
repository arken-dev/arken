// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/net/httpbody.h>

using arken::net::HttpBody;

/**
 * checkHttpBody
 */

HttpBody *
checkHttpBody( lua_State *L ) {
  return *(HttpBody **) luaL_checkudata(L, 1, "arken.net.HttpBody.metatable");
}

/**
 * ClassMethods
 */

static int
arken_net_HttpBody_loadFile( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  HttpBody **ptr = (HttpBody **)lua_newuserdata(L, sizeof(HttpBody*));
  *ptr = HttpBody::loadFile(path);
  luaL_getmetatable(L, "arken.net.HttpBody.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_net_HttpBody_loadBuffer( lua_State *L ) {
  size_t size;
  const char * buffer = luaL_checklstring(L, 1, &size);
  HttpBody **ptr = (HttpBody **)lua_newuserdata(L, sizeof(HttpBody*));
  *ptr = HttpBody::loadBuffer(buffer, size);
  luaL_getmetatable(L, "arken.net.HttpBody.metatable");
  lua_setmetatable(L, -2);
  return 1;
}


static const luaL_reg arken_HttpBody[] = {
  {"loadFile",   arken_net_HttpBody_loadFile},
  {"loadBuffer", arken_net_HttpBody_loadBuffer},
  {NULL, NULL}
};

void static
register_arken_HttpBody( lua_State *L ) {
  luaL_newmetatable(L, "arken.net.HttpBody");
  luaL_register(L, NULL, arken_HttpBody);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_net_HttpBody_read( lua_State *L ) {
  HttpBody *udata  = checkHttpBody( L );
  const char * buffer = udata->read();
  lua_pushlstring(L, buffer, udata->size());
  return 1;
}

static int
arken_net_HttpBody_size( lua_State *L ) {
  HttpBody *udata  = checkHttpBody( L );
  lua_pushnumber(L, udata->size());
  return 1;
}

static int
arken_net_HttpBody_release( lua_State *L ) {
  HttpBody *udata  = checkHttpBody( L );
  udata->release();
  return 0;
}

static int
arken_net_HttpBody_gc( lua_State *L ) {
  HttpBody *udata = checkHttpBody( L );
  delete udata;
  return 0;
}

static const
luaL_reg arken_HttpBody_metatable[] = {
  {"size",    arken_net_HttpBody_size},
  {"read",    arken_net_HttpBody_read},
  {"release", arken_net_HttpBody_release},
  {"__gc",    arken_net_HttpBody_gc},
  {NULL, NULL}
};

void static
register_arken_HttpBody_metatable( lua_State *L ) {
  luaL_newmetatable(L,  "arken.net.HttpBody.metatable");
  luaL_register(L, NULL, arken_HttpBody_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_net_HttpBody( lua_State *L ) {
    register_arken_HttpBody_metatable(L);
    register_arken_HttpBody(L);
    return 1;
  }

}
