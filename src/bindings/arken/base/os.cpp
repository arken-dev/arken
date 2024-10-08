// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>

using List = arken::string::List;

static int arken_os_abspath( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  string result = os::abspath(path);
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_basename( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  string result = os::basename(path);
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_atime( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushnumber( L, os::atime(path) );
  return 1;
}

static int arken_os_compare( lua_State *L ) {
  const char * path1 = luaL_checkstring(L, 1);
  const char * path2 = luaL_checkstring(L, 2);
  lua_pushboolean( L, os::compare(path1, path2) );
  return 1;
}

static int arken_os_copy( lua_State *L ) {
  const char * source      = luaL_checkstring(L, 1);
  const char * destination = luaL_checkstring(L, 2);
  bool force = false;
  if( lua_gettop(L) == 3) { /* número de argumentos */
    force = lua_toboolean(L, 3);
  }
  lua_pushboolean( L, os::copy(source, destination, force) );
  return 1;
}

static int arken_os_mem( lua_State *L ) {
  lua_pushnumber( L, os::mem() );
  return 1;
}

static int
arken_os_cext(lua_State *L) {
  const char * cext = os::cext();
  lua_pushstring(L, cext);
  return 1;
}

static int arken_os_cores( lua_State *L ) {
  lua_pushnumber( L, os::cores() );
  return 1;
}

static int arken_os_chdir( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushnumber( L, os::chdir(path) );
  return 1;
}

static int arken_os_ctime( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushnumber( L, os::ctime(path) );
  return 1;
}

static int arken_os_dirpath( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  string result = os::dirpath(path);
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_exists( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::exists(path) );
  return 1;
}

static int arken_os_executablePath( lua_State *L ) {
  string result = os::executablePath();
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_glob( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  List list(0);
  list = os::glob( path );

  auto ptr = static_cast<List **>(lua_newuserdata(L, sizeof(List*)));
  *ptr = new List(std::move(list));
  luaL_getmetatable(L, "arken.string.List.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int arken_os_find( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  List list(0);

  if( lua_gettop(L) == 1 ) { /* número de argumentos */
    list = os::find( path );
  }

  if( lua_gettop(L) == 2 ) { /* número de argumentos */
    list = os::find( path, luaL_checkstring(L, 2) );
  }

  if( lua_gettop(L) == 3 ) { /* número de argumentos */
    list = os::find( path, lua_tostring(L, 2), lua_toboolean(L, 3) );
  }

  auto ptr = static_cast<List **>(lua_newuserdata(L, sizeof(List*)));
  *ptr = new List(std::move(list));
  luaL_getmetatable(L, "arken.string.List.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int arken_os_home( lua_State *L ) {
  string result = os::home();
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_hostname( lua_State *L ) {
  string result = os::hostname();
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_isfile( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::isfile(path) );
  return 1;
}

static int arken_os_isdir( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::isdir(path) );
  return 1;
}

static int arken_os_islink( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::islink(path) );
  return 1;
}

static int arken_os_link( lua_State *L ) {
  const char * source      = luaL_checkstring(L, 1);
  const char * destination = luaL_checkstring(L, 2);
  bool force = false;
  if( lua_gettop(L) == 3) { /* número de argumentos */
    force = lua_toboolean(L, 3);
  }

  lua_pushboolean( L, os::link(source, destination, force) );
  return 1;
}

static int arken_os_microtime( lua_State *L ) {
  lua_pushnumber( L, os::microtime() );
  return 1;
}

static int arken_os_mkdir( lua_State *L ) {
  const char * dirname = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::mkdir(dirname) );
  return 1;
}

static int arken_os_mkpath( lua_State *L ) {
  const char * dirpath = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::mkpath(dirpath) );
  return 1;
}

static int arken_os_name( lua_State *L ) {
  const char * result = os::name();
  lua_pushstring( L, result );
  return 1;
}

static int arken_os_pwd( lua_State *L ) {
  string result = os::pwd();
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_pid( lua_State *L ) {
  lua_pushnumber( L, os::pid() );
  return 1;
}

static int arken_os_rmdir( lua_State *L ) {
  const char * dirname = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::rmdir(dirname) );
  return 1;
}

static int arken_os_rmpath( lua_State *L ) {
  const char * dirpath = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::rmpath(dirpath) );
  return 1;
}

static int arken_os_sleep( lua_State *L ) {
  double secs = luaL_checknumber(L, 1);
  os::sleep( secs );
  return 0;
}

static int arken_os_size( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushnumber( L, os::size(path) );
  return 1;
}

static int arken_os_target( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  string result = os::target(path);
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_tmp( lua_State *L ) {
  string result = os::tmp();
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_touch( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::touch(path) );
  return 1;
}

static int arken_os_uuid( lua_State *L ) {
  string result = os::uuid();
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_read( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  string result = os::read(path);
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_root( lua_State *L ) {
  string result = os::root();
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_md5( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  string result = os::md5(path);
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

static int arken_os_sha1( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  string result = os::sha1(path);
  lua_pushlstring( L, result.data(), result.size() );
  return 1;
}

int luaopen_arken_os( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"abspath",        arken_os_abspath},
    {"atime",          arken_os_atime},
    {"basename",       arken_os_basename},
    {"cext",           arken_os_cext},
    {"compare",        arken_os_compare},
    {"copy",           arken_os_copy},
    {"cores",          arken_os_cores},
    {"chdir",          arken_os_chdir},
    {"ctime",          arken_os_ctime},
    {"dirpath",        arken_os_dirpath},
    {"exists",         arken_os_exists},
    {"executablePath", arken_os_executablePath},
    {"glob",           arken_os_glob},
    {"find",           arken_os_find},
    {"home",           arken_os_home},
    {"hostname",       arken_os_hostname},
    {"isfile",         arken_os_isfile},
    {"isdir",          arken_os_isdir},
    {"islink",         arken_os_islink},
    {"link",           arken_os_link},
    {"mem",            arken_os_mem},
    {"microtime",      arken_os_microtime},
    {"mkdir",          arken_os_mkdir},
    {"mkpath",         arken_os_mkpath},
    {"name",           arken_os_name},
    {"pid",            arken_os_pid},
    {"pwd",            arken_os_pwd},
    {"rmdir",          arken_os_rmdir},
    {"rmpath",         arken_os_rmpath},
    {"read",           arken_os_read},
    {"target",         arken_os_target},
    {"tmp",            arken_os_tmp},
    {"touch",          arken_os_touch},
    {"sleep",          arken_os_sleep},
    {"size",           arken_os_size},
    {"uuid",           arken_os_uuid},
    {"root",           arken_os_root},
    {"md5",            arken_os_md5},
    {"sha1",           arken_os_sha1},
    {nullptr, nullptr}
  };
  luaL_register(L, "os", Map);
  return 1;
}
