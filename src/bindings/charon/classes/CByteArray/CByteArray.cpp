// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <CByteArray>

/**
 * checkCByteArray
 */

CByteArray *
checkCByteArray( lua_State *L ) {
  return *(CByteArray **) luaL_checkudata(L, 1, "CByteArray.metatable");
}

/**
 * ClassMethods
 */

static int
lua_CByteArrayClassMethodNew( lua_State *L ) {
  size_t len;
  const char *str = (char *) lua_tolstring(L, 1, &len);
  CByteArray **ptr = (CByteArray **)lua_newuserdata(L, sizeof(CByteArray*));
  *ptr= new CByteArray(CByteArray(str, len));
  luaL_getmetatable(L, "CByteArray.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg CByteArrayClassMethods[] = {
  {"new", lua_CByteArrayClassMethodNew},
  {NULL, NULL}
};

void static
registerCByteArrayClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "ByteArray");
  luaL_register(L, NULL, CByteArrayClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
lua_CByteArrayInstanceMethodDestruct( lua_State *L ) {
  CByteArray *udata = checkCByteArray( L );
  delete udata;
  return 0;
}

static int
lua_CByteArrayInstanceMethodToString( lua_State *L ) {
  CByteArray *udata = checkCByteArray( L );
  lua_pushlstring(L, udata->data(), udata->size());
  return 1;
}

static int
lua_CByteArrayInstanceMethodConcat( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  const char *str = lua_tostring(L, 2);
  udata->append(str);
  lua_pushstring(L, udata->data());
  return 1;
}

static int
lua_CByteArrayInstanceMethodToUpper( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  lua_pushstring(L, udata->toUpper());
  return 1;
}

static int
lua_CByteArrayInstanceMethodReplace( lua_State *L ) {
  CByteArray * udata  = checkCByteArray( L );
  const char * before = lua_tostring(L, 2);
  const char * after  = lua_tostring(L, 3);
  udata->replace(before, after);
  lua_pushstring(L, udata->data());
  return 1;
}

static int
lua_CByteArrayInstanceMethodTrimmed( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  lua_pushstring(L, udata->trimmed());
  return 1;
}

static int
lua_CByteArrayInstanceMethodSimplified( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  lua_pushstring(L, udata->simplified());
  return 1;
}

static int
lua_CByteArrayInstanceMethodAppend( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  const char *str    = lua_tostring(L, 2);
  udata->append(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
lua_CByteArrayInstanceMethodPrepend( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  const char *str    = lua_tostring(L, 2);
  udata->prepend(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
lua_CByteArrayInstanceMethodCamelcase( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  lua_pushstring(L, udata->camelcase());
  return 1;
}

static int
lua_CByteArrayInstanceMethodCapitalize( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  lua_pushstring(L, udata->capitalize());
  return 1;
}

static int
lua_CByteArrayInstanceMethodUnderscore( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  lua_pushstring(L, udata->underscore());
  return 1;
}

static int
lua_CByteArrayInstanceMethodLastIndexOf( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  const char *str    = lua_tostring(L, 2);
  lua_pushnumber(L, udata->lastIndexOf(str[0]));
  return 1;
}

static int
lua_CByteArrayInstanceMethodMid( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  int pos =  luaL_checkinteger(L, 2);
  int len;
  if(lua_gettop(L) == 3) {
    len = luaL_checkinteger(L, 3);
  } else {
    len = udata->size();
  }

  QByteArray mid = udata->mid(pos, len);
  lua_pushlstring(L, mid, mid.size() );
  return 1;
}

static int
lua_CByteArrayInstanceMethodSize( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  lua_pushnumber(L, udata->size());
  return 1;
}

static int
lua_CByteArrayInstanceMethodRight( lua_State *L ) {
  CByteArray *udata  = checkCByteArray( L );
  int len =  luaL_checkinteger(L, 2);
  lua_pushstring(L, udata->right(len));
  return 1;
}

static const
luaL_reg CByteArrayInstanceMethods[] = {
  {"camelcase", lua_CByteArrayInstanceMethodCamelcase},
  {"capitalize", lua_CByteArrayInstanceMethodCapitalize},
  {"right", lua_CByteArrayInstanceMethodRight},
  {"size", lua_CByteArrayInstanceMethodSize},
  {"mid", lua_CByteArrayInstanceMethodMid},
  {"lastIndexOf", lua_CByteArrayInstanceMethodLastIndexOf},
  {"toUpper", lua_CByteArrayInstanceMethodToUpper},
  {"prepend", lua_CByteArrayInstanceMethodPrepend},
  {"append", lua_CByteArrayInstanceMethodAppend},
  {"trimmed", lua_CByteArrayInstanceMethodTrimmed},
  {"replace", lua_CByteArrayInstanceMethodReplace},
  {"simplified", lua_CByteArrayInstanceMethodSimplified},
  {"underscore", lua_CByteArrayInstanceMethodUnderscore},
  {"__concat", lua_CByteArrayInstanceMethodConcat},
  {"__tostring", lua_CByteArrayInstanceMethodToString},
  {"__gc", lua_CByteArrayInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerCByteArrayInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "CByteArray.metatable");
  luaL_register(L, NULL, CByteArrayInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_CByteArray( lua_State *L ) {
    registerCByteArrayInstanceMethods(L);
    registerCByteArrayClassMethods(L);
    return 1;
  }
}
