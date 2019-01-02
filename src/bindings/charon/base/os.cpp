// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>

using List = charon::string::List;

static int charon_os_abspath( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  char * result = os::abspath(path);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int charon_os_basename( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  char * result = os::basename(path);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int charon_os_atime( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushnumber( L, os::atime(path) );
  return 1;
}

static int charon_os_compare( lua_State *L ) {
  const char * path1 = luaL_checkstring(L, 1);
  const char * path2 = luaL_checkstring(L, 2);

  lua_pushboolean( L, os::compare(path1, path2) );
  return 1;
}

static int charon_os_copy( lua_State *L ) {
  const char * source      = luaL_checkstring(L, 1);
  const char * destination = luaL_checkstring(L, 2);
  bool force = false;
  if( lua_gettop(L) == 3) { /* número de argumentos */
    force = lua_toboolean(L, 3);
  }

  lua_pushboolean( L, os::copy(source, destination, force) );
  return 1;
}

static int charon_os_cores( lua_State *L ) {
  lua_pushnumber( L, os::cores() );
  return 1;
}

static int charon_os_chdir( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushnumber( L, os::chdir(path) );
  return 1;
}

static int charon_os_ctime( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushnumber( L, os::ctime(path) );
  return 1;
}

static int charon_os_dirpath( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  char * result = os::dirpath(path);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int charon_os_exists( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::exists(path) );
  return 1;
}

static int charon_os_executablePath( lua_State *L ) {
  char * result = os::executablePath();
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int charon_os_glob( lua_State *L ) {
  List * list = 0;
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

  List **ptr = (List **)lua_newuserdata(L, sizeof(List*));
  *ptr = list;
  luaL_getmetatable(L, "charon.string.List.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int charon_os_home( lua_State *L ) {
  char * result = os::home();
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int charon_os_hostname( lua_State *L ) {
  char * result = os::hostname();
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int charon_os_isfile( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::isfile(path) );
  return 1;
}

static int charon_os_isdir( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::isdir(path) );
  return 1;
}

static int charon_os_islink( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::islink(path) );
  return 1;
}

static int charon_os_link( lua_State *L ) {
  const char * source      = luaL_checkstring(L, 1);
  const char * destination = luaL_checkstring(L, 2);
  bool force = false;
  if( lua_gettop(L) == 3) { /* número de argumentos */
    force = lua_toboolean(L, 3);
  }

  lua_pushboolean( L, os::link(source, destination, force) );
  return 1;
}

static int charon_os_microtime( lua_State *L ) {
  lua_pushnumber( L, os::microtime() );
  return 1;
}

static int charon_os_mkdir( lua_State *L ) {
  const char * dirname = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::mkdir(dirname) );
  return 1;
}

static int charon_os_mkpath( lua_State *L ) {
  const char * dirpath = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::mkpath(dirpath) );
  return 1;
}

static int charon_os_name( lua_State *L ) {
  const char * result = os::name();
  lua_pushstring( L, result );
  return 1;
}

static int charon_os_pwd( lua_State *L ) {
  char * result = os::pwd();
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int charon_os_pid( lua_State *L ) {
  lua_pushnumber( L, os::pid() );
  return 1;
}

static int charon_os_rmdir( lua_State *L ) {
  const char * dirname = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::rmdir(dirname) );
  return 1;
}

static int charon_os_rmpath( lua_State *L ) {
  const char * dirpath = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::rmpath(dirpath) );
  return 1;
}

static int charon_os_sleep( lua_State *L ) {
  double secs = luaL_checknumber(L, 1);
  os::sleep( secs );
  return 0;
}

static int charon_os_target( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  char * result = os::target(path);
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int charon_os_temp( lua_State *L ) {
  char * result = os::temp();
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int charon_os_touch( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::touch(path) );
  return 1;
}

static int charon_os_uuid( lua_State *L ) {
  char * result = os::uuid();
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

static int charon_os_read( lua_State *L ) {
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

static int charon_os_root( lua_State *L ) {
  char * result = os::root();
  lua_pushstring( L, result );
  delete[] result;
  return 1;
}

int luaopen_charon_os( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"abspath",    charon_os_abspath},
    {"atime",      charon_os_atime},
    {"basename",   charon_os_basename},
    {"compare",    charon_os_compare},
    {"copy",       charon_os_copy},
    {"cores",      charon_os_cores},
    {"chdir",      charon_os_chdir},
    {"ctime",      charon_os_ctime},
    {"dirpath",    charon_os_dirpath},
    {"exists",     charon_os_exists},
    {"executablePath", charon_os_executablePath},
    {"glob",       charon_os_glob},
    {"home",       charon_os_home},
    {"hostname",   charon_os_hostname},
    {"isfile",     charon_os_isfile},
    {"isdir",      charon_os_isdir},
    {"islink",     charon_os_islink},
    {"link",       charon_os_link},
    {"microtime",  charon_os_microtime},
    {"mkdir",      charon_os_mkdir},
    {"mkpath",     charon_os_mkpath},
    {"name",       charon_os_name},
    {"pid",        charon_os_pid},
    {"pwd",        charon_os_pwd},
    {"rmdir",      charon_os_rmdir},
    {"rmpath",     charon_os_rmpath},
    {"read",       charon_os_read},
    {"target",     charon_os_target},
    {"temp",       charon_os_temp},
    {"touch",      charon_os_touch},
    {"sleep",      charon_os_sleep},
    {"uuid",       charon_os_uuid},
    {"root",       charon_os_root},
    {NULL, NULL}
  };
  luaL_register(L, "os", Map);
  return 1;
}
