// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/task/fifo.h>

using fifo   = arken::concurrent::task::fifo;
using Shared = arken::concurrent::Shared;

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * data);

fifo *
checkNaiad( lua_State *L ) {
  return *(fifo **) luaL_checkudata(L, 1, "arken.concurrent.task.fifo.metatable");
}

fifo::node *
checkNode( lua_State *L ) {
  return *(fifo::node **) luaL_checkudata(L, 1, "arken.concurrent.task.fifo.node.metatable");
}

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_fifo_start(lua_State *L) {
  bool purge = false;
  char * params = nullptr;
  const char * fileName = luaL_checkstring(L, 1);

  if(lua_gettop(L) == 3) { // number of arguments
    purge = lua_toboolean(L, 3);
  }

  if(lua_gettop(L) == 1) { /* número de argumentos */
    params = new char[3]{'{','}','\0'};
  } else {
    lua_settop(L, 2);
    params = json_lock_encode(L);
  }

  fifo::node node = fifo::start( fileName, params, purge );
  fifo::node **ptr = (fifo::node **)lua_newuserdata(L, sizeof(fifo::node*));
  *ptr = new fifo::node(node);
  luaL_getmetatable(L, "arken.concurrent.task.fifo.node.metatable");
  lua_setmetatable(L, -2);

  delete params;

  return 1;
}

static int
arken_fifo_max(lua_State *L) {
  int max = luaL_checkinteger(L, 1);
  fifo::s_max = max;
  return 0;
}

static int
arken_fifo_actives(lua_State *L) {
  lua_pushinteger(L, fifo::s_actives);
  return 1;
}

static const luaL_reg NaiadClassMethods[] = {
  {"start",   arken_fifo_start},
  {"max",     arken_fifo_max},
  {"actives", arken_fifo_actives},
  {NULL, NULL}
};

void static
registerNaiadClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.task.fifo");
  luaL_register(L, NULL, NaiadClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_channel_node_instance_method_uuid( lua_State *L ) {
  fifo::node * node = checkNode( L );
  lua_pushlstring(L, node->uuid(), 36);
  return 1;
}

static int
arken_concurrent_channel_node_instance_method_microtime( lua_State *L ) {
  fifo::node * node = checkNode( L );
  lua_pushnumber(L, node->microtime());
  return 1;
}

static int
arken_concurrent_channel_node_instance_method_shared( lua_State *L ) {
  fifo::node * node = checkNode( L );
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
arken_concurrent_channel_node_instance_method_finished( lua_State *L ) {
  fifo::node * node = checkNode( L );
  lua_pushboolean(L, node->finished());
  return 1;
}

static int
arken_concurrent_channel_node_instance_method_wait( lua_State *L ) {
  fifo::node * node = checkNode( L );
  node->wait();
  return 0;
}

static int
arken_concurrent_channel_node_instance_method_destruct( lua_State *L ) {
  fifo::node * node = checkNode( L );
  delete node;
  return 0;
}

static const
luaL_reg NaiadNodeInstanceMethods[] = {
  {"uuid",      arken_concurrent_channel_node_instance_method_uuid},
  {"microtime", arken_concurrent_channel_node_instance_method_microtime},
  {"shared",    arken_concurrent_channel_node_instance_method_shared},
  {"finished",  arken_concurrent_channel_node_instance_method_finished},
  {"wait",      arken_concurrent_channel_node_instance_method_wait},
  {"__gc",      arken_concurrent_channel_node_instance_method_destruct},
  {NULL, NULL}
};

void static
registerNodeInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.task.fifo.node.metatable");
  luaL_register(L, NULL, NaiadNodeInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_task_fifo( lua_State *L ) {
    registerNodeInstanceMethods(L);
    registerNaiadClassMethods(L);
    return 1;
  }
}
