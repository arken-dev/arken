// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
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
charon_ByteArrayClassMethodNew( lua_State *L ) {
  size_t len;
  const char *str = (char *) lua_tolstring(L, 1, &len);
  ByteArray **ptr = (ByteArray **)lua_newuserdata(L, sizeof(ByteArray*));
  *ptr= new ByteArray(ByteArray(str, len));
  luaL_getmetatable(L, "ByteArray.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg ByteArrayClassMethods[] = {
  {"new", charon_ByteArrayClassMethodNew},
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
charon_ByteArrayInstanceMethodDestruct( lua_State *L ) {
  ByteArray *udata = checkByteArray( L );
  delete udata;
  return 0;
}

static int
charon_ByteArrayInstanceMethodToString( lua_State *L ) {
  ByteArray *udata = checkByteArray( L );
  lua_pushlstring(L, udata->data(), udata->size());
  return 1;
}

static int
charon_ByteArrayInstanceMethodConcat( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str = lua_tostring(L, 2);
  udata->append(str);
  lua_pushstring(L, udata->data());
  return 1;
}

static int
charon_ByteArrayInstanceMethodToUpper( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->toUpper());
  return 1;
}

static int
charon_ByteArrayInstanceMethodReplace( lua_State *L ) {
  ByteArray * udata  = checkByteArray( L );
  const char * before = lua_tostring(L, 2);
  const char * after  = lua_tostring(L, 3);
  udata->replace(before, after);
  lua_pushstring(L, udata->data());
  return 1;
}

static int
charon_ByteArrayInstanceMethodNormalize( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->normalize());
  return 1;
}

static int
charon_ByteArrayInstanceMethodTrimmed( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->trimmed());
  return 1;
}

static int
charon_ByteArrayInstanceMethodLeftTrimmed( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->leftTrimmed());
  return 1;
}

static int
charon_ByteArrayInstanceMethodRightTrimmed( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->rightTrimmed());
  return 1;
}

static int
charon_ByteArrayInstanceMethodSimplified( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->simplified());
  return 1;
}

static int
charon_ByteArrayInstanceMethodAppend( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str   = lua_tostring(L, 2);
  udata->append(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
charon_ByteArrayInstanceMethodPrepend( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str   = lua_tostring(L, 2);
  udata->prepend(str);
  lua_pushvalue(L, -2);
  return 1;
}

static int
charon_ByteArrayInstanceMethodCamelCase( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->camelCase());
  return 1;
}

static int
charon_ByteArrayInstanceMethodCapitalize( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->capitalize());
  return 1;
}

static int
charon_ByteArrayInstanceMethodUnderscore( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushstring(L, udata->underscore());
  return 1;
}

static int
charon_ByteArrayInstanceMethodLastIndexOf( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  const char *str    = lua_tostring(L, 2);
  lua_pushnumber(L, udata->lastIndexOf(str[0]));
  return 1;
}

static int
charon_ByteArrayInstanceMethodMid( lua_State *L ) {
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
charon_ByteArrayInstanceMethodSize( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  lua_pushnumber(L, udata->size());
  return 1;
}

static int
charon_ByteArrayInstanceMethodRight( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  int len =  luaL_checkinteger(L, 2);
  lua_pushstring(L, udata->right(len));
  return 1;
}

static int
charon_ByteArrayInstanceMethodResize( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  int len =  luaL_checkinteger(L, 2);
  udata->resize(len);
  return 0;
}

static int
charon_ByteArrayInstanceMethodSqueeze( lua_State *L ) {
  ByteArray *udata  = checkByteArray( L );
  udata->squeeze();
  return 0;
}


static const
luaL_reg ByteArrayInstanceMethods[] = {
  {"camelCase", charon_ByteArrayInstanceMethodCamelCase},
  {"capitalize", charon_ByteArrayInstanceMethodCapitalize},
  {"right", charon_ByteArrayInstanceMethodRight},
  {"size", charon_ByteArrayInstanceMethodSize},
  {"mid", charon_ByteArrayInstanceMethodMid},
  {"lastIndexOf", charon_ByteArrayInstanceMethodLastIndexOf},
  {"toUpper", charon_ByteArrayInstanceMethodToUpper},
  {"prepend", charon_ByteArrayInstanceMethodPrepend},
  {"append", charon_ByteArrayInstanceMethodAppend},
  {"trimmed",   charon_ByteArrayInstanceMethodTrimmed},
  {"leftTrimmed",  charon_ByteArrayInstanceMethodLeftTrimmed},
  {"rightTrimmed", charon_ByteArrayInstanceMethodRightTrimmed},
  {"normalize", charon_ByteArrayInstanceMethodNormalize},
  {"replace",   charon_ByteArrayInstanceMethodReplace},
  {"simplified", charon_ByteArrayInstanceMethodSimplified},
  {"underscore", charon_ByteArrayInstanceMethodUnderscore},
  {"resize", charon_ByteArrayInstanceMethodResize},
  {"squeeze", charon_ByteArrayInstanceMethodSqueeze},
  {"__concat", charon_ByteArrayInstanceMethodConcat},
  {"__tostring", charon_ByteArrayInstanceMethodToString},
  {"__gc", charon_ByteArrayInstanceMethodDestruct},
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
  int
  luaopen_arken_ByteArray( lua_State *L ) {
    registerByteArrayInstanceMethods(L);
    registerByteArrayClassMethods(L);
    return 1;
  }
}
