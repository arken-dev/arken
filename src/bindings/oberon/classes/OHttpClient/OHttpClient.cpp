#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>
#include <OHttpClient>
#include <QByteArray>

/**
 * checkOHttpClient
 */

OHttpClient *
checkOHttpClient( lua_State *L ) {
  return *(OHttpClient **) luaL_checkudata(L, 1, "OHttpClient.metatable");
}

/**
 * ClassMethods
 */

static int
lua_OHttpClientClassMethodNew( lua_State *L ) {
  const char * url = luaL_checkstring(L, 1);
  OHttpClient **ptr = (OHttpClient **)lua_newuserdata(L, sizeof(OHttpClient*));
  *ptr = new OHttpClient(url);
  luaL_getmetatable(L, "OHttpClient.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg OHttpClientClassMethods[] = {
  {"new", lua_OHttpClientClassMethodNew},
  {NULL, NULL}
};

void static
registerOHttpClientClassMethods( lua_State *L ) {
  luaL_register(L, "OHttpClient", OHttpClientClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_OHttpClientInstanceMethodAppendHeader( lua_State *L ) {
  OHttpClient *udata  = checkOHttpClient( L );
  const char * header = luaL_checkstring(L, 2);
  udata->appendHeader(header);
  return 0;
}

static int
lua_OHttpClientInstanceMethodSetVerbose( lua_State *L ) {
  OHttpClient *udata  = checkOHttpClient( L );
  bool verbose = lua_toboolean(L, 2);
  udata->setVerbose(verbose);
  return 0;
}

static int
lua_OHttpClientInstanceMethodVerbose( lua_State *L ) {
  OHttpClient *udata  = checkOHttpClient( L );
  bool verbose = udata->verbose();
  lua_pushinteger(L, verbose);
  return 1;
}

static int
lua_OHttpClientInstanceMethodSetBody( lua_State *L ) {
  OHttpClient *udata = checkOHttpClient( L );
  const char * body  = luaL_checkstring(L, 2);
  udata->setBody(body);
  return 0;
}

static int
lua_OHttpClientInstanceMethodBody( lua_State *L ) {
  OHttpClient *udata = checkOHttpClient( L );
  const char * body = udata->body();
  lua_pushstring(L, body);
  return 1;
}

static int
lua_OHttpClientInstanceMethodPerformGet( lua_State *L ) {
  OHttpClient *udata = checkOHttpClient( L );
  char * get = udata->performGet();
  lua_pushstring(L, get);
  delete[] get;
  return 1;
}

static int
lua_OHttpClientInstanceMethodPerformPost( lua_State *L ) {
  OHttpClient *udata = checkOHttpClient( L );
  char * post = udata->performPost();
  lua_pushstring(L, post);
  delete[] post;
  return 1;
}

static int
lua_OHttpClientInstanceMethodPerformPut( lua_State *L ) {
  OHttpClient *udata = checkOHttpClient( L );
  char * put = udata->performPut();
  lua_pushstring(L, put);
  delete[] put;
  return 1;
}

static int
lua_OHttpClientInstanceMethodPerformDelete( lua_State *L ) {
  OHttpClient *udata = checkOHttpClient( L );
  char * del = udata->performDelete();
  lua_pushstring(L, del);
  delete[] del;
  return 0;
}

static int
lua_OHttpClientInstanceMethodDestruct( lua_State *L ) {
  OHttpClient *udata = checkOHttpClient( L );
  delete udata;
  return 0;
}

static const
luaL_reg OHttpClientInstanceMethods[] = {
  {"appendHeader", lua_OHttpClientInstanceMethodAppendHeader},
  {"setVerbose", lua_OHttpClientInstanceMethodSetVerbose},
  {"verbose", lua_OHttpClientInstanceMethodVerbose},
  {"setBody", lua_OHttpClientInstanceMethodSetBody},
  {"body", lua_OHttpClientInstanceMethodBody},
  {"performGet", lua_OHttpClientInstanceMethodPerformGet},
  {"performPost", lua_OHttpClientInstanceMethodPerformPost},
  {"performPut", lua_OHttpClientInstanceMethodPerformPut},
  {"performDelete", lua_OHttpClientInstanceMethodPerformDelete},
  {"__gc", lua_OHttpClientInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerOHttpClientInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "OHttpClient.metatable");
  luaL_register(L, NULL, OHttpClientInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_OHttpClient( lua_State *L ) {
    registerOHttpClientClassMethods(L);
    registerOHttpClientInstanceMethods(L);
    return 1;
  }
}
