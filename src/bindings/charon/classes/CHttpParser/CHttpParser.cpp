// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>
#include <CHttpParser>
#include <QByteArray>

/**
 * checkCHttpParser
 */

CHttpParser *
checkCHttpParser( lua_State *L ) {
  return *(CHttpParser **) luaL_checkudata(L, 1, "CHttpParser.metatable");
}

/**
 * ClassMethods
 */

static int
lua_CHttpParserClassMethodNew( lua_State *L ) {
  size_t len;
  const char *data  = luaL_checklstring(L, 1, &len);
  CHttpParser **ptr = (CHttpParser **)lua_newuserdata(L, sizeof(CHttpParser*));
  *ptr= new CHttpParser(QByteArray(data, len));
  luaL_getmetatable(L, "CHttpParser.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg CHttpParserClassMethods[] = {
  {"new", lua_CHttpParserClassMethodNew},
  {NULL, NULL}
};

void static
registerCHttpParserClassMethods( lua_State *L ) {
  luaL_register(L, "CHttpParser", CHttpParserClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_CHttpParserInstanceMethodData( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  lua_pushstring(L, udata->data());
  return 1;
}

static int
lua_CHttpParserInstanceMethodField( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  const char  *field = luaL_checkstring(L, 2);
  lua_pushstring(L, udata->field(field));
  return 1;
}

static int
lua_CHttpParserInstanceMethodFragment( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  if( udata->fragment() == NULL ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, udata->fragment());
  }
  return 1;
}

static int
lua_CHttpParserInstanceMethodHttpVersion( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  lua_pushstring(L, udata->httpVersion());
  return 1;
}

static int
lua_CHttpParserInstanceMethodRequestUri( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  lua_pushstring(L, udata->requestUri());
  return 1;
}

static int
lua_CHttpParserInstanceMethodRequestPath( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  lua_pushstring(L, udata->requestPath());
  return 1;
}

static int
lua_CHttpParserInstanceMethodRequestMethod( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  lua_pushstring(L, udata->requestMethod());
  return 1;
}

static int
lua_CHttpParserInstanceMethodQueryString( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  lua_pushstring(L, udata->queryString());
  return 1;
}

static int
lua_CHttpParserInstanceMethodHeaderDone( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  lua_pushlstring(L, udata->headerDone(), udata->headerDoneLength() );
  return 1;
}

static int
lua_CHttpParserInstanceMethodHeaderDoneLength( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  lua_pushinteger(L, udata->headerDoneLength());
  return 1;
}

static int
lua_CHttpParserInstanceMethodToJson( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  lua_pushstring(L, udata->toJson());
  return 1;
}

static int
lua_CHttpParserInstanceMethodDestruct( lua_State *L ) {
  CHttpParser *udata = checkCHttpParser( L );
  delete udata;
  return 0;
}

static const
luaL_reg CHttpParserInstanceMethods[] = {
  {"data", lua_CHttpParserInstanceMethodData},
  {"field", lua_CHttpParserInstanceMethodField},
  {"fragment", lua_CHttpParserInstanceMethodFragment},
  {"headerDone", lua_CHttpParserInstanceMethodHeaderDone},
  {"headerDoneLength", lua_CHttpParserInstanceMethodHeaderDoneLength},
  {"httpVersion", lua_CHttpParserInstanceMethodHttpVersion},
  {"requestUri", lua_CHttpParserInstanceMethodRequestUri},
  {"requestMethod", lua_CHttpParserInstanceMethodRequestMethod},
  {"requestPath", lua_CHttpParserInstanceMethodRequestPath},
  {"queryString", lua_CHttpParserInstanceMethodQueryString},
  {"toJson", lua_CHttpParserInstanceMethodToJson},
  {"__gc", lua_CHttpParserInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerCHttpParserInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "CHttpParser.metatable");
  luaL_register(L, NULL, CHttpParserInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_CHttpParser( lua_State *L ) {
    registerCHttpParserClassMethods(L);
    registerCHttpParserInstanceMethods(L);
    return 1;
  }
}
