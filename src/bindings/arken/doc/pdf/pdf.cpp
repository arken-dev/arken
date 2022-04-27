// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/doc/pdf.h>

using arken::doc::Pdf;

/**
 * checkPdf
 */

Pdf *
checkPdf( lua_State *L , int position = 1) {
  return *static_cast<arken::doc::Pdf **>(luaL_checkudata(L, position, "arken.doc.Pdf.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_doc_Pdf_new( lua_State *L ) {
  auto ptr  = static_cast<Pdf **>(lua_newuserdata(L, sizeof(Pdf *)));
  *ptr = new Pdf();

  luaL_getmetatable(L, "arken.doc.Pdf.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_doc_Pdf[] = {
  {"new", arken_doc_Pdf_new},
  {nullptr, nullptr}
};

void static
register_arken_doc_Pdf( lua_State *L ) {
  luaL_newmetatable(L, "arken.doc.Pdf");
  luaL_register(L, nullptr, arken_doc_Pdf);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_doc_Pdf_write( lua_State *L ) {
  Pdf *ptr= checkPdf( L );
  int width  = luaL_checkinteger(L, 2);
  int height = luaL_checkinteger(L, 3);
  const char * text = lua_tostring(L, 4);

  ptr->write(width, height, text);
  return 0;
}

static int
arken_doc_Pdf_writeText( lua_State *L ) {
  Pdf *ptr= checkPdf( L );
  int width  = luaL_checkinteger(L, 2);
  int height = luaL_checkinteger(L, 3);
  const char * text = lua_tostring(L, 4);

  ptr->writeText(width, height, text);
  return 0;
}

static int
arken_doc_Pdf_writeRectangle( lua_State *L ) {
  Pdf *ptr= checkPdf( L );
  float x = luaL_checknumber(L, 2);
  float y = luaL_checknumber(L, 3);
  float width  = luaL_checknumber(L, 4);
  float height = luaL_checknumber(L, 5);
  float border = 1;

  if(lua_gettop(L) == 6) { // number of arguments
    border = luaL_checknumber(L, 6);
  }

  ptr->writeRectangle(x, y, width, height, border);
  return 0;
}

static int
arken_doc_Pdf_setFont( lua_State *L ) {
  Pdf *ptr= checkPdf( L );
  const char * name = lua_tostring(L, 2);
  int size = luaL_checkinteger(L, 3);
  ptr->setFont(name, size);
  return 0;
}

static int
arken_doc_Pdf_width( lua_State *L ) {
  Pdf *ptr   = checkPdf( L );
  lua_pushnumber(L, ptr->width());
  return 1;
}

static int
arken_doc_Pdf_height( lua_State *L ) {
  Pdf *ptr   = checkPdf( L );
  lua_pushnumber(L, ptr->height());
  return 1;
}

static int
arken_doc_Pdf_save( lua_State *L ) {
  Pdf *ptr = checkPdf( L );
  const char * path = lua_tostring(L, 2);
  ptr->save(path);

  return 0;
}

static int
arken_doc_Pdf_gc( lua_State *L ) {
  Pdf * ptr = checkPdf( L );
  delete ptr;
  return 0;
}

static const
luaL_reg arken_doc_Pdf_metatable[] = {
  {"write",          arken_doc_Pdf_write},
  {"writeText",      arken_doc_Pdf_writeText},
  {"writeRectangle", arken_doc_Pdf_writeRectangle},
  {"width",          arken_doc_Pdf_width},
  {"height",         arken_doc_Pdf_height},
  {"setFont",        arken_doc_Pdf_setFont},
  {"save",           arken_doc_Pdf_save},
  {"__gc",           arken_doc_Pdf_gc},
  {nullptr, nullptr}
};

void static
register_arken_doc_Pdf_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.doc.Pdf.metatable");
  luaL_register(L, nullptr, arken_doc_Pdf_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_doc_Pdf( lua_State *L ) {
    register_arken_doc_Pdf_metatable(L);
    register_arken_doc_Pdf(L);
    return 1;
  }
}
