// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/image.h>

/**
 * checkImage
 */

arken::Image *
checkImage( lua_State *L , int position = 1) {
  return *(arken::Image **) luaL_checkudata(L, position, "Image.metatable");
}

/**
 * ClassMethods
 */

static int
arken_ImageClassMethodNew( lua_State *L ) {
  arken::Image **ptr = (arken::Image **)lua_newuserdata(L, sizeof(Image*));

  if(lua_gettop(L) == 4) { // number of arguments
    int width  = luaL_checkinteger(L, 1);
    int height = luaL_checkinteger(L, 2);
    const char *color = lua_tostring(L, 3);
   *ptr = new arken::Image(width, height, color);
  } else {
    size_t len;
    const char *path = (char *) lua_tolstring(L, 1, &len);
    *ptr= new arken::Image(path);
  }
  luaL_getmetatable(L, "Image.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg ImageClassMethods[] = {
  {"new", arken_ImageClassMethodNew},
  {NULL, NULL}
};

void static
registerImageClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "Image");
  luaL_register(L, NULL, ImageClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_ImageInstanceMethodSave( lua_State *L ) {
  arken::Image *image = checkImage( L );
  const char * path= lua_tostring(L, 2);
  int quality = lua_tointeger(L, 3);
  image->save(path, quality);
  return 0;
}

static int
arken_ImageInstanceMethodResize( lua_State *L ) {
  arken::Image *image = checkImage( L );
  int width  = lua_tointeger(L, 3);
  int height = lua_tointeger(L, 3);
  image->resize(width, height);
  return 0;
}

static int
arken_ImageInstanceMethodWidth( lua_State *L ) {
  arken::Image *image = checkImage( L );
  lua_pushinteger(L, image->width());
  return 1;
}

static int
arken_ImageInstanceMethodHeight( lua_State *L ) {
  arken::Image *image = checkImage( L );
  lua_pushinteger(L, image->height());
  return 1;
}

static int
arken_ImageInstanceMethodDestruct( lua_State *L ) {
  arken::Image *image = checkImage( L );
  delete image;
  return 0;
}

static int
arken_ImageInstanceMethodComposite( lua_State *L ) {
  if(lua_gettop(L) == 4) { // number of arguments
    arken::Image *img1 = checkImage( L, 1 );
    arken::Image *img2 = checkImage( L, 2 );
    size_t x = (size_t) luaL_checkinteger(L, 3);
    size_t y = (size_t) luaL_checkinteger(L, 4);
    img1->composite(img2, x, y);
  } else {
    arken::Image *img1 = checkImage( L, 1 );
    arken::Image *img2 = checkImage( L, 2 );
    img1->composite(img2);
  }
  return 0;
}


static const
luaL_reg ImageInstanceMethods[] = {
  {"resize",    arken_ImageInstanceMethodResize},
  {"save",      arken_ImageInstanceMethodSave},
  {"width",     arken_ImageInstanceMethodWidth},
  {"height",    arken_ImageInstanceMethodHeight},
  {"composite", arken_ImageInstanceMethodComposite},
  {"__gc",      arken_ImageInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerImageInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "Image.metatable");
  luaL_register(L, NULL, ImageInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_Image( lua_State *L ) {
    registerImageInstanceMethods(L);
    registerImageClassMethods(L);
    return 1;
  }
}
