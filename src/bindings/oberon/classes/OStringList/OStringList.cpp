#include <luajit-2.0/lua.hpp>
#include <QtCore>
#include <OStringList>

/**
 * checkOStringList
 */

OStringList *
checkOStringList( lua_State *L ) {
  return *(OStringList **) luaL_checkudata(L, 1, "OStringList.metatable");
}

/**
 * ClassMethods
 */

static int
lua_OStringListClassMethodNew( lua_State *L ) {
  OStringList **ptr = (OStringList **)lua_newuserdata(L, sizeof(OStringList*));
  *ptr= new OStringList(OStringList());
  luaL_getmetatable(L, "OStringList.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg OStringListClassMethods[] = {
  {"new", lua_OStringListClassMethodNew},
  {NULL, NULL}
};

void static
registerOStringListClassMethods( lua_State *L ) {
  luaL_register(L, "OStringList", OStringListClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_OStringListInstanceMethodDestruct( lua_State *L ) {
  OStringList *udata = checkOStringList( L );
  delete udata;
  return 0;
}

static int
lua_OStringListInstanceMethodAt( lua_State *L ) {
  OStringList * udata  = checkOStringList( L );
  int pos = luaL_checkint(L, 2);
  lua_pushstring(L, udata->at(pos-1));
  return 1;
}

static int
lua_OStringListInstanceMethodFirst( lua_State *L ) {
  OStringList * udata  = checkOStringList( L );
  lua_pushstring(L, udata->first());
  return 1;
}

static int
lua_OStringListInstanceMethodLast( lua_State *L ) {
  OStringList * udata  = checkOStringList( L );
  lua_pushstring(L, udata->last());
  return 1;
}

static int
lua_OStringListInstanceMethodReplace( lua_State *L ) {
  OStringList * udata  = checkOStringList( L );
  int pos = luaL_checkint(L, 2);
  const char * value = luaL_checkstring(L, 3);
  udata->replace(pos, value);
  return 0;
}

static int
lua_OStringListInstanceMethodJoin( lua_State *L ) {
  OStringList * udata     = checkOStringList( L );
  const char  * separator = luaL_checkstring(L, 2);
  char * result = udata->join(separator);
  lua_pushstring(L, result);
  delete[] result;
  return 1;
}


static int
lua_OStringListInstanceMethodAppend( lua_State *L ) {
  OStringList * udata  = checkOStringList( L );
  const char * value = luaL_checkstring(L, 2);
  udata->append(value);
  lua_pushvalue(L, -2);
  return 1;
}

static int
lua_OStringListInstanceMethodSize( lua_State *L ) {
  OStringList * udata  = checkOStringList( L );
  lua_pushinteger(L, udata->size());
  return 1;
}

static const
luaL_reg OStringListInstanceMethods[] = {
  {"append", lua_OStringListInstanceMethodAppend},
  {"at", lua_OStringListInstanceMethodAt},
  {"first", lua_OStringListInstanceMethodFirst},
  {"last", lua_OStringListInstanceMethodLast},
  {"join", lua_OStringListInstanceMethodJoin},
  {"size", lua_OStringListInstanceMethodSize},
  {"replace", lua_OStringListInstanceMethodReplace},
  {"__gc", lua_OStringListInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerOStringListInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "OStringList.metatable");
  luaL_register(L, NULL, OStringListInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_OStringList( lua_State *L ) {
    registerOStringListClassMethods(L);
    registerOStringListInstanceMethods(L);
    return 1;
  }
}
