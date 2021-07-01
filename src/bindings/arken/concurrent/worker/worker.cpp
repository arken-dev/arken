// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/worker.h>

using worker = arken::concurrent::worker;
using Shared = arken::concurrent::Shared;

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * data);

worker *
checkWorker( lua_State *L ) {
  return *(worker **) luaL_checkudata(L, 1, "arken.concurrent.worker.metatable");
}

worker::node *
checkWorkerNode( lua_State *L ) {
  return *(worker::node **) luaL_checkudata(L, 1, "arken.concurrent.worker.node.metatable");
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
    params = json_lock_encode(L);
  }
  string uuid = worker::start( fileName, params, release );
  lua_pushstring(L, uuid);

  delete params;

  return 1;
}

static int
arken_worker_wait(lua_State *L) {
  worker::wait();
  return 0;
}

static const luaL_reg TaskClassMethods[] = {
  {"start", arken_worker_start},
  {"wait",  arken_worker_wait},
  {NULL, NULL}
};

void static
registerChannelClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.worker");
  luaL_register(L, NULL, TaskClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_worker_instance_method_enqueue( lua_State *L ) {
  worker * pointer = checkWorker( L );
  const char *str = luaL_checkstring(L, 2);
  pointer->enqueue(str);
  return 0;
}

static int
arken_concurrent_worker_instance_method_count( lua_State *L ) {
  worker * pointer = checkWorker( L );
  const char * key = luaL_checkstring(L, 2);
  pointer->count(key);
  return 0;
}

static int
arken_concurrent_worker_instance_method_total( lua_State *L ) {
  worker * pointer   = checkWorker( L );
  const char * label = luaL_checkstring(L, 2);
  lua_pushinteger(L, pointer->total(label));
  return 1;
}

static int
arken_concurrent_worker_instance_method_append( lua_State *L ) {
  worker * pointer    = checkWorker( L );
  const char * key    = luaL_checkstring(L, 2);
  const char * result = luaL_checkstring(L, 3);
  pointer->append(key, result);
  return 0;
}

static int
arken_concurrent_worker_instance_method_result( lua_State *L ) {
  worker * pointer = checkWorker( L );
  const char * key = luaL_checkstring(L, 2);
  lua_pushstring(L, pointer->result(key));

  return 1;
}

static int
arken_concurrent_worker_instance_method_uuid( lua_State *L ) {
  worker * pointer = checkWorker( L );
  lua_pushstring(L, pointer->uuid());

  return 1;
}

static int
arken_concurrent_worker_instance_method_shared( lua_State *L ) {
  worker * pointer = checkWorker( L );
  int rv;
  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.concurrent.Shared");
  rv = lua_pcall(L, 1, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  }

  Shared **ptr = (Shared **)lua_newuserdata(L, sizeof(Shared*));
  *ptr = new Shared(pointer->shared());
  luaL_getmetatable(L, "arken.concurrent.Shared.metatable");
  lua_setmetatable(L, -2);

  return 1;
}


//-----------------------------------------------------------------------------
// TRITON INSTANCE METHODS
//-----------------------------------------------------------------------------

static const
luaL_reg ChannelInstanceMethods[] = {
  {"enqueue", arken_concurrent_worker_instance_method_enqueue},
  {"count",   arken_concurrent_worker_instance_method_count},
  {"total",   arken_concurrent_worker_instance_method_total},
  {"append",  arken_concurrent_worker_instance_method_append},
  {"result",  arken_concurrent_worker_instance_method_result},
  {"uuid",    arken_concurrent_worker_instance_method_uuid},
  {"shared",  arken_concurrent_worker_instance_method_shared},
  {NULL, NULL}
};

void static
registerChannelInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.worker.metatable");
  luaL_register(L, NULL, ChannelInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

//-----------------------------------------------------------------------------
// TRITON NODE INSTANCE METHODS
//-----------------------------------------------------------------------------

static int
arken_concurrent_worker_node_instance_method_count( lua_State *L ) {
  worker::node * pointer = checkWorkerNode( L );
  const char * key = luaL_checkstring(L, 2);
  pointer->count(key);
  return 0;
}

static int
arken_concurrent_worker_node_instance_method_total( lua_State *L ) {
  worker::node * pointer   = checkWorkerNode( L );
  const char * label = luaL_checkstring(L, 2);
  lua_pushinteger(L, pointer->total(label));
  return 1;
}

static int
arken_concurrent_worker_node_instance_method_append( lua_State *L ) {
  worker::node * pointer    = checkWorkerNode( L );
  const char * key    = luaL_checkstring(L, 2);
  const char * result = luaL_checkstring(L, 3);
  pointer->append(key, result);
  return 0;
}

static int
arken_concurrent_worker_node_instance_method_result( lua_State *L ) {
  worker::node * pointer = checkWorkerNode( L );
  const char * key       = luaL_checkstring(L, 2);
  lua_pushstring(L, pointer->result(key));

  return 1;
}

static int
arken_concurrent_worker_node_instance_method_number( lua_State *L ) {
  worker::node * pointer = checkWorkerNode( L );
  lua_pushinteger(L, pointer->number());

  return 1;
}

static int
arken_concurrent_worker_node_instance_method_uuid( lua_State *L ) {
  worker::node * pointer = checkWorkerNode( L );
  lua_pushstring(L, pointer->uuid());

  return 1;
}

static int
arken_concurrent_worker_node_instance_method_shared( lua_State *L ) {
  worker::node * node = checkWorkerNode( L );
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



static const
luaL_reg WorkerNodeInstanceMethods[] = {
  {"count",   arken_concurrent_worker_node_instance_method_count},
  {"total",   arken_concurrent_worker_node_instance_method_total},
  {"append",  arken_concurrent_worker_node_instance_method_append},
  {"result",  arken_concurrent_worker_node_instance_method_result},
  {"number",  arken_concurrent_worker_node_instance_method_number},
  {"uuid",    arken_concurrent_worker_node_instance_method_uuid},
  {"shared",  arken_concurrent_worker_node_instance_method_shared},
  {NULL, NULL}
};

void static
registerWorkerNodeInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.worker.node.metatable");
  luaL_register(L, NULL, WorkerNodeInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}


extern "C" {
  int
  luaopen_arken_concurrent_worker( lua_State *L ) {
    registerWorkerNodeInstanceMethods(L);
    registerChannelInstanceMethods(L);
    registerChannelClassMethods(L);
    return 1;
  }
}
