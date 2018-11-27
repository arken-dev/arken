// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/task>

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * data);

using charon::task;

static int
charon_task_value(lua_State *L) {
  const char * uuid = luaL_checkstring(L, 1);
  const char * data = task::value(uuid);
  json_lock_decode(L, data);
  return 1;
}

static int
charon_task_start(lua_State *L) {
  const char * fileName = luaL_checkstring(L, 1);
  char * data = json_lock_encode(L);
  const char * uuid = task::start( fileName, data );
  lua_pushstring(L, uuid);
  delete[] data;
  return 1;
}

static int
charon_task_insert(lua_State *L) {
  const char * uuid;
  char * data = 0;
  if(lua_gettop(L) == 2) { // número de argumentos
    uuid = luaL_checkstring(L, 1);
    data = json_lock_encode(L);
  } else {
    data = json_lock_encode(L);
    lua_getglobal(L, "CHARON_UUID");
    uuid = lua_tostring(L, -1);
  }
  task::insert(uuid, data);
  if( data ) {
    delete[] data;
  }
  return 0;
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


extern "C" {
  int luaopen_charon_task( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"set",     charon_task_set},
      {"start",   charon_task_start},
      {"value",   charon_task_value},
      {"insert",  charon_task_insert},
      {"wait",    charon_task_wait},
      {NULL, NULL}
    };
    luaL_newmetatable(L, "task");
    luaL_register(L, NULL, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
