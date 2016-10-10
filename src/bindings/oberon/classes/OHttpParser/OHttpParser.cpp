#include <luajit-2.0/lua.hpp>
#include <QtCore>
#include <iostream>
#include <OHttpParser>
#include <QByteArray>

/**
 * checkOHttpParser
 */

OHttpParser *
checkOHttpParser( lua_State *L ) {
  return *(OHttpParser **) luaL_checkudata(L, 1, "OHttpParser.metatable");
}

/**
 * ClassMethods
 */

static int
lua_OHttpParserClassMethodNew( lua_State *L ) {
  size_t len;
  const char *data  = luaL_checklstring(L, 1, &len);
  OHttpParser **ptr = (OHttpParser **)lua_newuserdata(L, sizeof(OHttpParser*));
  *ptr= new OHttpParser(QByteArray(data));
  luaL_getmetatable(L, "OHttpParser.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg OHttpParserClassMethods[] = {
  {"new", lua_OHttpParserClassMethodNew},
  {NULL, NULL}
};

void static
registerOHttpParserClassMethods( lua_State *L ) {
  luaL_register(L, "OHttpParser", OHttpParserClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_OHttpParserInstanceMethodData( lua_State *L ) {
  OHttpParser *udata = checkOHttpParser( L );
  lua_pushstring(L, udata->data());
  return 1;
}

static int
lua_OHttpParserInstanceMethodField( lua_State *L ) {
  OHttpParser *udata = checkOHttpParser( L );
  const char  *field = luaL_checkstring(L, 2);
  lua_pushstring(L, udata->field(field));
  return 1;
}

static int
lua_OHttpParserInstanceMethodFragment( lua_State *L ) {
  OHttpParser *udata = checkOHttpParser( L );
  if( udata->fragment() == NULL ) {
    lua_pushnil(L);
  } else {
    lua_pushstring(L, udata->fragment());
  }
  return 1;
}

static int
lua_OHttpParserInstanceMethodHttpVersion( lua_State *L ) {
  OHttpParser *udata = checkOHttpParser( L );
  lua_pushstring(L, udata->httpVersion());
  return 1;
}

static int
lua_OHttpParserInstanceMethodRequestUri( lua_State *L ) {
  OHttpParser *udata = checkOHttpParser( L );
  lua_pushstring(L, udata->requestUri());
  return 1;
}

static int
lua_OHttpParserInstanceMethodRequestPath( lua_State *L ) {
  OHttpParser *udata = checkOHttpParser( L );
  lua_pushstring(L, udata->requestPath());
  return 1;
}

static int
lua_OHttpParserInstanceMethodRequestMethod( lua_State *L ) {
  OHttpParser *udata = checkOHttpParser( L );
  lua_pushstring(L, udata->requestMethod());
  return 1;
}

static int
lua_OHttpParserInstanceMethodQueryString( lua_State *L ) {
  OHttpParser *udata = checkOHttpParser( L );
  lua_pushstring(L, udata->queryString());
  return 1;
}

static int
lua_OHttpParserInstanceMethodHeaderDone( lua_State *L ) {
  OHttpParser *udata = checkOHttpParser( L );
  lua_pushstring(L, udata->headerDone());
  return 1;
}

static int
lua_OHttpParserInstanceMethodToJson( lua_State *L ) {
  OHttpParser *udata = checkOHttpParser( L );
  lua_pushstring(L, udata->toJson());
  return 1;
}

static int
lua_OHttpParserInstanceMethodDestruct( lua_State *L ) {
  OHttpParser *udata = checkOHttpParser( L );
  delete udata;
  return 0;
}

static const
luaL_reg OHttpParserInstanceMethods[] = {
  {"data", lua_OHttpParserInstanceMethodData},
  {"field", lua_OHttpParserInstanceMethodField},
  {"fragment", lua_OHttpParserInstanceMethodFragment},
  {"headerDone", lua_OHttpParserInstanceMethodHeaderDone},
  {"httpVersion", lua_OHttpParserInstanceMethodHttpVersion},
  {"requestUri", lua_OHttpParserInstanceMethodRequestUri},
  {"requestMethod", lua_OHttpParserInstanceMethodRequestMethod},
  {"requestPath", lua_OHttpParserInstanceMethodRequestPath},
  {"queryString", lua_OHttpParserInstanceMethodQueryString},
  {"toJson", lua_OHttpParserInstanceMethodToJson},
  {"__gc", lua_OHttpParserInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerOHttpParserInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "OHttpParser.metatable");
  luaL_register(L, NULL, OHttpParserInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_OHttpParser( lua_State *L ) {
    registerOHttpParserClassMethods(L);
    registerOHttpParserInstanceMethods(L);
    return 1;
  }
}
