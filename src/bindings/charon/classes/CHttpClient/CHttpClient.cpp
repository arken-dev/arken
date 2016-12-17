// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>
#include <CHttpClient>
#include <QByteArray>

/**
 * checkCHttpClient
 */

CHttpClient *
checkCHttpClient( lua_State *L ) {
  return *(CHttpClient **) luaL_checkudata(L, 1, "CHttpClient.metatable");
}

/**
 * ClassMethods
 */

static int
lua_CHttpClientClassMethodNew( lua_State *L ) {
  const char * url = luaL_checkstring(L, 1);
  CHttpClient **ptr = (CHttpClient **)lua_newuserdata(L, sizeof(CHttpClient*));
  *ptr = new CHttpClient(url);
  luaL_getmetatable(L, "CHttpClient.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg CHttpClientClassMethods[] = {
  {"new", lua_CHttpClientClassMethodNew},
  {NULL, NULL}
};

void static
registerCHttpClientClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "HttpClient");
  luaL_register(L, NULL, CHttpClientClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
lua_CHttpClientInstanceMethodAppendHeader( lua_State *L ) {
  CHttpClient *udata  = checkCHttpClient( L );
  const char * header = luaL_checkstring(L, 2);
  udata->appendHeader(header);
  return 0;
}

static int
lua_CHttpClientInstanceMethodSetVerbose( lua_State *L ) {
  CHttpClient *udata  = checkCHttpClient( L );
  bool verbose = lua_toboolean(L, 2);
  udata->setVerbose(verbose);
  return 0;
}

static int
lua_CHttpClientInstanceMethodVerbose( lua_State *L ) {
  CHttpClient *udata  = checkCHttpClient( L );
  bool verbose = udata->verbose();
  lua_pushinteger(L, verbose);
  return 1;
}

static int
lua_CHttpClientInstanceMethodSetBody( lua_State *L ) {
  CHttpClient *udata = checkCHttpClient( L );
  const char * body  = luaL_checkstring(L, 2);
  udata->setBody(body);
  return 0;
}

static int
lua_CHttpClientInstanceMethodBody( lua_State *L ) {
  CHttpClient *udata = checkCHttpClient( L );
  const char * body = udata->body();
  lua_pushstring(L, body);
  return 1;
}

static int
lua_CHttpClientInstanceMethodPerformGet( lua_State *L ) {
  CHttpClient *udata = checkCHttpClient( L );
  char * get = udata->performGet();
  lua_pushstring(L, get);
  delete[] get;
  return 1;
}

static int
lua_CHttpClientInstanceMethodPerformPost( lua_State *L ) {
  CHttpClient *udata = checkCHttpClient( L );
  char * post = udata->performPost();
  lua_pushstring(L, post);
  delete[] post;
  return 1;
}

static int
lua_CHttpClientInstanceMethodPerformPut( lua_State *L ) {
  CHttpClient *udata = checkCHttpClient( L );
  char * put = udata->performPut();
  lua_pushstring(L, put);
  delete[] put;
  return 1;
}

static int
lua_CHttpClientInstanceMethodPerformDelete( lua_State *L ) {
  CHttpClient *udata = checkCHttpClient( L );
  char * del = udata->performDelete();
  lua_pushstring(L, del);
  delete[] del;
  return 0;
}

static int
lua_CHttpClientInstanceMethodDestruct( lua_State *L ) {
  CHttpClient *udata = checkCHttpClient( L );
  delete udata;
  return 0;
}

static const
luaL_reg CHttpClientInstanceMethods[] = {
  {"appendHeader", lua_CHttpClientInstanceMethodAppendHeader},
  {"setVerbose", lua_CHttpClientInstanceMethodSetVerbose},
  {"verbose", lua_CHttpClientInstanceMethodVerbose},
  {"setBody", lua_CHttpClientInstanceMethodSetBody},
  {"body", lua_CHttpClientInstanceMethodBody},
  {"performGet", lua_CHttpClientInstanceMethodPerformGet},
  {"performPost", lua_CHttpClientInstanceMethodPerformPost},
  {"performPut", lua_CHttpClientInstanceMethodPerformPut},
  {"performDelete", lua_CHttpClientInstanceMethodPerformDelete},
  {"__gc", lua_CHttpClientInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerCHttpClientInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "CHttpClient.metatable");
  luaL_register(L, NULL, CHttpClientInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_charon_net_HttpClient( lua_State *L ) {
    registerCHttpClientInstanceMethods(L);
    registerCHttpClientClassMethods(L);
    return 1;
  }
}
