// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <QtCore>
#include <iostream>
#include <QByteArray>

using charon::net::HttpParser;

/**
 * checkHttpParser
 */

HttpParser *
checkHttpParser( lua_State *L ) {
  return *(HttpParser **) luaL_checkudata(L, 1, "HttpParser.metatable");
}

/**
 * ClassMethods
 */

static int
lua_HttpParserClassMethodNew( lua_State *L ) {
  size_t len;
  const char *data  = luaL_checklstring(L, 1, &len);
  HttpParser **ptr = (HttpParser **)lua_newuserdata(L, sizeof(HttpParser*));
  *ptr= new HttpParser(QByteArray(data, len));
  luaL_getmetatable(L, "HttpParser.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg HttpParserClassMethods[] = {
  {"new", lua_HttpParserClassMethodNew},
  {NULL, NULL}
};

void static
registerHttpParserClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "HttpParser");
  luaL_register(L, NULL, HttpParserClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
lua_HttpParserInstanceMethodData( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  lua_pushstring(L, udata->data());
  return 1;
}

static int
lua_HttpParserInstanceMethodField( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  const char  *field = luaL_checkstring(L, 2);
  lua_pushstring(L, udata->field(field));
  return 1;
}

static int
lua_HttpParserInstanceMethodFragment( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  if( udata->fragment() == NULL ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, udata->fragment());
  }
  return 1;
}

static int
lua_HttpParserInstanceMethodHttpVersion( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  lua_pushstring(L, udata->httpVersion());
  return 1;
}

static int
lua_HttpParserInstanceMethodRequestUri( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  lua_pushstring(L, udata->requestUri());
  return 1;
}

static int
lua_HttpParserInstanceMethodRequestPath( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  lua_pushstring(L, udata->requestPath());
  return 1;
}

static int
lua_HttpParserInstanceMethodRequestMethod( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  lua_pushstring(L, udata->requestMethod());
  return 1;
}

static int
lua_HttpParserInstanceMethodQueryString( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  lua_pushstring(L, udata->queryString());
  return 1;
}

static int
lua_HttpParserInstanceMethodHeaderDone( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  if( udata->headerDoneLength() == 0 ) {
    lua_pushnil(L);
  } else {
    lua_pushlstring(L, udata->headerDone(), udata->headerDoneLength() );
  }
  return 1;
}

static int
lua_HttpParserInstanceMethodHeaderDoneLength( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  lua_pushinteger(L, udata->headerDoneLength());
  return 1;
}

static int
lua_HttpParserInstanceMethodToJson( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  lua_pushstring(L, udata->toJson());
  return 1;
}

static int
lua_HttpParserInstanceMethodDestruct( lua_State *L ) {
  HttpParser *udata = checkHttpParser( L );
  delete udata;
  return 0;
}

static const
luaL_reg HttpParserInstanceMethods[] = {
  {"data", lua_HttpParserInstanceMethodData},
  {"field", lua_HttpParserInstanceMethodField},
  {"fragment", lua_HttpParserInstanceMethodFragment},
  {"headerDone", lua_HttpParserInstanceMethodHeaderDone},
  {"headerDoneLength", lua_HttpParserInstanceMethodHeaderDoneLength},
  {"httpVersion", lua_HttpParserInstanceMethodHttpVersion},
  {"requestUri", lua_HttpParserInstanceMethodRequestUri},
  {"requestMethod", lua_HttpParserInstanceMethodRequestMethod},
  {"requestPath", lua_HttpParserInstanceMethodRequestPath},
  {"queryString", lua_HttpParserInstanceMethodQueryString},
  {"toJson", lua_HttpParserInstanceMethodToJson},
  {"__gc", lua_HttpParserInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerHttpParserInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "HttpParser.metatable");
  luaL_register(L, NULL, HttpParserInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_charon_net_HttpParser( lua_State *L ) {
    registerHttpParserInstanceMethods(L);
    registerHttpParserClassMethods(L);
    return 1;
  }
}
