// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/naiad.h>

using naiad = arken::concurrent::naiad;

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * data);

naiad *
checkNaiad( lua_State *L ) {
  return *(naiad **) luaL_checkudata(L, 1, "arken.concurrent.naiad.metatable");
}

naiad::node *
checkNode( lua_State *L ) {
  return *(naiad::node **) luaL_checkudata(L, 1, "arken.concurrent.naiad.node.metatable");
}


//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_naiad_start(lua_State *L) {
  bool release  = false;
  int  priority = 0;
  char * params = nullptr;
  const char * fileName = luaL_checkstring(L, 1);

  if(lua_gettop(L) == 4) { // number of arguments
    release = lua_toboolean(L, 4);
  }

  if(lua_gettop(L) >= 3) { // number of arguments
    priority = lua_tointeger(L, 3);
  }

  if(lua_gettop(L) == 1) { /* número de argumentos */
    params = new char[3]{'{','}','\0'};
  } else {
    lua_settop(L, 2);
    params = json_lock_encode(L);
  }

  naiad::node node = naiad::start( fileName, params, priority, release );
  naiad::node **ptr = (naiad::node **)lua_newuserdata(L, sizeof(naiad::node*));
  *ptr = new naiad::node(node);
  luaL_getmetatable(L, "arken.concurrent.naiad.node.metatable");
  lua_setmetatable(L, -2);

  delete params;

  return 1;
}

static int
arken_naiad_wait(lua_State *L) {
  naiad::wait();
  return 0;
}

static int
arken_naiad_max(lua_State *L) {
  int max = luaL_checkinteger(L, 1);
  naiad::s_max = max;
  return 0;
}

static int
arken_naiad_actives(lua_State *L) {
  lua_pushinteger(L, naiad::s_actives);
  return 1;
}


static const luaL_reg NaiadClassMethods[] = {
  {"start",   arken_naiad_start},
  {"wait",    arken_naiad_wait},
  {"max",     arken_naiad_max},
  {"actives", arken_naiad_actives},
  {NULL, NULL}
};

void static
registerNaiadClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.naiad");
  luaL_register(L, NULL, NaiadClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_channel_node_instance_method_uuid( lua_State *L ) {
  naiad::node * node = checkNode( L );
  lua_pushlstring(L, node->uuid(), 37);
  return 1;
}

static int
arken_concurrent_channel_node_instance_method_priority( lua_State *L ) {
  naiad::node * node = checkNode( L );
  lua_pushinteger(L, node->priority());
  return 1;
}

static int
arken_concurrent_channel_node_instance_method_microtime( lua_State *L ) {
  naiad::node * node = checkNode( L );
  lua_pushnumber(L, node->microtime());
  return 1;
}

static int
arken_concurrent_channel_node_instance_method_destruct( lua_State *L ) {
  naiad::node * node = checkNode( L );
  delete node;
  return 0;
}

static const
luaL_reg NaiadNodeInstanceMethods[] = {
  {"uuid",      arken_concurrent_channel_node_instance_method_uuid},
  {"priority",  arken_concurrent_channel_node_instance_method_priority},
  {"microtime", arken_concurrent_channel_node_instance_method_microtime},
  {"__gc",      arken_concurrent_channel_node_instance_method_destruct},
  {NULL, NULL}
};

void static
registerNodeInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.naiad.node.metatable");
  luaL_register(L, NULL, NaiadNodeInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}


extern "C" {
  int
  luaopen_arken_concurrent_naiad( lua_State *L ) {
    registerNodeInstanceMethods(L);
    registerNaiadClassMethods(L);
    return 1;
  }
}
