#include <luajit-2.0/lua.hpp>
#include <oberon/helper>

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

static int lua_oberon_os_ctime( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushnumber( L, os::ctime(path) );
  return 1;
}

static int lua_oberon_os_exists( lua_State *L ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean( L, os::exists(path) );
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

static int lua_oberon_os_name( lua_State *L ) {
  lua_pushstring( L, os::name() );
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

static int lua_oberon_os_uuid( lua_State *L ) {
  lua_pushstring( L, os::uuid() );
  return 1;
}

static int lua_oberon_os_read( lua_State *L ) {
  size_t size;
  const char * path = luaL_checkstring(L, 1);
  const char * raw  = os::read(path, &size);
  lua_pushlstring( L, raw, size );
  return 1;
}

int luaopen_oberon_os( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"compare",    lua_oberon_os_compare},
    {"copy",       lua_oberon_os_copy},
    {"cores",      lua_oberon_os_cores},
    {"ctime",      lua_oberon_os_ctime},
    {"exists",     lua_oberon_os_exists},
    {"hostname",   lua_oberon_os_hostname},
    {"link",       lua_oberon_os_link},
    {"microtime",  lua_oberon_os_microtime},
    {"name",       lua_oberon_os_name},
    {"read",       lua_oberon_os_read},
    {"target",     lua_oberon_os_target},
    {"sleep",      lua_oberon_os_sleep},
    {"uuid",       lua_oberon_os_uuid},
    {NULL, NULL}
  };
  luaL_register(L, "os", Map);
  return 1;
}
