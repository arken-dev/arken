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
  return *static_cast<HttpClient **>(luaL_checkudata(L, 1, "arken.net.HttpClient.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_net_HttpClient_new( lua_State *L ) {
  const char * url = luaL_checkstring(L, 1);
  auto ptr = static_cast<HttpClient **>(lua_newuserdata(L, sizeof(HttpClient*)));
  *ptr = new HttpClient(url);
  luaL_getmetatable(L, "arken.net.HttpClient.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_net_HttpClient_parseStatus( lua_State *L ) {
  const char * data = luaL_checkstring(L, 1);
  lua_pushnumber(L, HttpClient::parseStatus(data));
  return 1;
}

static const luaL_reg arken_net_HttpClient[] = {
  {"new",         arken_net_HttpClient_new},
  {"parseStatus", arken_net_HttpClient_parseStatus},
  {nullptr, nullptr}
};

void static
register_arken_net_HttpClient( lua_State *L ) {
  luaL_newmetatable(L, "arken.net.HttpClient");
  luaL_register(L, nullptr, arken_net_HttpClient);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_net_HttpClient_appendHeader( lua_State *L ) {
  HttpClient * udata  = checkHttpClient( L );
  const char * header = luaL_checkstring(L, 2);
  udata->appendHeader(header);
  return 0;
}

static int
arken_net_HttpClient_setVerbose( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  bool verbose = lua_toboolean(L, 2);
  udata->setVerbose(verbose);
  return 0;
}

static int
arken_net_HttpClient_setSslVerifyPeer( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  bool sslVerifyPeer = lua_toboolean(L, 2);
  udata->setSslVerifyPeer(sslVerifyPeer);
  return 0;
}

static int
arken_net_HttpClient_setSslVerifyHost( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  auto sslVerifyHost = (long) luaL_checkint(L, 2);
  udata->setSslVerifyHost(sslVerifyHost);
  return 0;
}

static int
arken_net_HttpClient_setSslVersion( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  auto sslVersion = (long) luaL_checkint(L, 2);
  udata->setSslVersion(sslVersion);
  return 0;
}

static int
arken_net_HttpClient_setUseSsl( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  auto useSsl = (long) luaL_checkint(L, 2);
  udata->setUseSsl(useSsl);
  return 0;
}

static int
arken_net_HttpClient_setBody( lua_State *L ) {
  size_t len;
  HttpClient * udata = checkHttpClient( L );
  const char * body  = luaL_checklstring(L, 2, &len);
  udata->setBody(string(body, len));
  return 0;
}

static int
arken_net_HttpClient_setTimeout( lua_State *L ) {
  HttpClient *udata  = checkHttpClient( L );
  const long timeout = luaL_checkint(L, 2);
  udata->setTimeout(timeout);
  return 0;
}

static int
arken_net_HttpClient_setCert( lua_State *L ) {
  size_t len;
  HttpClient * udata = checkHttpClient( L );
  const char * cert  = luaL_checklstring(L, 2, &len);
  udata->setCert(string(cert, len));
  return 0;
}

static int
arken_net_HttpClient_setCertKey( lua_State *L ) {
  size_t len;
  HttpClient * udata = checkHttpClient( L );
  const char * key = luaL_checklstring(L, 2, &len);
  udata->setCertKey(string(key, len));
  return 0;
}

static int
arken_net_HttpClient_body( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  string body = udata->body();
  lua_pushlstring(L, body.data(), body.size());
  return 1;
}

static int
arken_net_HttpClient_performGet( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  string result = udata->performGet();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_net_HttpClient_performPost( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  string result = udata->performPost();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_net_HttpClient_performPatch( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  string result = udata->performPatch();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_net_HttpClient_performPut( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  string result = udata->performPut();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_net_HttpClient_performDelete( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  string result = udata->performDelete();
  lua_pushlstring(L, result.data(), result.size());
  return 0;
}

static int
arken_net_HttpClient_data( lua_State *L ) {
  HttpClient * udata = checkHttpClient( L );
  string data = udata->data();
  lua_pushlstring(L, data.data(), data.size());
  return 1;
}

static int
arken_net_HttpClient_message( lua_State *L ) {
  HttpClient * udata   = checkHttpClient( L );
  string message = udata->message();
  lua_pushlstring(L, message.data(), message.size());
  return 1;
}

static int
arken_net_HttpClient_status( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  lua_pushinteger(L, udata->status());
  return 1;
}

static int
arken_net_HttpClient_failure( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  lua_pushboolean(L, udata->failure());
  return 1;
}

static int
arken_net_HttpClient_gc( lua_State *L ) {
  HttpClient *udata = checkHttpClient( L );
  delete udata;
  return 0;
}

static const
luaL_reg arken_net_HttpClient_metatable[] = {
  {"appendHeader",     arken_net_HttpClient_appendHeader},
  {"setVerbose",       arken_net_HttpClient_setVerbose},
  {"setSslVerifyPeer", arken_net_HttpClient_setSslVerifyPeer},
  {"setSslVerifyHost", arken_net_HttpClient_setSslVerifyHost},
  {"setSslVersion",    arken_net_HttpClient_setSslVersion},
  {"setUseSsl",        arken_net_HttpClient_setUseSsl},
  {"setTimeout",       arken_net_HttpClient_setTimeout},
  {"setBody",          arken_net_HttpClient_setBody},
  {"setCert",          arken_net_HttpClient_setCert},
  {"setCertKey",       arken_net_HttpClient_setCertKey},
  {"body",             arken_net_HttpClient_body},
  {"performGet",       arken_net_HttpClient_performGet},
  {"performPost",      arken_net_HttpClient_performPost},
  {"performPatch",     arken_net_HttpClient_performPatch},
  {"performPut",       arken_net_HttpClient_performPut},
  {"performDelete",    arken_net_HttpClient_performDelete},
  {"status",           arken_net_HttpClient_status},
  {"data",             arken_net_HttpClient_data},
  {"failure",          arken_net_HttpClient_failure},
  {"message",          arken_net_HttpClient_message},
  {"__gc",             arken_net_HttpClient_gc},
  {nullptr, nullptr}
};

void static
register_arken_net_HttpClient_metatable( lua_State *L ) {
  luaL_newmetatable(L,  "arken.net.HttpClient.metatable");
  luaL_register(L, nullptr, arken_net_HttpClient_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_net_HttpClient( lua_State *L ) {
    register_arken_net_HttpClient_metatable(L);
    register_arken_net_HttpClient(L);
    return 1;
  }
}
