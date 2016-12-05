// Copyright 2016 The Oberon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <oberon/helper>

static int lua_oberon_os_abspath( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushstring( L, os::abspath(path) );
  return 1;
}

static int lua_oberon_os_atime( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushnumber( L, os::atime(path) );
  return 1;
}

static int lua_oberon_os_compare( lua_State *L ) {
  const char * path1 = luaL_checkstring(L, 1);
  const char * path2 = luaL_checkstring(L, 2);

  lua_pushboolean( L, os::compare(path1, path2) );
  return 1;
}

static int lua_oberon_os_copy( lua_State *L ) {
  const char * source      = luaL_checkstring(L, 1);
  const char * destination = luaL_checkstring(L, 2);
  bool force = false;
  if( lua_gettop(L) == 3) { /* número de argumentos */
    force = lua_toboolean(L, 3);
  }

  lua_pushboolean( L, os::copy(source, destination, force) );
  return 1;
}

static int lua_oberon_os_cores( lua_State *L ) {
  lua_pushnumber( L, os::cores() );
  return 1;
}

static int lua_oberon_os_chdir( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushnumber( L, os::chdir(path) );
  return 1;
}

static int lua_oberon_os_ctime( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushnumber( L, os::ctime(path) );
  return 1;
}

static int lua_oberon_os_dirpath( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushstring( L, os::dirpath(path) );
  return 1;
}

static int lua_oberon_os_exists( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::exists(path) );
  return 1;
}

static int lua_oberon_os_glob( lua_State *L ) {
  OStringList * list = 0;
  const char  * path = luaL_checkstring(L, 1);

  if( lua_gettop(L) == 1 ) { /* número de argumentos */
    list = os::glob( path );
  }

  if( lua_gettop(L) == 2 ) { /* número de argumentos */
    if( lua_isboolean(L, 2) ) {
      list = os::glob( path, lua_toboolean(L, 2) );
    } else {
      list = os::glob( path, lua_tostring(L, 2) );
    }
  }

  if( lua_gettop(L) == 3 ) { /* número de argumentos */
    list = os::glob( path, lua_tostring(L, 2), lua_toboolean(L, 3) );
  }

  OStringList **ptr = (OStringList **)lua_newuserdata(L, sizeof(OStringList*));
  *ptr = list;
  luaL_getmetatable(L, "OStringList.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int lua_oberon_os_home( lua_State *L ) {
  lua_pushstring( L, os::home() );
  return 1;
}

static int lua_oberon_os_hostname( lua_State *L ) {
  lua_pushstring( L, os::hostname() );
  return 1;
}

static int lua_oberon_os_link( lua_State *L ) {
  const char * source      = luaL_checkstring(L, 1);
  const char * destination = luaL_checkstring(L, 2);
  bool force = false;
  if( lua_gettop(L) == 3) { /* número de argumentos */
    force = lua_toboolean(L, 3);
  }

  lua_pushboolean( L, os::link(source, destination, force) );
  return 1;
}

static int lua_oberon_os_microtime( lua_State *L ) {
  lua_pushnumber( L, os::microtime() );
  return 1;
}

static int lua_oberon_os_mkdir( lua_State *L ) {
  const char * dirname = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::mkdir(dirname) );
  return 1;
}

static int lua_oberon_os_mkpath( lua_State *L ) {
  const char * dirpath = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::mkpath(dirpath) );
  return 1;
}

static int lua_oberon_os_name( lua_State *L ) {
  lua_pushstring( L, os::name() );
  return 1;
}

static int lua_oberon_os_pwd( lua_State *L ) {
  lua_pushstring( L, os::pwd() );
  return 1;
}

static int lua_oberon_os_pid( lua_State *L ) {
  lua_pushnumber( L, os::pid() );
  return 1;
}

static int lua_oberon_os_rmdir( lua_State *L ) {
  const char * dirname = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::rmdir(dirname) );
  return 1;
}

static int lua_oberon_os_rmpath( lua_State *L ) {
  const char * dirpath = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::rmpath(dirpath) );
  return 1;
}

static int lua_oberon_os_sleep( lua_State *L ) {
  double secs = luaL_checknumber(L, 1);
  os::sleep( secs );
  return 0;
}

static int lua_oberon_os_target( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushstring( L, os::target(path) );
  return 1;
}

static int lua_oberon_os_temp( lua_State *L ) {
  lua_pushstring( L, os::temp() );
  return 1;
}

static int lua_oberon_os_touch( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::touch(path) );
  return 1;
}

static int lua_oberon_os_uuid( lua_State *L ) {
  char * uuid = os::uuid();
  lua_pushstring( L, uuid );
  delete[] uuid;
  return 1;
}

static int lua_oberon_os_read( lua_State *L ) {
  size_t size = -1;
  const char * path = luaL_checkstring(L, 1);
  const char * raw  = os::read(path, &size);
  if( size != -1u ) {
    lua_pushlstring( L, raw, size );
  } else {
    lua_pushstring( L, raw );
  }
  delete[] raw;
  return 1;
}

static int lua_oberon_os_root( lua_State *L ) {
  lua_pushstring( L, os::root() );
  return 1;
}

int luaopen_oberon_os( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"abspath",    lua_oberon_os_abspath},
    {"atime",      lua_oberon_os_atime},
    {"compare",    lua_oberon_os_compare},
    {"copy",       lua_oberon_os_copy},
    {"cores",      lua_oberon_os_cores},
    {"chdir",      lua_oberon_os_chdir},
    {"ctime",      lua_oberon_os_ctime},
    {"dirpath",    lua_oberon_os_dirpath},
    {"exists",     lua_oberon_os_exists},
    {"glob",       lua_oberon_os_glob},
    {"home",       lua_oberon_os_home},
    {"hostname",   lua_oberon_os_hostname},
    {"link",       lua_oberon_os_link},
    {"microtime",  lua_oberon_os_microtime},
    {"mkdir",      lua_oberon_os_mkdir},
    {"mkpath",     lua_oberon_os_mkpath},
    {"name",       lua_oberon_os_name},
    {"pid",        lua_oberon_os_pid},
    {"pwd",        lua_oberon_os_pwd},
    {"rmdir",      lua_oberon_os_rmdir},
    {"rmpath",     lua_oberon_os_rmpath},
    {"read",       lua_oberon_os_read},
    {"target",     lua_oberon_os_target},
    {"temp",       lua_oberon_os_temp},
    {"touch",      lua_oberon_os_touch},
    {"sleep",      lua_oberon_os_sleep},
    {"uuid",       lua_oberon_os_uuid},
    {"root",       lua_oberon_os_root},
    {NULL, NULL}
  };
  luaL_register(L, "os", Map);
  return 1;
}
