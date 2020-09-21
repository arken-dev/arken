// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>

using arken::net::HttpClient;

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
arken_HttpClientClassMethodNew( lua_State *L ) {
  const char * url = luaL_checkstring(L, 1);
  HttpClient **ptr = (HttpClient **)lua_newuserdata(L, sizeof(HttpClient*));
  *ptr = new HttpClient(url);
  luaL_getmetatable(L, "HttpClient.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg HttpClientClassMethods[] = {
  {"new", arken_HttpClientClassMethodNew},
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
arken_HttpClientInstanceMethodAppendHeader( lua_State *L ) {
  HttpClient * udata  = checkHttpClient( L );
  const char * header = luaL_checkstring(L, 2);
  udata->appendHeader(header);
  return 0;
}

static int
arken_HttpClientInstanceMethodSetVerbose( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  bool verbose = lua_toboolean(L, 2);
  udata->setVerbose(verbose);
  return 0;
}

static int
arken_HttpClientInstanceMethodSetBody( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  const char * body  = luaL_checkstring(L, 2);
  udata->setBody(body);
  return 0;
}

static int
arken_HttpClientInstanceMethodBody( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  const char * body  = udata->body();
  lua_pushstring(L, body);
  return 1;
}

static int
arken_HttpClientInstanceMethodPerformGet( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  string result = udata->performGet();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_HttpClientInstanceMethodPerformPost( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  string result = udata->performPost();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_HttpClientInstanceMethodPerformPut( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  string result = udata->performPut();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_HttpClientInstanceMethodPerformDelete( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  string result = udata->performDelete();
  lua_pushlstring(L, result.data(), result.size());
  return 0;
}

static int
arken_HttpClientInstanceMethodData( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  const char * data  = udata->data();
  lua_pushstring(L, data);
  return 1;
}

static int
arken_HttpClientInstanceMethodMessage( lua_State *L ) {
  HttpClient * udata   = checkHttpClient( L );
  const char * message = udata->message();
  lua_pushstring(L, message);
  return 1;
}

static int
arken_HttpClientInstanceMethodStatus( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  lua_pushinteger(L, udata->status());
  return 1;
}

static int
arken_HttpClientInstanceMethodFailure( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  lua_pushboolean(L, udata->failure());
  return 1;
}

static int
arken_HttpClientInstanceMethodDestruct( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  delete udata;
  return 0;
}

static const
luaL_reg HttpClientInstanceMethods[] = {
  {"appendHeader", arken_HttpClientInstanceMethodAppendHeader},
  {"setVerbose", arken_HttpClientInstanceMethodSetVerbose},
  {"setBody", arken_HttpClientInstanceMethodSetBody},
  {"body", arken_HttpClientInstanceMethodBody},
  {"performGet", arken_HttpClientInstanceMethodPerformGet},
  {"performPost", arken_HttpClientInstanceMethodPerformPost},
  {"performPut", arken_HttpClientInstanceMethodPerformPut},
  {"performDelete", arken_HttpClientInstanceMethodPerformDelete},
  {"status",        arken_HttpClientInstanceMethodStatus},
  {"data",        arken_HttpClientInstanceMethodData},
  {"failure",     arken_HttpClientInstanceMethodFailure},
  {"message",     arken_HttpClientInstanceMethodMessage},
  {"__gc", arken_HttpClientInstanceMethodDestruct},
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
  luaopen_arken_net_HttpClient( lua_State *L ) {
    registerHttpClientInstanceMethods(L);
    registerHttpClientClassMethods(L);
    return 1;
  }
}
