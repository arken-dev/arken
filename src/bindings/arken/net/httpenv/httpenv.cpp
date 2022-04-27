// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/net/httpenv.h>

using arken::net::HttpEnv;

/**
 * checkHttpEnv
 */

HttpEnv *
checkHttpEnv( lua_State *L ) {
  return *(HttpEnv **) luaL_checkudata(L, 1, "arken.net.HttpEnv.metatable");
}

/**
 * ClassMethods
 */

static int
arken_net_HttpEnv_new( lua_State *L ) {
  size_t len;
  const char *data  = luaL_checklstring(L, 1, &len);
  HttpEnv **ptr = (HttpEnv **)lua_newuserdata(L, sizeof(HttpEnv*));
  *ptr= new HttpEnv(data, len);
  luaL_getmetatable(L, "arken.net.HttpEnv.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_net_HttpEnv[] = {
  {"new", arken_net_HttpEnv_new},
  {nullptr, nullptr}
};

void static
register_arken_net_HttpEnv( lua_State *L ) {
  luaL_newmetatable(L,  "arken.net.HttpEnv");
  luaL_register(L, nullptr, arken_net_HttpEnv);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_net_HttpEnv_data( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushlstring(L, udata->data(), udata->len());
  return 1;
}

static int
arken_net_HttpEnv_field( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  const char  *field = luaL_checkstring(L, 2);
  lua_pushstring(L, udata->field(field));
  return 1;
}

static int
arken_net_HttpEnv_setField( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  const char * field = luaL_checkstring(L, 2);
  const char * value = luaL_checkstring(L, 3);
  udata->setField(field, value);
  return 1;
}

static int
arken_net_HttpEnv_setFragment( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t len;
  const char * at = luaL_checklstring(L, 2, &len);
  udata->setFragment(at, len);
  return 1;
}

static int
arken_net_HttpEnv_setQueryString( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t len;
  const char * value = luaL_checklstring(L, 2, &len);
  udata->setQueryString(value, len);
  return 1;
}

static int
arken_net_HttpEnv_setRequestPath( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t len;
  const char * value = luaL_checklstring(L, 2, &len);
  udata->setRequestPath(value, len);
  return 1;
}

static int
arken_net_HttpEnv_setRequestMethod( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t len;
  const char * at = luaL_checklstring(L, 2, &len);
  udata->setRequestMethod(at, len);
  return 1;
}

static int
arken_net_HttpEnv_setRequestUri( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t len;
  const char * at = luaL_checklstring(L, 2, &len);
  udata->setRequestUri(at, len);
  return 1;
}

static int
arken_net_HttpEnv_setHttpVersion( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t len;
  const char * at = luaL_checklstring(L, 2, &len);
  udata->setHttpVersion(at, len);
  return 1;
}

static int
arken_net_HttpEnv_setHeaderDone( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t length;
  const char * raw = luaL_checklstring(L, 2, &length);
  char * tmp = new char[length+1];
  for(size_t i=0; i < length; i++) {
    tmp[i] = raw[i];
  }
  tmp[length] = '\0';
  udata->setHeaderDone(tmp, length);
  return 1;
}

static int
arken_net_HttpEnv_fragment( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushstring(L, udata->fragment());
  return 1;
}

static int
arken_net_HttpEnv_httpVersion( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushstring(L, udata->httpVersion());
  return 1;
}

static int
arken_net_HttpEnv_requestUri( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushstring(L, udata->requestUri());
  return 1;
}

static int
arken_net_HttpEnv_requestPath( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  string data = udata->requestPath();
  lua_pushlstring(L, data, data.size());
  return 1;
}

static int
arken_net_HttpEnv_requestMethod( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushstring(L, udata->requestMethod());
  return 1;
}

static int
arken_net_HttpEnv_queryString( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushstring(L, udata->queryString());
  return 1;
}

static int
arken_net_HttpEnv_headerDone( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  string  header = udata->headerDone();
  if( header.size() == 0 ) {
    lua_pushnil(L);
  } else {
    lua_pushlstring(L, header.data(), header.size() );
  }
  return 1;
}

static int
arken_net_HttpEnv_gc( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  delete udata;
  return 0;
}

static const
luaL_reg arken_net_HttpEnv_metatable[] = {
  {"data",             arken_net_HttpEnv_data},
  {"field",            arken_net_HttpEnv_field},
  {"setField",         arken_net_HttpEnv_setField},
  {"setFragment",      arken_net_HttpEnv_setFragment},
  {"setHeaderDone",    arken_net_HttpEnv_setHeaderDone},
  {"setQueryString",   arken_net_HttpEnv_setQueryString},
  {"setRequestPath",   arken_net_HttpEnv_setRequestPath},
  {"setRequestMethod", arken_net_HttpEnv_setRequestMethod},
  {"setRequestUri",    arken_net_HttpEnv_setRequestUri},
  {"setHttpVersion",   arken_net_HttpEnv_setHttpVersion},
  {"fragment",         arken_net_HttpEnv_fragment},
  {"headerDone",       arken_net_HttpEnv_headerDone},
  {"httpVersion",      arken_net_HttpEnv_httpVersion},
  {"requestUri",       arken_net_HttpEnv_requestUri},
  {"requestMethod",    arken_net_HttpEnv_requestMethod},
  {"requestPath",      arken_net_HttpEnv_requestPath},
  {"queryString",      arken_net_HttpEnv_queryString},
  {"__gc",             arken_net_HttpEnv_gc},
  {nullptr, nullptr}
};

void static
register_arken_net_HttpEnv_metatable( lua_State *L ) {
  luaL_newmetatable(L,  "arken.net.HttpEnv.metatable");
  luaL_register(L, nullptr, arken_net_HttpEnv_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_net_HttpEnv( lua_State *L ) {
    register_arken_net_HttpEnv_metatable(L);
    register_arken_net_HttpEnv(L);
    return 1;
  }

}
