// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/xml/document.h>

using arken::xml::Document;
using arken::xml::Node;

/**
 * checkDocument
 */

arken::xml::Document *
checkDocument( lua_State *L , int position = 1) {
  return *static_cast<arken::xml::Document **>(luaL_checkudata(L, position, "arken.xml.Document.metatable"));
}

arken::xml::Node *
checkNode( lua_State *L , int position = 1) {
  return *static_cast<arken::xml::Node **>(luaL_checkudata(L, position, "arken.xml.Node.metatable"));
}

/**
 * ClassMethods
 */

static int
arken_xml_Document_new( lua_State *L ) {
  auto ptr = static_cast<arken::xml::Document **>(lua_newuserdata(L, sizeof(Document*)));
  *ptr = new arken::xml::Document();
  luaL_getmetatable(L, "arken.xml.Document.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg arken_xml_Document[] = {
  {"new", arken_xml_Document_new},
  {nullptr, nullptr}
};

void static
register_arken_xml_Document( lua_State *L ) {
  luaL_newmetatable(L, "arken.xml.Document");
  luaL_register(L, nullptr, arken_xml_Document);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_xml_Document_loadFile( lua_State *L ) {
  arken::xml::Document * ptr = checkDocument( L );
  const char * path = lua_tostring(L, 2);
  ptr->loadFile(path);
  return 0;
}

static int
arken_xml_Document_loadXml( lua_State *L ) {
  arken::xml::Document * ptr = checkDocument( L );
  size_t size;
  const char *buffer = luaL_checklstring(L, 2, &size);
  ptr->loadXml(string(buffer, size));
  return 0;
}

static int
arken_xml_Document_canonicalize( lua_State *L ) {
  arken::xml::Document * ptr = checkDocument( L );
  string result = ptr->canonicalize();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_xml_Document_c14n( lua_State *L ) {
  arken::xml::Document * ptr = checkDocument( L );
  string result = ptr->c14n();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_xml_Document_save( lua_State *L ) {
  arken::xml::Document * ptr = checkDocument( L );
  string result = ptr->save();
  lua_pushlstring(L, result.data(), result.size());
  return 1;
}

static int
arken_xml_Document_create( lua_State *L ) {
  arken::xml::Document * ptr = checkDocument( L );
  auto node = static_cast<arken::xml::Node **>(lua_newuserdata(L, sizeof(Node *)));
  const char *name  = luaL_checkstring(L, 2);

  if(lua_gettop(L) == 4) { /* número de argumentos */
    const char *value = luaL_checkstring(L, 3);
    *node = ptr->create(name, value);
  } else {
    *node = ptr->create(name);
  }

  luaL_getmetatable(L, "arken.xml.Node.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_xml_Document_root( lua_State *L ) {
  arken::xml::Document * ptr = checkDocument( L );
  auto node = static_cast<arken::xml::Node **>(lua_newuserdata(L, sizeof(Node *)));
  *node = ptr->root();
  luaL_getmetatable(L, "arken.xml.Node.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_xml_Document_append( lua_State *L ) {
  arken::xml::Document *document = checkDocument( L, 1 );
  arken::xml::Node *node = checkNode( L , 2 );
  document->append(node);
  return 0;
}

static int
arken_xml_Document_gc( lua_State *L ) {
  arken::xml::Document *document = checkDocument( L );
  delete document;
  return 0;
}

static const
luaL_reg arken_xml_Document_metatable[] = {
  {"loadXml",      arken_xml_Document_loadXml},
  {"loadFile",     arken_xml_Document_loadFile},
  {"canonicalize", arken_xml_Document_canonicalize},
  {"c14n",         arken_xml_Document_c14n},
  {"save",         arken_xml_Document_save},
  {"create",       arken_xml_Document_create},
  {"root",         arken_xml_Document_root},
  {"append",       arken_xml_Document_append},
  {"__gc",         arken_xml_Document_gc},
  {nullptr, nullptr}
};

void static
register_arken_xml_Document_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.xml.Document.metatable");
  luaL_register(L, nullptr, arken_xml_Document_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_xml_Node_append( lua_State *L ) {
  arken::xml::Node *node1 = checkNode( L , 1 );
  arken::xml::Node *node2 = checkNode( L , 2 );
  node1->append(node2);
  return 0;
}

static int
arken_xml_Node_attribute( lua_State *L ) {
  arken::xml::Node *node = checkNode( L , 1 );
  const char *key   = luaL_checkstring(L, 2);
  const char *value = luaL_checkstring(L, 3);
  node->attribute(key, value);
  return 0;
}

static int
arken_xml_Node_gc( lua_State *L ) {
  arken::xml::Node *node= checkNode( L );
  delete node;
  return 0;
}

static const
luaL_reg arken_xml_Node_metatable[] = {
  {"append",      arken_xml_Node_append},
  {"attribute",   arken_xml_Node_attribute},
  {"__gc",        arken_xml_Node_gc},
  {nullptr, nullptr}
};

void static
register_arken_xml_Node_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.xml.Node.metatable");
  luaL_register(L, nullptr, arken_xml_Node_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_xml_Document( lua_State *L ) {
    register_arken_xml_Node_metatable(L);
    register_arken_xml_Document_metatable(L);
    register_arken_xml_Document(L);
    return 1;
  }
}
