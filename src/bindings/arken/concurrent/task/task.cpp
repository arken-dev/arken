// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/task>

using task = arken::concurrent::task;

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
arken_task_start(lua_State *L) {
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
  string uuid = task::start( fileName, data, release );
  lua_pushlstring(L, uuid.data(), uuid.size());
  return 1;
}

static int
arken_task_wait(lua_State *L) {
  task::wait();
  return 0;
}

static const luaL_reg TaskClassMethods[] = {
  {"start", arken_task_start},
  {"wait",  arken_task_wait},
  {NULL, NULL}
};

void static
registerTaskClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.task");
  luaL_register(L, NULL, TaskClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_task( lua_State *L ) {
    registerTaskClassMethods(L);
    return 1;
  }
}
