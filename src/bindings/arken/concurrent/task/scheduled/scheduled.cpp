// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/task/scheduled.h>

using scheduled = arken::concurrent::task::scheduled;
using Shared = arken::concurrent::Shared;

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * data);

scheduled *
checkNaiad( lua_State *L ) {
  return *(scheduled **) luaL_checkudata(L, 1, "arken.concurrent.task.scheduled.metatable");
}

scheduled::node *
checkNode( lua_State *L ) {
  return *(scheduled::node **) luaL_checkudata(L, 1, "arken.concurrent.task.scheduled.node.metatable");
}

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_scheduled_start(lua_State *L) {
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
    params = json_lock_encode(L);
  }

  scheduled::node node = scheduled::start( fileName, params, name, release );
  scheduled::node **ptr = (scheduled::node **)lua_newuserdata(L, sizeof(scheduled::node*));
  *ptr = new scheduled::node(node);
  luaL_getmetatable(L, "arken.concurrent.task.scheduled.node.metatable");
  lua_setmetatable(L, -2);

  delete params;

  return 1;
}

static int
arken_scheduled_max(lua_State *L) {
  if(lua_gettop(L) == 1) { /* número de argumentos */
    int max = luaL_checkinteger(L, 1);
    scheduled::max() = max;
    return 0;
  } else {
    lua_pushinteger(L, scheduled::max());
    return 1;
  }
}

static int
arken_scheduled_actives(lua_State *L) {
  lua_pushinteger(L, scheduled::actives());
  return 1;
}

static int
arken_scheduled_inspect( lua_State *L ) {
  lua_pushstring(L, scheduled::inspect());
  return 1;
}

static const luaL_reg NaiadClassMethods[] = {
  {"start",   arken_scheduled_start},
  {"max",     arken_scheduled_max},
  {"actives", arken_scheduled_actives},
  {"inspect", arken_scheduled_inspect},
  {NULL, NULL}

};

void static
registerNaiadClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.task.scheduled");
  luaL_register(L, NULL, NaiadClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_channel_node_instance_method_uuid( lua_State *L ) {
  scheduled::node * node = checkNode( L );
  lua_pushlstring(L, node->uuid(), 36);
  return 1;
}

static int
arken_concurrent_channel_node_instance_method_name( lua_State *L ) {
  scheduled::node * node = checkNode( L );
  lua_pushstring(L, node->name());
  return 1;
}

static int
arken_concurrent_channel_node_instance_method_microtime( lua_State *L ) {
  scheduled::node * node = checkNode( L );
  lua_pushnumber(L, node->microtime());
  return 1;
}

static int
arken_concurrent_channel_node_instance_method_shared( lua_State *L ) {
  scheduled::node * node = checkNode( L );
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
  scheduled::node * node = checkNode( L );
  lua_pushboolean(L, node->finished());
  return 1;
}

static int
arken_concurrent_channel_node_instance_method_wait( lua_State *L ) {
  scheduled::node * node = checkNode( L );
  node->wait();
  return 0;
}

static int
arken_concurrent_channel_node_instance_method_destruct( lua_State *L ) {
  scheduled::node * node = checkNode( L );
  delete node;
  return 0;
}

static const
luaL_reg NaiadNodeInstanceMethods[] = {
  {"uuid",      arken_concurrent_channel_node_instance_method_uuid},
  {"name",      arken_concurrent_channel_node_instance_method_name},
  {"microtime", arken_concurrent_channel_node_instance_method_microtime},
  {"shared",    arken_concurrent_channel_node_instance_method_shared},
  {"finished",  arken_concurrent_channel_node_instance_method_finished},
  {"wait",      arken_concurrent_channel_node_instance_method_wait},
  {"__gc",      arken_concurrent_channel_node_instance_method_destruct},
  {NULL, NULL}
};

void static
registerNodeInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.task.scheduled.node.metatable");
  luaL_register(L, NULL, NaiadNodeInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_task_scheduled( lua_State *L ) {
    registerNodeInstanceMethods(L);
    registerNaiadClassMethods(L);
    return 1;
  }
}
