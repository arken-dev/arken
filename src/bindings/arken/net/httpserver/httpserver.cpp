// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/net/httpserver.h>

using arken::net::HttpServer;
using arken::string;

/**
 * checkHttpServer
 */

HttpServer *
checkHttpServer( lua_State *L ) {
  return *(HttpServer **) luaL_checkudata(L, 1, "HttpServer.metatable");
}

/**
 * ClassMethods
 */

static int
arken_HttpServerClassMethodNew( lua_State *L ) {
  const char * address = luaL_checkstring(L, 1);
  int port = luaL_checkinteger(L, 2); 
  HttpServer **ptr = (HttpServer **)lua_newuserdata(L, sizeof(HttpServer*));
  *ptr = new HttpServer(address, port);
  luaL_getmetatable(L, "HttpServer.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg HttpServerClassMethods[] = {
  {"new", arken_HttpServerClassMethodNew},
  {NULL, NULL}
};

void static
registerHttpServerClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "HttpServer");
  luaL_register(L, NULL, HttpServerClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_HttpServerInstanceMethodStart( lua_State *L ) {
  HttpServer * udata = checkHttpServer( L );
  udata->start();
  return 0;
}

static int
arken_HttpServerInstanceMethodSetThreads( lua_State *L ) {
  HttpServer * udata  = checkHttpServer( L );
  int threads = luaL_checkint(L, 2);
  udata->setThreads(threads);
  return 0;
}

static int
arken_HttpServerInstanceMethodSetPid( lua_State *L ) {
  HttpServer * udata  = checkHttpServer( L );
  const char * pid = luaL_checkstring(L, 2);
  udata->setPid(pid);
  return 0;
}

static int
arken_HttpServerInstanceMethodSetDispatcher( lua_State *L ) {
  HttpServer * udata  = checkHttpServer( L );
  const char * dispatcher = luaL_checkstring(L, 2);
  udata->setDispatcher(dispatcher);
  return 0;
}

static int
arken_HttpServerInstanceMethodDestruct( lua_State *L ) {
  HttpServer *udata = checkHttpServer( L );
  delete udata;
  return 0;
}

static const
luaL_reg HttpServerInstanceMethods[] = {
  {"start",         arken_HttpServerInstanceMethodStart},
  {"setThreads",    arken_HttpServerInstanceMethodSetThreads},
  {"setPid",        arken_HttpServerInstanceMethodSetPid},
  {"setDispatcher", arken_HttpServerInstanceMethodSetDispatcher},
  {"__gc",          arken_HttpServerInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerHttpServerInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "HttpServer.metatable");
  luaL_register(L, NULL, HttpServerInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_net_HttpServer( lua_State *L ) {
    registerHttpServerInstanceMethods(L);
    registerHttpServerClassMethods(L);
    return 1;
  }
}
