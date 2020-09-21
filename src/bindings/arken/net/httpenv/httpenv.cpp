// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <QtCore>
#include <iostream>
#include <QByteArray>

using arken::net::HttpEnv;

/**
 * checkHttpEnv
 */

HttpEnv *
checkHttpEnv( lua_State *L ) {
  return *(HttpEnv **) luaL_checkudata(L, 1, "HttpEnv.metatable");
}

/**
 * ClassMethods
 */

static int
arken_HttpEnvClassMethodNew( lua_State *L ) {
  size_t len;
  const char *data  = luaL_checklstring(L, 1, &len);
  HttpEnv **ptr = (HttpEnv **)lua_newuserdata(L, sizeof(HttpEnv*));
  *ptr= new HttpEnv(data, len);
  luaL_getmetatable(L, "HttpEnv.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg HttpEnvClassMethods[] = {
  {"new", arken_HttpEnvClassMethodNew},
  {NULL, NULL}
};

void static
registerHttpEnvClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "HttpEnv");
  luaL_register(L, NULL, HttpEnvClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_HttpEnvInstanceMethodData( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushstring(L, udata->data());
  return 1;
}

static int
arken_HttpEnvInstanceMethodField( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  const char  *field = luaL_checkstring(L, 2);
  lua_pushstring(L, udata->field(field));
  return 1;
}

static int
arken_HttpEnvInstanceMethodSetField( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  const char * field = luaL_checkstring(L, 2);
  const char * value = luaL_checkstring(L, 3);
  udata->setField(field, value);
  return 1;
}

static int
arken_HttpEnvInstanceMethodSetFragment( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t length;
  const char * raw = luaL_checklstring(L, 2, &length);
  char * tmp = new char[length+1];
  for(size_t i=0; i < length; i++) {
    tmp[i] = raw[i];
  }
  tmp[length] = '\0';
  udata->setFragment(tmp);
  return 1;
}

static int
arken_HttpEnvInstanceMethodSetQueryString( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t length;
  const char * raw = luaL_checklstring(L, 2, &length);
  char * tmp = new char[length+1];
  for(size_t i=0; i < length; i++) {
    tmp[i] = raw[i];
  }
  tmp[length] = '\0';
  udata->setQueryString(tmp);
  return 1;
}

static int
arken_HttpEnvInstanceMethodSetRequestPath( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t length;
  const char * raw = luaL_checklstring(L, 2, &length);
  char * tmp = new char[length+1];
  for(size_t i=0; i < length; i++) {
    tmp[i] = raw[i];
  }
  tmp[length] = '\0';
  udata->setRequestPath(tmp);
  return 1;
}

static int
arken_HttpEnvInstanceMethodSetRequestMethod( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t length;
  const char * raw = luaL_checklstring(L, 2, &length);
  char * tmp = new char[length+1];
  for(size_t i=0; i < length; i++) {
    tmp[i] = raw[i];
  }
  tmp[length] = '\0';
  udata->setRequestMethod(tmp);
  return 1;
}

static int
arken_HttpEnvInstanceMethodSetRequestUri( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t length;
  const char * raw = luaL_checklstring(L, 2, &length);
  char * tmp = new char[length+1];
  for(size_t i=0; i < length; i++) {
    tmp[i] = raw[i];
  }
  tmp[length] = '\0';
  udata->setRequestUri(tmp);
  return 1;
}

static int
arken_HttpEnvInstanceMethodSetHttpVersion( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  size_t length;
  const char * raw = luaL_checklstring(L, 2, &length);
  char * tmp = new char[length+1];
  for(size_t i=0; i < length; i++) {
    tmp[i] = raw[i];
  }
  tmp[length] = '\0';
  udata->setHttpVersion(tmp);
  return 1;
}

static int
arken_HttpEnvInstanceMethodSetHeaderDone( lua_State *L ) {
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
arken_HttpEnvInstanceMethodFragment( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  if( udata->fragment() == NULL ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, udata->fragment());
  }
  return 1;
}

static int
arken_HttpEnvInstanceMethodHttpVersion( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushstring(L, udata->httpVersion());
  return 1;
}

static int
arken_HttpEnvInstanceMethodRequestUri( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushstring(L, udata->requestUri());
  return 1;
}

static int
arken_HttpEnvInstanceMethodRequestPath( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushstring(L, udata->requestPath());
  return 1;
}

static int
arken_HttpEnvInstanceMethodRequestMethod( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushstring(L, udata->requestMethod());
  return 1;
}

static int
arken_HttpEnvInstanceMethodQueryString( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushstring(L, udata->queryString());
  return 1;
}

static int
arken_HttpEnvInstanceMethodHeaderDone( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  if( udata->headerDoneLength() == 0 ) {
    lua_pushnil(L);
  } else {
    lua_pushlstring(L, udata->headerDone(), udata->headerDoneLength() );
  }
  return 1;
}

static int
arken_HttpEnvInstanceMethodHeaderDoneLength( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  lua_pushinteger(L, udata->headerDoneLength());
  return 1;
}

static int
arken_HttpEnvInstanceMethodDestruct( lua_State *L ) {
  HttpEnv *udata = checkHttpEnv( L );
  delete udata;
  return 0;
}

static const
luaL_reg HttpEnvInstanceMethods[] = {
  {"data", arken_HttpEnvInstanceMethodData},
  {"field", arken_HttpEnvInstanceMethodField},
  {"setField", arken_HttpEnvInstanceMethodSetField},
  {"setFragment", arken_HttpEnvInstanceMethodSetFragment},
  {"setHeaderDone", arken_HttpEnvInstanceMethodSetHeaderDone},
  {"setQueryString", arken_HttpEnvInstanceMethodSetQueryString},
  {"setRequestPath", arken_HttpEnvInstanceMethodSetRequestPath},
  {"setRequestMethod", arken_HttpEnvInstanceMethodSetRequestMethod},
  {"setRequestUri", arken_HttpEnvInstanceMethodSetRequestUri},
  {"setHttpVersion", arken_HttpEnvInstanceMethodSetHttpVersion},
  {"fragment", arken_HttpEnvInstanceMethodFragment},
  {"headerDone", arken_HttpEnvInstanceMethodHeaderDone},
  {"headerDoneLength", arken_HttpEnvInstanceMethodHeaderDoneLength},
  {"httpVersion", arken_HttpEnvInstanceMethodHttpVersion},
  {"requestUri", arken_HttpEnvInstanceMethodRequestUri},
  {"requestMethod", arken_HttpEnvInstanceMethodRequestMethod},
  {"requestPath", arken_HttpEnvInstanceMethodRequestPath},
  {"queryString", arken_HttpEnvInstanceMethodQueryString},
  {"__gc", arken_HttpEnvInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerHttpEnvInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "HttpEnv.metatable");
  luaL_register(L, NULL, HttpEnvInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_net_HttpEnv( lua_State *L ) {
    registerHttpEnvInstanceMethods(L);
    registerHttpEnvClassMethods(L);
    return 1;
  }

  int
  luaopen_arken_net_HttpEnv( lua_State *L ) {
    registerHttpEnvInstanceMethods(L);
    registerHttpEnvClassMethods(L);
    return 1;
  }
}
