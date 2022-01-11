// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/barcode.h>

using arken::Barcode;

/**
 * checkBarcode
 */

arken::Barcode *
checkBarcode( lua_State *L , int position = 1) {
  return *(arken::Barcode **) luaL_checkudata(L, position, "arken.Barcode.metatable");
}

/**
 * ClassMethods
 */

static int
arken_BarcodeClassMethodNew( lua_State *L ) {
  arken::Barcode **ptr = (arken::Barcode **)lua_newuserdata(L, sizeof(Barcode*));
  int width  = luaL_checkinteger(L, 1);
  int height = luaL_checkinteger(L, 2);
  const char *format = lua_tostring(L, 3);
  if(lua_gettop(L) == 5) { // number of arguments
    const char *text   = lua_tostring(L, 4);
    *ptr = new arken::Barcode(width, height, format, text);
  } else {
    *ptr = new arken::Barcode(width, height, format);
  }

  luaL_getmetatable(L, "arken.Barcode.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg BarcodeClassMethods[] = {
  {"new", arken_BarcodeClassMethodNew},
  {NULL, NULL}
};

void static
registerBarcodeClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.Barcode");
  luaL_register(L, NULL, BarcodeClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_BarcodeInstanceMethodSetText( lua_State *L ) {
  arken::Barcode *barcode = checkBarcode( L );
  const char * text = lua_tostring(L, 2);
  barcode->setText(text);
  return 0;
}

static int
arken_BarcodeInstanceMethodSave( lua_State *L ) {
  arken::Barcode *barcode = checkBarcode( L );
  const char * path = lua_tostring(L, 2);
  barcode->save(path);

  return 0;
}

static int
arken_BarcodeInstanceMethodDestruct( lua_State *L ) {
  arken::Barcode *barcode = checkBarcode( L );
  delete barcode;
  return 0;
}

static const
luaL_reg BarcodeInstanceMethods[] = {
  {"setText", arken_BarcodeInstanceMethodSetText},
  {"save",    arken_BarcodeInstanceMethodSave},
  {"__gc",    arken_BarcodeInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerBarcodeInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.Barcode.metatable");
  luaL_register(L, NULL, BarcodeInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_Barcode( lua_State *L ) {
    registerBarcodeInstanceMethods(L);
    registerBarcodeClassMethods(L);
    return 1;
  }
}
