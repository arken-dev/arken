// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/helper>
#include <QtCore>

using charon::ByteArray;

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
lua_ByteArrayClassMethodNew( lua_State *L ) {
  size_t len;
  const char *str = (char *) lua_tolstring(L, 1, &len);
  ByteArray **ptr = (ByteArray **)lua_newuserdata(L, sizeof(ByteArray*));
  *ptr= new ByteArray(ByteArray(str, len));
  luaL_getmetatable(L, "ByteArray.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg ByteArrayClassMethods[] = {
  {"new", lua_ByteArrayClassMethodNew},
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
lua_ByteArrayInstanceMethodDestruct( lua_State *L ) {
  ByteArray *udata = checkByteArray( L );
  delete udata;
  return 0;
}

static int
lua_ByteArrayInstanceMethodToString( lua_State *L ) {
  ByteArray *udata = checkByteArray( L );
  lua_pushlstring(L, udata->data(), udata->size());
  return 1;
}

static int
lua_ByteArrayInstanceMethodConcat( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str = lua_tostring(L, 2);
  udata->append(str);
  lua_pushstring(L, udata->data());
  return 1;
}

static int
lua_ByteArrayInstanceMethodToUpper( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->toUpper());
  return 1;
}

static int
lua_ByteArrayInstanceMethodReplace( lua_State *L ) {
  ByteArray * udata  = checkByteArray( L );
  const char * before = lua_tostring(L, 2);
  const char * after  = lua_tostring(L, 3);
  udata->replace(before, after);
  lua_pushstring(L, udata->data());
  return 1;
}

static int
lua_ByteArrayInstanceMethodNormalize( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->normalize());
  return 1;
}

static int
lua_ByteArrayInstanceMethodTrim( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->trim());
  return 1;
}

static int
lua_ByteArrayInstanceMethodTrimLeft( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->trimLeft());
  return 1;
}

static int
lua_ByteArrayInstanceMethodTrimRight( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->trimRight());
  return 1;
}


static int
lua_ByteArrayInstanceMethodSimplified( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->reduce());
  return 1;
}

static int
lua_ByteArrayInstanceMethodAppend( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str    = lua_tostring(L, 2);
  udata->append(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
lua_ByteArrayInstanceMethodPrepend( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str    = lua_tostring(L, 2);
  udata->prepend(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
lua_ByteArrayInstanceMethodCamelcase( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->camelcase());
  return 1;
}

static int
lua_ByteArrayInstanceMethodCapitalize( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->capitalize());
  return 1;
}

static int
lua_ByteArrayInstanceMethodUnderscore( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->underscore());
  return 1;
}

static int
lua_ByteArrayInstanceMethodLastIndexOf( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str    = lua_tostring(L, 2);
  lua_pushnumber(L, udata->lastIndexOf(str[0]));
  return 1;
}

static int
lua_ByteArrayInstanceMethodMid( lua_State *L ) {
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
lua_ByteArrayInstanceMethodSize( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushnumber(L, udata->size());
  return 1;
}

static int
lua_ByteArrayInstanceMethodRight( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  int len =  luaL_checkinteger(L, 2);
  lua_pushstring(L, udata->right(len));
  return 1;
}

static const
luaL_reg ByteArrayInstanceMethods[] = {
  {"camelcase", lua_ByteArrayInstanceMethodCamelcase},
  {"capitalize", lua_ByteArrayInstanceMethodCapitalize},
  {"right", lua_ByteArrayInstanceMethodRight},
  {"size", lua_ByteArrayInstanceMethodSize},
  {"mid", lua_ByteArrayInstanceMethodMid},
  {"lastIndexOf", lua_ByteArrayInstanceMethodLastIndexOf},
  {"toUpper", lua_ByteArrayInstanceMethodToUpper},
  {"prepend", lua_ByteArrayInstanceMethodPrepend},
  {"append", lua_ByteArrayInstanceMethodAppend},
  {"trim",      lua_ByteArrayInstanceMethodTrim},
  {"trimLeft",  lua_ByteArrayInstanceMethodTrimLeft},
  {"trimRight", lua_ByteArrayInstanceMethodTrimRight},
  {"normalize", lua_ByteArrayInstanceMethodNormalize},
  {"replace",   lua_ByteArrayInstanceMethodReplace},
  {"reduce", lua_ByteArrayInstanceMethodSimplified},
  {"underscore", lua_ByteArrayInstanceMethodUnderscore},
  {"__concat", lua_ByteArrayInstanceMethodConcat},
  {"__tostring", lua_ByteArrayInstanceMethodToString},
  {"__gc", lua_ByteArrayInstanceMethodDestruct},
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
  luaopen_charon_ByteArray( lua_State *L ) {
    registerByteArrayInstanceMethods(L);
    registerByteArrayClassMethods(L);
    return 1;
  }
}
