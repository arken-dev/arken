#include <lua/lua.hpp>
#include <QtCore>
#include <OByteArray>

/**
 * checkOByteArray
 */

OByteArray *
checkOByteArray( lua_State *L ) {
  return *(OByteArray **) luaL_checkudata(L, 1, "OByteArray.metatable");
}

/**
 * ClassMethods
 */

static int
lua_OByteArrayClassMethodNew( lua_State *L ) {
  const char *str = (char *) lua_tostring(L, 1);
  OByteArray **ptr = (OByteArray **)lua_newuserdata(L, sizeof(OByteArray*));
  *ptr= new OByteArray(OByteArray(str));
  luaL_getmetatable(L, "OByteArray.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg OByteArrayClassMethods[] = {
  {"new", lua_OByteArrayClassMethodNew},
  {NULL, NULL}
};

void static
registerOByteArrayClassMethods( lua_State *L ) {
  luaL_register(L, "OByteArray", OByteArrayClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_OByteArrayInstanceMethodDestruct( lua_State *L ) {
  OByteArray *udata = checkOByteArray( L );
  delete udata;
  return 0;
}

static int
lua_OByteArrayInstanceMethodToString( lua_State *L ) {
  OByteArray *udata = checkOByteArray( L );
  lua_pushstring(L, udata->data());
  return 1;
}

static int
lua_OByteArrayInstanceMethodConcat( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  const char *str = lua_tostring(L, 2);
  udata->append(str);
  lua_pushstring(L, udata->data());
  return 1;
}

static int
lua_OByteArrayInstanceMethodToUpper( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  lua_pushstring(L, udata->toUpper());
  return 1;
}

static int
lua_OByteArrayInstanceMethodReplace( lua_State *L ) {
  OByteArray * udata  = checkOByteArray( L );
  const char * before = lua_tostring(L, 2);
  const char * after  = lua_tostring(L, 3);
  udata->replace(before, after);
  lua_pushstring(L, udata->data());
  return 1;
}

static int
lua_OByteArrayInstanceMethodTrimmed( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  lua_pushstring(L, udata->trimmed());
  return 1;
}

static int
lua_OByteArrayInstanceMethodSimplified( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  lua_pushstring(L, udata->simplified());
  return 1;
}

static int
lua_OByteArrayInstanceMethodAppend( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  const char *str    = lua_tostring(L, 2);
  udata->append(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
lua_OByteArrayInstanceMethodPrepend( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  const char *str    = lua_tostring(L, 2);
  udata->prepend(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
lua_OByteArrayInstanceMethodCamelcase( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  lua_pushstring(L, udata->camelcase());
  return 1;
}

static int
lua_OByteArrayInstanceMethodCapitalize( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  lua_pushstring(L, udata->capitalize());
  return 1;
}

static int
lua_OByteArrayInstanceMethodUnderscore( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  lua_pushstring(L, udata->underscore());
  return 1;
}

static int
lua_OByteArrayInstanceMethodLastIndexOf( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  const char *str    = lua_tostring(L, 2);
  lua_pushnumber(L, udata->lastIndexOf(str[0]));
  return 1;
}

static int
lua_OByteArrayInstanceMethodMid( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  int pos =  luaL_checkinteger(L, 2);
  int len;
  if(lua_gettop(L) == 3) {
    len =  luaL_checkinteger(L, 3);
  } else {
    len = udata->size();
  }

  lua_pushstring(L, udata->mid(pos, len));
  return 1;
}

static int
lua_OByteArrayInstanceMethodSize( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  lua_pushnumber(L, udata->size());
  return 1;
}

static int
lua_OByteArrayInstanceMethodRight( lua_State *L ) {
  OByteArray *udata  = checkOByteArray( L );
  int len =  luaL_checkinteger(L, 2);
  lua_pushstring(L, udata->right(len));
  return 1;
}

static const
luaL_reg OByteArrayInstanceMethods[] = {
  {"camelcase", lua_OByteArrayInstanceMethodCamelcase},
  {"capitalize", lua_OByteArrayInstanceMethodCapitalize},
  {"right", lua_OByteArrayInstanceMethodRight},
  {"size", lua_OByteArrayInstanceMethodSize},
  {"mid", lua_OByteArrayInstanceMethodMid},
  {"lastIndexOf", lua_OByteArrayInstanceMethodLastIndexOf},
  {"toUpper", lua_OByteArrayInstanceMethodToUpper},
  {"prepend", lua_OByteArrayInstanceMethodPrepend},
  {"append", lua_OByteArrayInstanceMethodAppend},
  {"trimmed", lua_OByteArrayInstanceMethodTrimmed},
  {"replace", lua_OByteArrayInstanceMethodReplace},
  {"simplified", lua_OByteArrayInstanceMethodSimplified},
  {"underscore", lua_OByteArrayInstanceMethodUnderscore},
  {"__concat", lua_OByteArrayInstanceMethodConcat},
  {"__tostring", lua_OByteArrayInstanceMethodToString},
  {"__gc", lua_OByteArrayInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerOByteArrayInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "OByteArray.metatable");
  luaL_register(L, NULL, OByteArrayInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_OByteArray( lua_State *L ) {
    registerOByteArrayClassMethods(L);
    registerOByteArrayInstanceMethods(L);
    return 1;
  }
}
