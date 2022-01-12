// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/task.h>

using task = arken::concurrent::task;
using Shared = arken::concurrent::Shared;

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * data);

task *
checkTask( lua_State *L ) {
  return *(task **) luaL_checkudata(L, 1, "arken.concurrent.task.metatable");
}

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_concurrent_task_start(lua_State *L) {
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
  task tsk = task::start( fileName, params, release );

  task **ptr = (task **)lua_newuserdata(L, sizeof(task*));
  *ptr = new task(tsk);

  luaL_getmetatable(L, "arken.concurrent.task.metatable");
  lua_setmetatable(L, -2);

  delete params;

  return 1;
}

static const luaL_reg arken_concurrent_task[] = {
  {"start", arken_concurrent_task_start},
  {NULL, NULL}
};

void static
register_arken_concurrent_task( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.task");
  luaL_register(L, NULL, arken_concurrent_task);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_task_uuid( lua_State *L ) {
  task * pointer = checkTask( L );
  lua_pushstring(L, pointer->uuid());
  return 1;
}

static int
arken_concurrent_task_finished( lua_State *L ) {
  task * pointer = checkTask( L );
  lua_pushboolean(L, pointer->finished());

  return 1;
}

static int
arken_concurrent_task_shared( lua_State *L ) {
  task * pointer = checkTask( L );
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
luaL_reg arken_concurrent_task_metatable[] = {
  {"uuid",     arken_concurrent_task_uuid},
  {"shared",   arken_concurrent_task_shared},
  {"finished", arken_concurrent_task_finished},
  {NULL, NULL}
};

void static
register_arken_concurrent_task_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.task.metatable");
  luaL_register(L, NULL, arken_concurrent_task_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_task( lua_State *L ) {
    register_arken_concurrent_task_metatable(L);
    register_arken_concurrent_task(L);
    return 1;
  }
}
