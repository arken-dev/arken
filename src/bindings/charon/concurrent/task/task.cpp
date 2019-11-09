// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/task>

using task = charon::concurrent::task;

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * data);

task *
checkTask( lua_State *L ) {
  return *(task **) luaL_checkudata(L, 1, "charon.concurrent.task.metatable");
}

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
charon_task_start(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  char * data = json_lock_encode(L);
  string uuid = task::start( fileName, data );
  lua_pushlstring(L, uuid.data(), uuid.size());
  return 1;
}

static int
charon_task_set(lua_State *L) {
  int max = luaL_checkinteger(L, 1);
  task::set(max);
  return 0;
}

static int
charon_task_wait(lua_State *L) {
  task::wait();
  return 0;
}

static const luaL_reg TaskClassMethods[] = {
  {"start", charon_task_start},
  {"wait",  charon_task_wait},
  {"set",   charon_task_set},
  {NULL, NULL}
};

void static
registerTaskClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "charon.concurrent.task");
  luaL_register(L, NULL, TaskClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_charon_concurrent_task( lua_State *L ) {
    registerTaskClassMethods(L);
    return 1;
  }
}
