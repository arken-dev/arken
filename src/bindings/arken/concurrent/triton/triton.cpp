// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/triton.h>

using triton = arken::concurrent::triton;

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * data);

triton *
checkTriton( lua_State *L ) {
  return *(triton **) luaL_checkudata(L, 1, "arken.concurrent.triton.metatable");
}

triton::node *
checkTritonNode( lua_State *L ) {
  return *(triton::node **) luaL_checkudata(L, 1, "arken.concurrent.triton.node.metatable");
}


//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_triton_start(lua_State *L) {
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
  triton::start( fileName, params, release );

  delete params;

  return 1;
}

static int
arken_triton_wait(lua_State *L) {
  triton::wait();
  return 0;
}

static const luaL_reg TaskClassMethods[] = {
  {"start", arken_triton_start},
  {"wait",  arken_triton_wait},
  {NULL, NULL}
};

void static
registerChannelClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.triton");
  luaL_register(L, NULL, TaskClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_triton_instance_method_enqueue( lua_State *L ) {
  triton * pointer = checkTriton( L );
  const char *str = luaL_checkstring(L, 2);
  pointer->enqueue(str);
  return 0;
}

static int
arken_concurrent_triton_instance_method_count( lua_State *L ) {
  triton * pointer = checkTriton( L );
  const char * key = luaL_checkstring(L, 2);
  pointer->count(key);
  return 0;
}

static int
arken_concurrent_triton_instance_method_total( lua_State *L ) {
  triton * pointer   = checkTriton( L );
  const char * label = luaL_checkstring(L, 2);
  lua_pushinteger(L, pointer->total(label));
  return 1;
}

static int
arken_concurrent_triton_instance_method_append( lua_State *L ) {
  triton * pointer    = checkTriton( L );
  const char * key    = luaL_checkstring(L, 2);
  const char * result = luaL_checkstring(L, 3);
  pointer->append(key, result);
  return 0;
}

static int
arken_concurrent_triton_instance_method_result( lua_State *L ) {
  triton * pointer = checkTriton( L );
  const char * key = luaL_checkstring(L, 2);
  lua_pushstring(L, pointer->result(key));

  return 1;
}

//-----------------------------------------------------------------------------
// TRITON INSTANCE METHODS
//-----------------------------------------------------------------------------

static const
luaL_reg ChannelInstanceMethods[] = {
  {"enqueue", arken_concurrent_triton_instance_method_enqueue},
  {"count",   arken_concurrent_triton_instance_method_count},
  {"total",   arken_concurrent_triton_instance_method_total},
  {"append",  arken_concurrent_triton_instance_method_append},
  {"result",  arken_concurrent_triton_instance_method_result},
  {NULL, NULL}
};

void static
registerChannelInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.triton.metatable");
  luaL_register(L, NULL, ChannelInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

//-----------------------------------------------------------------------------
// TRITON NODE INSTANCE METHODS
//-----------------------------------------------------------------------------

static int
arken_concurrent_triton_node_instance_method_count( lua_State *L ) {
  triton::node * pointer = checkTritonNode( L );
  const char * key = luaL_checkstring(L, 2);
  pointer->count(key);
  return 0;
}

static int
arken_concurrent_triton_node_instance_method_total( lua_State *L ) {
  triton::node * pointer   = checkTritonNode( L );
  const char * label = luaL_checkstring(L, 2);
  lua_pushinteger(L, pointer->total(label));
  return 1;
}

static int
arken_concurrent_triton_node_instance_method_append( lua_State *L ) {
  triton::node * pointer    = checkTritonNode( L );
  const char * key    = luaL_checkstring(L, 2);
  const char * result = luaL_checkstring(L, 3);
  pointer->append(key, result);
  return 0;
}

static int
arken_concurrent_triton_node_instance_method_result( lua_State *L ) {
  triton::node * pointer = checkTritonNode( L );
  const char * key       = luaL_checkstring(L, 2);
  lua_pushstring(L, pointer->result(key));

  return 1;
}

static int
arken_concurrent_triton_node_instance_method_number( lua_State *L ) {
  triton::node * pointer = checkTritonNode( L );
  lua_pushinteger(L, pointer->number());

  return 1;
}

static const
luaL_reg TritonNodeInstanceMethods[] = {
  {"count",   arken_concurrent_triton_node_instance_method_count},
  {"total",   arken_concurrent_triton_node_instance_method_total},
  {"append",  arken_concurrent_triton_node_instance_method_append},
  {"result",  arken_concurrent_triton_node_instance_method_result},
  {"number",  arken_concurrent_triton_node_instance_method_number},
  {NULL, NULL}
};

void static
registerTritonNodeInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.triton.node.metatable");
  luaL_register(L, NULL, TritonNodeInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}


extern "C" {
  int
  luaopen_arken_concurrent_triton( lua_State *L ) {
    registerTritonNodeInstanceMethods(L);
    registerChannelInstanceMethods(L);
    registerChannelClassMethods(L);
    return 1;
  }
}
