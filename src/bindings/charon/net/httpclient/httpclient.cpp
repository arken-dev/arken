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
lua_HttpClientClassMethodNew( lua_State *L ) {
  const char * url = luaL_checkstring(L, 1);
  HttpClient **ptr = (HttpClient **)lua_newuserdata(L, sizeof(HttpClient*));
  *ptr = new HttpClient(url);
  luaL_getmetatable(L, "HttpClient.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg HttpClientClassMethods[] = {
  {"new", lua_HttpClientClassMethodNew},
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
lua_HttpClientInstanceMethodAppendHeader( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  const char * header = luaL_checkstring(L, 2);
  udata->appendHeader(header);
  return 0;
}

static int
lua_HttpClientInstanceMethodSetVerbose( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  bool verbose = lua_toboolean(L, 2);
  udata->setVerbose(verbose);
  return 0;
}

static int
lua_HttpClientInstanceMethodVerbose( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  bool verbose = udata->verbose();
  lua_pushinteger(L, verbose);
  return 1;
}

static int
lua_HttpClientInstanceMethodSetBody( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  const char * body  = luaL_checkstring(L, 2);
  udata->setBody(body);
  return 0;
}

static int
lua_HttpClientInstanceMethodBody( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  const char * body = udata->body();
  lua_pushstring(L, body);
  return 1;
}

static int
lua_HttpClientInstanceMethodPerformGet( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  char * get = udata->performGet();
  lua_pushstring(L, get);
  delete[] get;
  return 1;
}

static int
lua_HttpClientInstanceMethodPerformPost( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  char * post = udata->performPost();
  lua_pushstring(L, post);
  delete[] post;
  return 1;
}

static int
lua_HttpClientInstanceMethodPerformPut( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  char * put = udata->performPut();
  lua_pushstring(L, put);
  delete[] put;
  return 1;
}

static int
lua_HttpClientInstanceMethodPerformDelete( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  char * del = udata->performDelete();
  lua_pushstring(L, del);
  delete[] del;
  return 0;
}

static int
lua_HttpClientInstanceMethodUrlRedirect( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  const char * urlRedirect = udata->urlRedirect();
  lua_pushstring(L, urlRedirect);
  return 1;
}

static int
lua_HttpClientInstanceMethodDestruct( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  delete udata;
  return 0;
}

static const
luaL_reg HttpClientInstanceMethods[] = {
  {"appendHeader", lua_HttpClientInstanceMethodAppendHeader},
  {"setVerbose", lua_HttpClientInstanceMethodSetVerbose},
  {"verbose", lua_HttpClientInstanceMethodVerbose},
  {"setBody", lua_HttpClientInstanceMethodSetBody},
  {"body", lua_HttpClientInstanceMethodBody},
  {"performGet", lua_HttpClientInstanceMethodPerformGet},
  {"performPost", lua_HttpClientInstanceMethodPerformPost},
  {"performPut", lua_HttpClientInstanceMethodPerformPut},
  {"performDelete", lua_HttpClientInstanceMethodPerformDelete},
  {"urlRedirect", lua_HttpClientInstanceMethodUrlRedirect},
  {"__gc", lua_HttpClientInstanceMethodDestruct},
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
