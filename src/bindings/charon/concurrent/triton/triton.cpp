// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/concurrent/triton.h>

using triton = charon::concurrent::triton;

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * data);

triton *
checkChannel( lua_State *L ) {
  return *(triton **) luaL_checkudata(L, 1, "charon.concurrent.triton.metatable");
}

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
charon_triton_start(lua_State *L) {
  bool release = false;
  const char * fileName = luaL_checkstring(L, 1);
  char * data;

  if(lua_gettop(L) == 1) { /* número de argumentos */
    data = new char[3]{'{','}','\0'};
  } else {
    if(lua_gettop(L) == 3) { // number of arguments
      release = lua_toboolean(L, 3);
      lua_settop(L, 2);
    }
    data = json_lock_encode(L);
  }
  triton::start( fileName, data, release );
  return 1;
}

static int
charon_triton_wait(lua_State *L) {
  triton::wait();
  return 0;
}

static const luaL_reg TaskClassMethods[] = {
  {"start", charon_triton_start},
  {"wait",  charon_triton_wait},
  {NULL, NULL}
};

void static
registerChannelClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "charon.concurrent.triton");
  luaL_register(L, NULL, TaskClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
charon_concurrent_triton_instance_method_enqueue( lua_State *L ) {
  triton * pointer = checkChannel( L );
  const char *str = luaL_checkstring(L, 2);
  pointer->enqueue(str);
  return 0;
}

static int
charon_concurrent_triton_instance_method_count( lua_State *L ) {
  triton * pointer = checkChannel( L );
  const char * key = luaL_checkstring(L, 2);
  pointer->count(key);
  return 0;
}

static int
charon_concurrent_triton_instance_method_total( lua_State *L ) {
  triton * pointer = checkChannel( L );
  const char * label = luaL_checkstring(L, 2);
  lua_pushinteger(L, pointer->total(label));
  return 1;
}

static int
charon_concurrent_triton_instance_method_append( lua_State *L ) {
  triton * pointer = checkChannel( L );
  const char * key    = luaL_checkstring(L, 2);
  const char * result = luaL_checkstring(L, 3);
  pointer->append(key, result);
  return 0;
}

static int
charon_concurrent_triton_instance_method_result( lua_State *L ) {
  triton * pointer = checkChannel( L );
  const char * key = luaL_checkstring(L, 2);
  lua_pushstring(L, pointer->result(key));

  return 1;
}


static const
luaL_reg ChannelInstanceMethods[] = {
  {"enqueue", charon_concurrent_triton_instance_method_enqueue},
  {"count",   charon_concurrent_triton_instance_method_count},
  {"total",   charon_concurrent_triton_instance_method_total},
  {"append",  charon_concurrent_triton_instance_method_append},
  {"result",  charon_concurrent_triton_instance_method_result},
  {NULL, NULL}
};

void static
registerChannelInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "charon.concurrent.triton.metatable");
  luaL_register(L, NULL, ChannelInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_charon_concurrent_triton( lua_State *L ) {
    registerChannelInstanceMethods(L);
    registerChannelClassMethods(L);
    return 1;
  }
  int
  luaopen_arken_concurrent_triton( lua_State *L ) {
    registerChannelInstanceMethods(L);
    registerChannelClassMethods(L);
    return 1;
  }
}
