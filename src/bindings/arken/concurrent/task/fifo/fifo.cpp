// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/task/fifo.h>
#include <arken/json.h>

using fifo   = arken::concurrent::task::fifo;
using Shared = arken::concurrent::Shared;
using json   = arken::json;

fifo::node *
checkNode( lua_State *L ) {
  return *(fifo::node **) luaL_checkudata(L, 1, "arken.concurrent.task.fifo.node.metatable");
}

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_concurrent_task_fifo_start(lua_State *L) {
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
    params = json::encode(L);
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
arken_concurrent_task_fifo_max(lua_State *L) {
  if(lua_gettop(L) == 1) { /* número de argumentos */
    int max = luaL_checkinteger(L, 1);
    fifo::max() = max;
    return 0;
  } else {
    lua_pushinteger(L, fifo::max());
    return 1;
  }
}

static int
arken_concurrent_task_fifo_actives(lua_State *L) {
  lua_pushinteger(L, fifo::actives());
  return 1;
}

static int
arken_concurrent_task_fifo_inspect( lua_State *L ) {
  lua_pushstring(L, fifo::inspect());
  return 1;
}

static const luaL_reg arken_concurrent_task_fifo[] = {
  {"start",   arken_concurrent_task_fifo_start},
  {"max",     arken_concurrent_task_fifo_max},
  {"actives", arken_concurrent_task_fifo_actives},
  {"inspect", arken_concurrent_task_fifo_inspect},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_task_fifo( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.task.fifo");
  luaL_register(L, nullptr, arken_concurrent_task_fifo);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_task_fifo_node_uuid( lua_State *L ) {
  fifo::node * node = checkNode( L );
  lua_pushlstring(L, node->uuid(), 36);
  return 1;
}

static int
arken_concurrent_task_fifo_node_microtime( lua_State *L ) {
  fifo::node * node = checkNode( L );
  lua_pushnumber(L, node->microtime());
  return 1;
}

static int
arken_concurrent_task_fifo_node_shared( lua_State *L ) {
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
arken_concurrent_task_fifo_node_finished( lua_State *L ) {
  fifo::node * node = checkNode( L );
  lua_pushboolean(L, node->finished());
  return 1;
}

static int
arken_concurrent_task_fifo_node_wait( lua_State *L ) {
  fifo::node * node = checkNode( L );
  node->wait();
  return 0;
}

static int
arken_concurrent_task_fifo_node_gc( lua_State *L ) {
  fifo::node * node = checkNode( L );
  delete node;
  return 0;
}

static const
luaL_reg arken_concurrent_task_fifo_node_metatable[] = {
  {"uuid",      arken_concurrent_task_fifo_node_uuid},
  {"microtime", arken_concurrent_task_fifo_node_microtime},
  {"shared",    arken_concurrent_task_fifo_node_shared},
  {"finished",  arken_concurrent_task_fifo_node_finished},
  {"wait",      arken_concurrent_task_fifo_node_wait},
  {"__gc",      arken_concurrent_task_fifo_node_gc},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_task_fifo_node_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.task.fifo.node.metatable");
  luaL_register(L, nullptr, arken_concurrent_task_fifo_node_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_task_fifo( lua_State *L ) {
    register_arken_concurrent_task_fifo_node_metatable(L);
    register_arken_concurrent_task_fifo(L);
    return 1;
  }
}
