// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/cache>
#include <charon/mvm>
#include <QDebug>
#include <QThread>
#include "mirandastate.h"


using charon::ByteArray;
using charon::cache;
using charon::mvm;

char * json_lock_encode(lua_State *L);

static int
miranda_server_gc(lua_State *L) {
  qDebug() << "gc...";
  lua_pushinteger(L, MirandaState::gc());
  return 1;
}

static int
miranda_server_reload(lua_State *) {
  MirandaState::reload();
  mvm::reload();
  qDebug() << "reload: " << MirandaState::version() ;
  return 0;
}

static int
miranda_server_clear(lua_State *L) {
  int result = MirandaState::clear();
  qDebug() << "clear: " << MirandaState::version() ;
  lua_pushnumber(L, result);
  return 1;
}

static int
miranda_server_version(lua_State *L) {
  lua_pushnumber(L, MirandaState::version());
  return 1;
}

static int
miranda_server_task(lua_State *L) {
  const char * file_name = luaL_checkstring(L, 1);
  const char * uuid = os::uuid();
  /*
  if ( lua_isstring(L, 2) ) {
    MirandaState::insert(uuid, lua_tostring(L, 2));
  }
  */
  cache::insert(uuid, json_lock_encode(L));
  MirandaState::createTask( file_name, uuid );
  lua_pushstring(L, uuid);
  delete[] uuid;
  return 1;
}

static int
miranda_server_service(lua_State *L) {
  const char * file_name = luaL_checkstring(L, 1);
  MirandaState::createService( file_name );
  return 0;
}

void
miranda_server_register(lua_State * L) {
  static const         luaL_reg Map[] = {
    {"gc",      miranda_server_gc},
    {"version", miranda_server_version},
    {"reload",  miranda_server_reload},
    {"clear",   miranda_server_clear},
    {"task",    miranda_server_task},
    {"service", miranda_server_service},
    {NULL, NULL}
  };

  luaL_register(L, "server", Map);
}
