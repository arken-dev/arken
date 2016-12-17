// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <CByteArray>
#include <QDebug>
#include <QThread>
#include <charon/helper>
#include "mirandastate.h"

extern "C" {
#include <json.h>
}

char * json_lock_encode(lua_State *l);
void json_lock_decode(lua_State *l, const char * data);

static int
miranda_task_value(lua_State *L) {

  const char * uuid = luaL_checkstring(L, 1);
  const char * data = MirandaState::value(uuid);

  json_lock_decode(L, data);

  return 1;
}

static int
miranda_task_start(lua_State *L) {
  const char * file_name = luaL_checkstring(L, 1);
  const char * uuid = os::uuid();
  char * result = json_lock_encode(L);
  MirandaState::insert(uuid, result);
  MirandaState::createTask( file_name, uuid );
  lua_pushstring(L, uuid);
  delete[] uuid;
  return 1;
}

static int
miranda_task_pool(lua_State *L) {
  const char * file_name = luaL_checkstring(L, 1);
  const char * uuid = os::uuid();
  MirandaState::insert(uuid, json_lock_encode(L));
  MirandaState::taskPool( file_name, uuid );
  lua_pushstring(L, uuid);
  delete[] uuid;
  return 1;
}


static int
miranda_task_insert(lua_State *L) {
  const char * uuid = luaL_checkstring(L, 1);
  MirandaState::insert(uuid, json_lock_encode(L));
  return 0;
}

void
miranda_task_register(lua_State * L) {
  static const         luaL_reg Map[] = {
    {"pool",    miranda_task_pool},
    {"value",   miranda_task_value},
    {"start",   miranda_task_start},
    {"insert",  miranda_task_insert},
    {NULL, NULL}
  };

  luaL_register(L, "task", Map);
}
