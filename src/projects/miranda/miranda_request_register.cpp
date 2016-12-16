// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <CHttpParser>

static int
lua_CHttpParserInstanceMethodData( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  CHttpParser *req = (CHttpParser *) lua_touserdata(L, -1);
  char * data = req->data();
  lua_pushstring(L, data);
  delete[] data;
  return 1;
}

static int
lua_CHttpParserInstanceMethodField( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  CHttpParser *req = (CHttpParser *) lua_touserdata(L, -1);
  const char  *field = luaL_checkstring(L, 1);
  lua_pushstring(L, req->field(field));
  return 1;
}

static int
lua_CHttpParserInstanceMethodFragment( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  CHttpParser *req = (CHttpParser *) lua_touserdata(L, -1);
  if( req->fragment() == NULL ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, req->fragment());
  }
  return 1;
}

static int
lua_CHttpParserInstanceMethodHttpVersion( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  CHttpParser *req = (CHttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->httpVersion());
  return 1;
}

static int
lua_CHttpParserInstanceMethodRequestUri( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  CHttpParser *req = (CHttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestUri());
  return 1;
}

static int
lua_CHttpParserInstanceMethodRequestPath( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  CHttpParser *req = (CHttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestPath());
  return 1;
}

static int
lua_CHttpParserInstanceMethodRequestMethod( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  CHttpParser *req = (CHttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestMethod());
  return 1;
}

static int
lua_CHttpParserInstanceMethodQueryString( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  CHttpParser *req = (CHttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->queryString());
  return 1;
}

static int
lua_CHttpParserInstanceMethodHeaderDone( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  CHttpParser *req = (CHttpParser *) lua_touserdata(L, -1);
  lua_pushlstring(L, req->headerDone(), req->headerDoneLength());
  return 1;
}

void
miranda_request_register(lua_State * L) {
  static const 	luaL_reg Map[] = {
    {"data", lua_CHttpParserInstanceMethodData},
    {"field", lua_CHttpParserInstanceMethodField},
    {"fragment", lua_CHttpParserInstanceMethodFragment},
    {"headerDone", lua_CHttpParserInstanceMethodHeaderDone},
    {"httpVersion", lua_CHttpParserInstanceMethodHttpVersion},
    {"requestUri", lua_CHttpParserInstanceMethodRequestUri},
    {"requestMethod", lua_CHttpParserInstanceMethodRequestMethod},
    {"requestPath", lua_CHttpParserInstanceMethodRequestPath},
    {"queryString", lua_CHttpParserInstanceMethodQueryString},
    {NULL, NULL}
  };

  luaL_register(L, "request", Map);
}
