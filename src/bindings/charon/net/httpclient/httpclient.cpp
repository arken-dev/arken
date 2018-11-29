// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>

using charon::net::HttpClient;

/**
 * checkHttpClient
 */

HttpClient *
checkHttpClient( lua_State *L ) {
  return *(HttpClient **) luaL_checkudata(L, 1, "HttpClient.metatable");
}

/**
 * ClassMethods
 */

static int
charon_HttpClientClassMethodNew( lua_State *L ) {
  const char * url = luaL_checkstring(L, 1);
  HttpClient **ptr = (HttpClient **)lua_newuserdata(L, sizeof(HttpClient*));
  *ptr = new HttpClient(url);
  luaL_getmetatable(L, "HttpClient.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg HttpClientClassMethods[] = {
  {"new", charon_HttpClientClassMethodNew},
  {NULL, NULL}
};

void static
registerHttpClientClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "HttpClient");
  luaL_register(L, NULL, HttpClientClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
charon_HttpClientInstanceMethodAppendHeader( lua_State *L ) {
  HttpClient * udata  = checkHttpClient( L );
  const char * header = luaL_checkstring(L, 2);
  udata->appendHeader(header);
  return 0;
}

static int
charon_HttpClientInstanceMethodSetVerbose( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  bool verbose = lua_toboolean(L, 2);
  udata->setVerbose(verbose);
  return 0;
}

static int
charon_HttpClientInstanceMethodSetBody( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  const char * body  = luaL_checkstring(L, 2);
  udata->setBody(body);
  return 0;
}

static int
charon_HttpClientInstanceMethodBody( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  const char * body  = udata->body();
  lua_pushstring(L, body);
  return 1;
}

static int
charon_HttpClientInstanceMethodPerformGet( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  char * result = udata->performGet();
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_HttpClientInstanceMethodPerformPost( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  char * result = udata->performPost();
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_HttpClientInstanceMethodPerformPut( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  char * result = udata->performPut();
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}

static int
charon_HttpClientInstanceMethodPerformDelete( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  char * result = udata->performDelete();
  lua_pushstring(L, result);
  delete[] result;
  return 0;
}

static int
charon_HttpClientInstanceMethodData( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  const char * data  = udata->data();
  lua_pushstring(L, data);
  return 1;
}

static int
charon_HttpClientInstanceMethodStatus( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  lua_pushinteger(L, udata->status());
  return 1;
}

static int
charon_HttpClientInstanceMethodDestruct( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  delete udata;
  return 0;
}

static const
luaL_reg HttpClientInstanceMethods[] = {
  {"appendHeader", charon_HttpClientInstanceMethodAppendHeader},
  {"setVerbose", charon_HttpClientInstanceMethodSetVerbose},
  {"setBody", charon_HttpClientInstanceMethodSetBody},
  {"body", charon_HttpClientInstanceMethodBody},
  {"performGet", charon_HttpClientInstanceMethodPerformGet},
  {"performPost", charon_HttpClientInstanceMethodPerformPost},
  {"performPut", charon_HttpClientInstanceMethodPerformPut},
  {"performDelete", charon_HttpClientInstanceMethodPerformDelete},
  {"status",        charon_HttpClientInstanceMethodStatus},
  {"data",        charon_HttpClientInstanceMethodData},
  {"__gc", charon_HttpClientInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerHttpClientInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "HttpClient.metatable");
  luaL_register(L, NULL, HttpClientInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_charon_net_HttpClient( lua_State *L ) {
    registerHttpClientInstanceMethods(L);
    registerHttpClientClassMethods(L);
    return 1;
  }
}
