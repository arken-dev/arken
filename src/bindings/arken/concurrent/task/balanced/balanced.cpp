// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/task/balanced.h>
#include <arken/json.h>

using balanced = arken::concurrent::task::balanced;
using Shared   = arken::concurrent::Shared;
using json     = arken::json;

balanced *
checkTask( lua_State *L ) {
  return *(balanced **) luaL_checkudata(L, 1, "arken.concurrent.task.balanced.metatable");
}

balanced::node *
checkNode( lua_State *L ) {
  return *(balanced::node **) luaL_checkudata(L, 1, "arken.concurrent.task.balanced.node.metatable");
}


//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_concurrent_task_balanced_start(lua_State *L) {
  bool release  = false;
  char * params = nullptr;
  const char * fileName = luaL_checkstring(L, 1);
  const char * name = "default";

  if(lua_gettop(L) == 4) { // number of arguments
    release = lua_toboolean(L, 4);
  }

  if(lua_gettop(L) >= 3) { // number of arguments
    name = luaL_checkstring(L, 3);
  }

  if(lua_gettop(L) == 1) { /* número de argumentos */
    params = new char[3]{'{','}','\0'};
  } else {
    lua_settop(L, 2);
    params = json::encode(L);
  }

  balanced::node node = balanced::start( fileName, params, name, release );
  balanced::node **ptr = (balanced::node **)lua_newuserdata(L, sizeof(balanced::node*));
  *ptr = new balanced::node(node);
  luaL_getmetatable(L, "arken.concurrent.task.balanced.node.metatable");
  lua_setmetatable(L, -2);

  delete params;

  return 1;
}

static int
arken_concurrent_task_balanced_max(lua_State *L) {
  if(lua_gettop(L) == 1) { /* número de argumentos */
    int max = luaL_checkinteger(L, 1);
    balanced::max() = max;
    return 0;
  } else {
    lua_pushinteger(L, balanced::max());
    return 1;
  }
}

static int
arken_concurrent_task_balanced_actives(lua_State *L) {
  lua_pushinteger(L, balanced::actives());
  return 1;
}

static int
arken_concurrent_task_balanced_inspect( lua_State *L ) {
  lua_pushstring(L, balanced::inspect());
  return 1;
}

static const luaL_reg arken_concurrent_task_balanced[] = {
  {"start",   arken_concurrent_task_balanced_start},
  {"max",     arken_concurrent_task_balanced_max},
  {"actives", arken_concurrent_task_balanced_actives},
  {"inspect", arken_concurrent_task_balanced_inspect},
  {NULL, NULL}
};

void static
register_arken_concurrent_task_balanced( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.task.balanced");
  luaL_register(L, NULL, arken_concurrent_task_balanced);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_task_balanced_node_uuid( lua_State *L ) {
  balanced::node * node = checkNode( L );
  lua_pushlstring(L, node->uuid(), 36);
  return 1;
}

static int
arken_concurrent_task_balanced_node_name( lua_State *L ) {
  balanced::node * node = checkNode( L );
  lua_pushstring(L, node->name());
  return 1;
}

static int
arken_concurrent_task_balanced_node_microtime( lua_State *L ) {
  balanced::node * node = checkNode( L );
  lua_pushnumber(L, node->microtime());
  return 1;
}

static int
arken_concurrent_task_balanced_node_shared( lua_State *L ) {
  balanced::node * node = checkNode( L );
  int rv;
  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.concurrent.Shared");
  rv = lua_pcall(L, 1, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  }

  Shared **ptr = (Shared **)lua_newuserdata(L, sizeof(Shared*));
  *ptr = new Shared(node->shared());
  luaL_getmetatable(L, "arken.concurrent.Shared.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_concurrent_task_balanced_node_gc( lua_State *L ) {
  balanced::node * node = checkNode( L );
  delete node;
  return 0;
}

static int
arken_concurrent_task_balanced_node_finished( lua_State *L ) {
  balanced::node * node = checkNode( L );
  lua_pushboolean(L, node->finished());
  return 1;
}

static int
arken_concurrent_task_balanced_node_wait( lua_State *L ) {
  balanced::node * node = checkNode( L );
  node->wait();
  return 0;
}

static const
luaL_reg arken_concurrent_task_balanced_node_metatable[] = {
  {"uuid",      arken_concurrent_task_balanced_node_uuid},
  {"name",      arken_concurrent_task_balanced_node_name},
  {"microtime", arken_concurrent_task_balanced_node_microtime},
  {"shared",    arken_concurrent_task_balanced_node_shared},
  {"finished",  arken_concurrent_task_balanced_node_finished},
  {"wait",      arken_concurrent_task_balanced_node_wait},
  {"__gc",      arken_concurrent_task_balanced_node_gc},
  {NULL, NULL}
};

void static
register_arken_concurrent_task_balanced_node_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.task.balanced.node.metatable");
  luaL_register(L, NULL, arken_concurrent_task_balanced_node_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_task_balanced( lua_State *L ) {
    register_arken_concurrent_task_balanced_node_metatable(L);
    register_arken_concurrent_task_balanced(L);
    return 1;
  }
}
