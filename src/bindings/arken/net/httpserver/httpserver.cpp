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
  return *static_cast<HttpServer **>(luaL_checkudata(L, 1, "arken.net.HttpServer.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_net_HttpServer_new( lua_State *L ) {
  const char * address = luaL_checkstring(L, 1);
  int port = luaL_checkinteger(L, 2); 
  auto ptr = static_cast<HttpServer **>(lua_newuserdata(L, sizeof(HttpServer*)));
  *ptr = new HttpServer(address, port);
  luaL_getmetatable(L, "arken.net.HttpServer.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_net_HttpServer[] = {
  {"new", arken_net_HttpServer_new},
  {nullptr, nullptr}
};

void static
register_arken_net_HttpServer( lua_State *L ) {
  luaL_newmetatable(L,  "arken.net.HttpServer");
  luaL_register(L, nullptr, arken_net_HttpServer);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_net_HttpServer_start( lua_State *L ) {
  HttpServer * udata = checkHttpServer( L );
  udata->start();
  return 0;
}

static int
arken_net_HttpServer_setThreads( lua_State *L ) {
  HttpServer * udata  = checkHttpServer( L );
  int threads = luaL_checkint(L, 2);
  udata->setThreads(threads);
  return 0;
}

static int
arken_net_HttpServer_setPid( lua_State *L ) {
  HttpServer * udata  = checkHttpServer( L );
  const char * pid = luaL_checkstring(L, 2);
  udata->setPid(pid);
  return 0;
}

static int
arken_net_HttpServer_setDispatcher( lua_State *L ) {
  HttpServer * udata  = checkHttpServer( L );
  const char * dispatcher = luaL_checkstring(L, 2);
  udata->setDispatcher(dispatcher);
  return 0;
}

static int
arken_net_HttpServer_addService( lua_State *L ) {
  HttpServer * udata  = checkHttpServer( L );
  const char * service = luaL_checkstring(L, 2);
  udata->addService(service);
  return 0;
}

static int
arken_net_HttpServer_gc( lua_State *L ) {
  HttpServer *udata = checkHttpServer( L );
  delete udata;
  return 0;
}

static const
luaL_reg arken_net_HttpServer_metatable[] = {
  {"start",         arken_net_HttpServer_start},
  {"setThreads",    arken_net_HttpServer_setThreads},
  {"setPid",        arken_net_HttpServer_setPid},
  {"setDispatcher", arken_net_HttpServer_setDispatcher},
  {"addService",    arken_net_HttpServer_addService},
  {"__gc",          arken_net_HttpServer_gc},
  {nullptr, nullptr}
};

void static
register_arken_net_HttpServer_metatable( lua_State *L ) {
  luaL_newmetatable(L,  "arken.net.HttpServer.metatable");
  luaL_register(L, nullptr, arken_net_HttpServer_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_net_HttpServer( lua_State *L ) {
    register_arken_net_HttpServer_metatable(L);
    register_arken_net_HttpServer(L);
    return 1;
  }
}
