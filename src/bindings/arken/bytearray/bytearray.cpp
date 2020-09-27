// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <QtCore>

using arken::ByteArray;

/**
 * checkByteArray
 */

ByteArray *
checkByteArray( lua_State *L ) {
  return *(ByteArray **) luaL_checkudata(L, 1, "ByteArray.metatable");
}

/**
 * ClassMethods
 */

static int
arken_ByteArrayClassMethodNew( lua_State *L ) {
  size_t len;
  const char *str = (char *) lua_tolstring(L, 1, &len);
  ByteArray **ptr = (ByteArray **)lua_newuserdata(L, sizeof(ByteArray*));
  *ptr= new ByteArray(ByteArray(str, len));
  luaL_getmetatable(L, "ByteArray.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg ByteArrayClassMethods[] = {
  {"new", arken_ByteArrayClassMethodNew},
  {NULL, NULL}
};

void static
registerByteArrayClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "ByteArray");
  luaL_register(L, NULL, ByteArrayClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_ByteArrayInstanceMethodDestruct( lua_State *L ) {
  ByteArray *udata = checkByteArray( L );
  delete udata;
  return 0;
}

static int
arken_ByteArrayInstanceMethodToString( lua_State *L ) {
  ByteArray *udata = checkByteArray( L );
  lua_pushlstring(L, udata->data(), udata->size());
  return 1;
}

static int
arken_ByteArrayInstanceMethodConcat( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str = lua_tostring(L, 2);
  udata->append(str);
  lua_pushstring(L, udata->data());
  return 1;
}

static int
arken_ByteArrayInstanceMethodToUpper( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->toUpper());
  return 1;
}

static int
arken_ByteArrayInstanceMethodReplace( lua_State *L ) {
  ByteArray * udata  = checkByteArray( L );
  const char * before = lua_tostring(L, 2);
  const char * after  = lua_tostring(L, 3);
  udata->replace(before, after);
  lua_pushstring(L, udata->data());
  return 1;
}

static int
arken_ByteArrayInstanceMethodNormalize( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->normalize());
  return 1;
}

static int
arken_ByteArrayInstanceMethodTrimmed( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->trimmed());
  return 1;
}

static int
arken_ByteArrayInstanceMethodLeftTrimmed( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->leftTrimmed());
  return 1;
}

static int
arken_ByteArrayInstanceMethodRightTrimmed( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->rightTrimmed());
  return 1;
}

static int
arken_ByteArrayInstanceMethodSimplified( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->simplified());
  return 1;
}

static int
arken_ByteArrayInstanceMethodAppend( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str   = lua_tostring(L, 2);
  udata->append(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
arken_ByteArrayInstanceMethodPrepend( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str   = lua_tostring(L, 2);
  udata->prepend(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
arken_ByteArrayInstanceMethodCamelCase( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->camelCase());
  return 1;
}

static int
arken_ByteArrayInstanceMethodCapitalize( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->capitalize());
  return 1;
}

static int
arken_ByteArrayInstanceMethodUnderscore( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->underscore());
  return 1;
}

static int
arken_ByteArrayInstanceMethodLastIndexOf( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str    = lua_tostring(L, 2);
  lua_pushnumber(L, udata->lastIndexOf(str[0]));
  return 1;
}

static int
arken_ByteArrayInstanceMethodMid( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
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
arken_ByteArrayInstanceMethodSize( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushnumber(L, udata->size());
  return 1;
}

static int
arken_ByteArrayInstanceMethodRight( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  int len =  luaL_checkinteger(L, 2);
  lua_pushstring(L, udata->right(len));
  return 1;
}

static int
arken_ByteArrayInstanceMethodResize( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  int len =  luaL_checkinteger(L, 2);
  udata->resize(len);
  return 0;
}

static int
arken_ByteArrayInstanceMethodSqueeze( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  udata->squeeze();
  return 0;
}


static const
luaL_reg ByteArrayInstanceMethods[] = {
  {"camelCase", arken_ByteArrayInstanceMethodCamelCase},
  {"capitalize", arken_ByteArrayInstanceMethodCapitalize},
  {"right", arken_ByteArrayInstanceMethodRight},
  {"size", arken_ByteArrayInstanceMethodSize},
  {"mid", arken_ByteArrayInstanceMethodMid},
  {"lastIndexOf", arken_ByteArrayInstanceMethodLastIndexOf},
  {"toUpper", arken_ByteArrayInstanceMethodToUpper},
  {"prepend", arken_ByteArrayInstanceMethodPrepend},
  {"append", arken_ByteArrayInstanceMethodAppend},
  {"trimmed",   arken_ByteArrayInstanceMethodTrimmed},
  {"leftTrimmed",  arken_ByteArrayInstanceMethodLeftTrimmed},
  {"rightTrimmed", arken_ByteArrayInstanceMethodRightTrimmed},
  {"normalize", arken_ByteArrayInstanceMethodNormalize},
  {"replace",   arken_ByteArrayInstanceMethodReplace},
  {"simplified", arken_ByteArrayInstanceMethodSimplified},
  {"underscore", arken_ByteArrayInstanceMethodUnderscore},
  {"resize", arken_ByteArrayInstanceMethodResize},
  {"squeeze", arken_ByteArrayInstanceMethodSqueeze},
  {"__concat", arken_ByteArrayInstanceMethodConcat},
  {"__tostring", arken_ByteArrayInstanceMethodToString},
  {"__gc", arken_ByteArrayInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerByteArrayInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "ByteArray.metatable");
  luaL_register(L, NULL, ByteArrayInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_ByteArray( lua_State *L ) {
    registerByteArrayInstanceMethods(L);
    registerByteArrayClassMethods(L);
    return 1;
  }
}
