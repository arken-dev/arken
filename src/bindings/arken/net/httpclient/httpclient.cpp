// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/net/httpclient.h>

using arken::net::HttpClient;
using arken::string;

/**
 * checkHttpClient
 */

HttpClient *
checkHttpClient( lua_State *L ) {
  return *(HttpClient **) luaL_checkudata(L, 1, "arken.net.HttpClient.metatable");
}

/**
 * ClassMethods
 */

static int
arken_HttpClientClassMethodNew( lua_State *L ) {
  const char * url = luaL_checkstring(L, 1);
  HttpClient **ptr = (HttpClient **)lua_newuserdata(L, sizeof(HttpClient*));
  *ptr = new HttpClient(url);
  luaL_getmetatable(L, "arken.net.HttpClient.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg HttpClientClassMethods[] = {
  {"new", arken_HttpClientClassMethodNew},
  {NULL, NULL}
};

void static
registerHttpClientClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.net.HttpClient");
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
arken_HttpClientInstanceMethodSetSslVerifyPeer( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  bool sslVerifyPeer = lua_toboolean(L, 2);
  udata->setSslVerifyPeer(sslVerifyPeer);
  return 0;
}

static int
arken_HttpClientInstanceMethodSetSslVerifyHost( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  long sslVerifyHost = (long) luaL_checkint(L, 2);
  udata->setSslVerifyHost(sslVerifyHost);
  return 0;
}

static int
arken_HttpClientInstanceMethodSetSslVersion( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  long sslVersion = (long) luaL_checkint(L, 2);
  udata->setSslVersion(sslVersion);
  return 0;
}

static int
arken_HttpClientInstanceMethodSetUseSsl( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  long useSsl = (long) luaL_checkint(L, 2);
  udata->setUseSsl(useSsl);
  return 0;
}

static int
arken_HttpClientInstanceMethodSetBody( lua_State *L ) {
  size_t len;
  HttpClient * udata = checkHttpClient( L );
  const char * body  = luaL_checklstring(L, 2, &len);
  udata->setBody(string(body, len));
  return 0;
}

static int
arken_HttpClientInstanceMethodSetCert( lua_State *L ) {
  size_t len;
  HttpClient * udata = checkHttpClient( L );
  const char * cert  = luaL_checklstring(L, 2, &len);
  udata->setCert(string(cert, len));
  return 0;
}

static int
arken_HttpClientInstanceMethodSetCertKey( lua_State *L ) {
  size_t len;
  HttpClient * udata = checkHttpClient( L );
  const char * key = luaL_checklstring(L, 2, &len);
  udata->setCertKey(string(key, len));
  return 0;
}

static int
arken_HttpClientInstanceMethodBody( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  string body = udata->body();
  lua_pushlstring(L, body.data(), body.size());
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
  string data = udata->data();
  lua_pushlstring(L, data.data(), data.size());
  return 1;
}

static int
arken_HttpClientInstanceMethodMessage( lua_State *L ) {
  HttpClient * udata   = checkHttpClient( L );
  string message = udata->message();
  lua_pushlstring(L, message.data(), message.size());
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
  {"appendHeader",     arken_HttpClientInstanceMethodAppendHeader},
  {"setVerbose",       arken_HttpClientInstanceMethodSetVerbose},
  {"setSslVerifyPeer", arken_HttpClientInstanceMethodSetSslVerifyPeer},
  {"setSslVerifyHost", arken_HttpClientInstanceMethodSetSslVerifyHost},
  {"setSslVersion",    arken_HttpClientInstanceMethodSetSslVersion},
  {"setUseSsl",        arken_HttpClientInstanceMethodSetUseSsl},
  {"setBody",          arken_HttpClientInstanceMethodSetBody},
  {"setCert",          arken_HttpClientInstanceMethodSetCert},
  {"setCertKey",       arken_HttpClientInstanceMethodSetCertKey},
  {"body",             arken_HttpClientInstanceMethodBody},
  {"performGet",       arken_HttpClientInstanceMethodPerformGet},
  {"performPost",      arken_HttpClientInstanceMethodPerformPost},
  {"performPut",       arken_HttpClientInstanceMethodPerformPut},
  {"performDelete",    arken_HttpClientInstanceMethodPerformDelete},
  {"status",           arken_HttpClientInstanceMethodStatus},
  {"data",             arken_HttpClientInstanceMethodData},
  {"failure",          arken_HttpClientInstanceMethodFailure},
  {"message",          arken_HttpClientInstanceMethodMessage},
  {"__gc",             arken_HttpClientInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerHttpClientInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.net.HttpClient.metatable");
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
