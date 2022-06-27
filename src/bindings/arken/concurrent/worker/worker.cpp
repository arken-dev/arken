// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/worker.h>
#include <arken/json.h>

using worker = arken::concurrent::worker;
using Shared = arken::mvm::Shared;
using json   = arken::json;

worker *
checkWorker( lua_State *L ) {
  return *static_cast<worker **>(luaL_checkudata(L, 1, "arken.concurrent.worker.metatable"));
}

worker::node *
checkWorkerNode( lua_State *L ) {
  return *static_cast<worker::node **>(luaL_checkudata(L, 1, "arken.concurrent.worker.node.metatable"));
}


//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_worker_start(lua_State *L) {
  bool release = false;
  const char * fileName = luaL_checkstring(L, 1);
  char * params;

  if(lua_gettop(L) == 1) { /* número de argumentos */
    params = new char[3]{'{','}','\0'};
  } else {
    if(lua_gettop(L) == 3) { // number of arguments
      release = lua_toboolean(L, 3);
      lua_settop(L, 2);
    }
    params = json::encode(L);
  }

  worker wrk = worker::start( fileName, params, release );
  auto ptr = static_cast<worker **>(lua_newuserdata(L, sizeof(worker*)));
  *ptr = new worker(wrk);

  luaL_getmetatable(L, "arken.concurrent.worker.metatable");
  lua_setmetatable(L, -2);

  delete[] params;

  return 1;
}

static int
arken_worker_max(lua_State *L) {
  int max = luaL_checkinteger(L, 1);
  worker::s_max = max;
  return 0;
}

static const luaL_reg TaskClassMethods[] = {
  {"start", arken_worker_start},
  {"max",   arken_worker_max},
  {nullptr, nullptr}
};

void static
registerWorkerClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.worker");
  luaL_register(L, nullptr, TaskClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_worker_enqueue( lua_State *L ) {
  worker * pointer = checkWorker( L );
  char * data = json::encode(L);
  pointer->enqueue(data);
  delete[] data;
  return 0;
}

static int
arken_concurrent_worker_uuid( lua_State *L ) {
  worker * pointer = checkWorker( L );
  lua_pushstring(L, pointer->uuid());

  return 1;
}

static int
arken_concurrent_worker_progress( lua_State *L ) {
  worker * pointer = checkWorker( L );
  lua_pushnumber(L, pointer->progress());

  return 1;
}

static int
arken_concurrent_worker_finished( lua_State *L ) {
  worker * pointer = checkWorker( L );
  lua_pushboolean(L, pointer->finished());

  return 1;
}


static int
arken_concurrent_worker_shared( lua_State *L ) {
  worker * pointer = checkWorker( L );
  int rv;
  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.mvm");
  rv = lua_pcall(L, 1, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  }

  auto ptr = static_cast<Shared **>(lua_newuserdata(L, sizeof(Shared*)));
  *ptr = new Shared(pointer->shared());
  luaL_getmetatable(L, "arken.mvm.Shared.metatable");
  lua_setmetatable(L, -2);

  return 1;
}


//-----------------------------------------------------------------------------
// TRITON INSTANCE METHODS
//-----------------------------------------------------------------------------

static const
luaL_reg arken_concurrent_worker_metatable[] = {
  {"enqueue",  arken_concurrent_worker_enqueue},
  {"uuid",     arken_concurrent_worker_uuid},
  {"shared",   arken_concurrent_worker_shared},
  {"progress", arken_concurrent_worker_progress},
  {"finished", arken_concurrent_worker_finished},
  {nullptr, nullptr}
};

void static
registerWorkerInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L,  "arken.concurrent.worker.metatable");
  luaL_register(L, nullptr, arken_concurrent_worker_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

//-----------------------------------------------------------------------------
// TRITON NODE INSTANCE METHODS
//-----------------------------------------------------------------------------

static int
arken_concurrent_worker_node_number( lua_State *L ) {
  worker::node * pointer = checkWorkerNode( L );
  lua_pushinteger(L, pointer->number());

  return 1;
}

static int
arken_concurrent_worker_node_uuid( lua_State *L ) {
  worker::node * pointer = checkWorkerNode( L );
  lua_pushstring(L, pointer->uuid());

  return 1;
}

static int
arken_concurrent_worker_node_shared( lua_State *L ) {
  worker::node * node = checkWorkerNode( L );
  int rv;
  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.mvm");
  rv = lua_pcall(L, 1, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  }

  auto ptr = static_cast<Shared **>(lua_newuserdata(L, sizeof(Shared*)));
  *ptr = new Shared(node->shared());
  luaL_getmetatable(L, "arken.mvm.Shared.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_concurrent_worker_node_master( lua_State *L ) {
  worker::node * node = checkWorkerNode( L );

  auto ptr = static_cast<worker **>(lua_newuserdata(L, sizeof(worker*)));
  *ptr = new worker(node->master());

  luaL_getmetatable(L, "arken.concurrent.worker.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const
luaL_reg arken_concurrent_worker_node_metatable[] = {
  {"number",  arken_concurrent_worker_node_number},
  {"uuid",    arken_concurrent_worker_node_uuid},
  {"shared",  arken_concurrent_worker_node_shared},
  {"master",  arken_concurrent_worker_node_master},
  {nullptr, nullptr}
};

void static
registerWorkerNodeInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L,  "arken.concurrent.worker.node.metatable");
  luaL_register(L, nullptr, arken_concurrent_worker_node_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}


extern "C" {
  int
  luaopen_arken_concurrent_worker( lua_State *L ) {
    registerWorkerNodeInstanceMethods(L);
    registerWorkerInstanceMethods(L);
    registerWorkerClassMethods(L);
    return 1;
  }
}
