#include <luajit-2.0/lua.hpp>
#include <OHttpRequest>

static int
lua_OHttpRequestInstanceMethodField( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpRequest *req = *(OHttpRequest **) lua_touserdata(L, -1);
  const char  *field = luaL_checkstring(L, 1);
  lua_pushstring(L, req->field(field));
  return 1;
}

static int
lua_OHttpRequestInstanceMethodFragment( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpRequest *req = *(OHttpRequest **) lua_touserdata(L, -1);
  if( req->fragment() == NULL ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, req->fragment());
  }
  return 1;
}

static int
lua_OHttpRequestInstanceMethodHttpVersion( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpRequest *req = *(OHttpRequest **) lua_touserdata(L, -1);
  lua_pushstring(L, req->httpVersion());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodRequestUri( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpRequest *req = *(OHttpRequest **) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestUri());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodRequestPath( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpRequest *req = *(OHttpRequest **) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestPath());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodRequestMethod( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpRequest *req = *(OHttpRequest **) lua_touserdata(L, -1);
  lua_pushstring(L, req->requestMethod());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodQueryString( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpRequest *req = *(OHttpRequest **) lua_touserdata(L, -1);
  lua_pushstring(L, req->queryString());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodHeaderDone( lua_State *L ) {
  lua_getglobal( L, "__http_request");
  OHttpRequest *req = *(OHttpRequest **) lua_touserdata(L, -1);
  lua_pushstring(L, req->headerDone());
  return 1;
}

void
miranda_request_register(lua_State * L) {
  static const 	luaL_reg Map[] = {
    {"field", lua_OHttpRequestInstanceMethodField},
    {"fragment", lua_OHttpRequestInstanceMethodFragment},
    {"headerDone", lua_OHttpRequestInstanceMethodHeaderDone},
    {"httpVersion", lua_OHttpRequestInstanceMethodHttpVersion},
    {"requestUri", lua_OHttpRequestInstanceMethodRequestUri},
    {"requestMethod", lua_OHttpRequestInstanceMethodRequestMethod},
    {"requestPath", lua_OHttpRequestInstanceMethodRequestPath},
    {"queryString", lua_OHttpRequestInstanceMethodQueryString},
    {NULL, NULL}
  };

  luaL_register(L, "request", Map);
}
