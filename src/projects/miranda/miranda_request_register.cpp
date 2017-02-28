// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>

using charon::net::HttpParser;

static int
lua_HttpParserInstanceMethodData( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  HttpParser *req = (HttpParser *) lua_touserdata(L, -1);
  char * data = req->data();
  lua_pushstring(L, data);
  delete[] data;
  return 1;
}

static int
lua_HttpParserInstanceMethodField( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  HttpParser *req = (HttpParser *) lua_touserdata(L, -1);
  const char  *field = luaL_checkstring(L, 1);
  lua_pushstring(L, req->field(field));
  return 1;
}

static int
lua_HttpParserInstanceMethodFragment( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  HttpParser *req = (HttpParser *) lua_touserdata(L, -1);
  if( req->fragment() == NULL ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, req->fragment());
  }
  return 1;
}

static int
lua_HttpParserInstanceMethodHttpVersion( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  HttpParser *req = (HttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->httpVersion());
  return 1;
}

static int
lua_HttpParserInstanceMethodRequestUri( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  HttpParser *req = (HttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestUri());
  return 1;
}

static int
lua_HttpParserInstanceMethodRequestPath( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  HttpParser *req = (HttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestPath());
  return 1;
}

static int
lua_HttpParserInstanceMethodRequestMethod( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  HttpParser *req = (HttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestMethod());
  return 1;
}

static int
lua_HttpParserInstanceMethodQueryString( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  HttpParser *req = (HttpParser *) lua_touserdata(L, -1);
  lua_pushstring(L, req->queryString());
  return 1;
}

static int
lua_HttpParserInstanceMethodHeaderDone( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  HttpParser *req = (HttpParser *) lua_touserdata(L, -1);
  lua_pushlstring(L, req->headerDone(), req->headerDoneLength());
  return 1;
}

void
miranda_request_register(lua_State * L) {
  static const 	luaL_reg Map[] = {
    {"data", lua_HttpParserInstanceMethodData},
    {"field", lua_HttpParserInstanceMethodField},
    {"fragment", lua_HttpParserInstanceMethodFragment},
    {"headerDone", lua_HttpParserInstanceMethodHeaderDone},
    {"httpVersion", lua_HttpParserInstanceMethodHttpVersion},
    {"requestUri", lua_HttpParserInstanceMethodRequestUri},
    {"requestMethod", lua_HttpParserInstanceMethodRequestMethod},
    {"requestPath", lua_HttpParserInstanceMethodRequestPath},
    {"queryString", lua_HttpParserInstanceMethodQueryString},
    {NULL, NULL}
  };

  luaL_register(L, "request", Map);
}
