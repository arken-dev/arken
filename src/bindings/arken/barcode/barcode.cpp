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
  return *static_cast<arken::Barcode **>(luaL_checkudata(L, position, "arken.Barcode.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_Barcode_new( lua_State *L ) {
  arken::Barcode **ptr = static_cast<arken::Barcode **>(lua_newuserdata(L, sizeof(Barcode*)));
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

static const luaL_reg arken_Barcode[] = {
  {"new", arken_Barcode_new},
  {nullptr, nullptr}
};

void static
register_arken_Barcode( lua_State *L ) {
  luaL_newmetatable(L, "arken.Barcode");
  luaL_register(L, nullptr, arken_Barcode);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_Barcode_setText( lua_State *L ) {
  arken::Barcode *barcode = checkBarcode( L );
  const char * text = lua_tostring(L, 2);
  barcode->setText(text);
  return 0;
}

static int
arken_Barcode_save( lua_State *L ) {
  arken::Barcode *barcode = checkBarcode( L );
  const char * path = lua_tostring(L, 2);
  barcode->save(path);

  return 0;
}

static int
arken_Barcode_gc( lua_State *L ) {
  arken::Barcode *barcode = checkBarcode( L );
  delete barcode;
  return 0;
}

static const
luaL_reg arken_Barcode_metatable[] = {
  {"setText", arken_Barcode_setText},
  {"save",    arken_Barcode_save},
  {"__gc",    arken_Barcode_gc},
  {nullptr, nullptr}
};

void static
register_arken_Barcode_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.Barcode.metatable");
  luaL_register(L, nullptr, arken_Barcode_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_Barcode( lua_State *L ) {
    register_arken_Barcode_metatable(L);
    register_arken_Barcode(L);
    return 1;
  }
}
