// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <OHttpParser>

static int
lua_OHttpParserInstanceMethodData( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpParser *req = (OHttpParser *) lua_touserdata(L, -1);
  char * data = req->data();
  lua_pushstring(L, data);
  delete[] data;
  return 1;
}

static int
lua_OHttpParserInstanceMethodField( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpParser *req = (OHttpParser *) lua_touserdata(L, -1);
  const char  *field = luaL_checkstring(L, 1);
  lua_pushstring(L, req->field(field));
  return 1;
}

static int
lua_OHttpParserInstanceMethodFragment( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpParser *req = (OHttpParser *) lua_touserdata(L, -1);
  if( req->fragment() == NULL ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, req->fragment());
  }
  return 1;
}

static int
lua_OHttpParserInstanceMethodHttpVersion( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpParser *req = (OHttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->httpVersion());
  return 1;
}

static int
lua_OHttpParserInstanceMethodRequestUri( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpParser *req = (OHttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestUri());
  return 1;
}

static int
lua_OHttpParserInstanceMethodRequestPath( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpParser *req = (OHttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestPath());
  return 1;
}

static int
lua_OHttpParserInstanceMethodRequestMethod( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpParser *req = (OHttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestMethod());
  return 1;
}

static int
lua_OHttpParserInstanceMethodQueryString( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpParser *req = (OHttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->queryString());
  return 1;
}

static int
lua_OHttpParserInstanceMethodHeaderDone( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpParser *req = (OHttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->headerDone());
  return 1;
}

void
miranda_request_register(lua_State * L) {
  static const 	luaL_reg Map[] = {
    {"data", lua_OHttpParserInstanceMethodData},
    {"field", lua_OHttpParserInstanceMethodField},
    {"fragment", lua_OHttpParserInstanceMethodFragment},
    {"headerDone", lua_OHttpParserInstanceMethodHeaderDone},
    {"httpVersion", lua_OHttpParserInstanceMethodHttpVersion},
    {"requestUri", lua_OHttpParserInstanceMethodRequestUri},
    {"requestMethod", lua_OHttpParserInstanceMethodRequestMethod},
    {"requestPath", lua_OHttpParserInstanceMethodRequestPath},
    {"queryString", lua_OHttpParserInstanceMethodQueryString},
    {NULL, NULL}
  };

  luaL_register(L, "request", Map);
}
