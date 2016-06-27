#include <luajit-2.0/lua.hpp>
#include <QtCore>
#include <iostream>
#include <OHttpRequest>
#include <QByteArray>

/**
 * checkOHttpRequest
 */

OHttpRequest *
checkOHttpRequest( lua_State *L ) {
  return *(OHttpRequest **) luaL_checkudata(L, 1, "OHttpRequest.metatable");
}

/**
 * ClassMethods
 */

static int
lua_OHttpRequestClassMethodNew( lua_State *L ) {
  size_t len;
  const char *data  = luaL_checklstring(L, 1, &len);
  OHttpRequest **ptr = (OHttpRequest **)lua_newuserdata(L, sizeof(OHttpRequest*));
  *ptr= new OHttpRequest(QByteArray(data));
  luaL_getmetatable(L, "OHttpRequest.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg OHttpRequestClassMethods[] = {
  {"new", lua_OHttpRequestClassMethodNew},
  {NULL, NULL}
};

void static
registerOHttpRequestClassMethods( lua_State *L ) {
  luaL_register(L, "OHttpRequest", OHttpRequestClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_OHttpRequestInstanceMethodField( lua_State *L ) {
  OHttpRequest *udata = checkOHttpRequest( L );
  const char  *field = luaL_checkstring(L, 2);
  lua_pushstring(L, udata->field(field));
  return 1;
}

static int
lua_OHttpRequestInstanceMethodFragment( lua_State *L ) {
  OHttpRequest *udata = checkOHttpRequest( L );
  if( udata->fragment() == NULL ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, udata->fragment());
  }
  return 1;
}

static int
lua_OHttpRequestInstanceMethodHttpVersion( lua_State *L ) {
  OHttpRequest *udata = checkOHttpRequest( L );
  lua_pushstring(L, udata->httpVersion());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodRequestUri( lua_State *L ) {
  OHttpRequest *udata = checkOHttpRequest( L );
  lua_pushstring(L, udata->requestUri());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodRequestPath( lua_State *L ) {
  OHttpRequest *udata = checkOHttpRequest( L );
  lua_pushstring(L, udata->requestPath());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodRequestMethod( lua_State *L ) {
  OHttpRequest *udata = checkOHttpRequest( L );
  lua_pushstring(L, udata->requestMethod());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodQueryString( lua_State *L ) {
  OHttpRequest *udata = checkOHttpRequest( L );
  lua_pushstring(L, udata->queryString());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodHeaderDone( lua_State *L ) {
  OHttpRequest *udata = checkOHttpRequest( L );
  lua_pushstring(L, udata->headerDone());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodToJson( lua_State *L ) {
  OHttpRequest *udata = checkOHttpRequest( L );
  lua_pushstring(L, udata->toJson());
  return 1;
}

static int
lua_OHttpRequestInstanceMethodDestruct( lua_State *L ) {
  OHttpRequest *udata = checkOHttpRequest( L );
  delete udata;
  return 0;
}

static const
luaL_reg OHttpRequestInstanceMethods[] = {
  {"field", lua_OHttpRequestInstanceMethodField},
  {"fragment", lua_OHttpRequestInstanceMethodFragment},
  {"headerDone", lua_OHttpRequestInstanceMethodHeaderDone},
  {"httpVersion", lua_OHttpRequestInstanceMethodHttpVersion},
  {"requestUri", lua_OHttpRequestInstanceMethodRequestUri},
  {"requestMethod", lua_OHttpRequestInstanceMethodRequestMethod},
  {"requestPath", lua_OHttpRequestInstanceMethodRequestPath},
  {"queryString", lua_OHttpRequestInstanceMethodQueryString},
  {"toJson", lua_OHttpRequestInstanceMethodToJson},
  {"__gc", lua_OHttpRequestInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerOHttpRequestInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "OHttpRequest.metatable");
  luaL_register(L, NULL, OHttpRequestInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_OHttpRequest( lua_State *L ) {
    registerOHttpRequestClassMethods(L);
    registerOHttpRequestInstanceMethods(L);
    return 1;
  }
}
