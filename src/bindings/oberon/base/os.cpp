#include <luajit-2.0/lua.hpp>
#include <oberon/base>

static int lua_oberon_os_cores( lua_State *L ) {
  lua_pushnumber( L, os::cores() );
  return 1;
}

static int lua_oberon_os_microtime( lua_State *L ) {
  double result = os::microtime();
  lua_pushnumber( L, result );
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

static int lua_oberon_os_uuid( lua_State *L ) {
  lua_pushstring( L, os::uuid() );
  return 1;
}

static int lua_oberon_os_read( lua_State *L ) {
  const char *path = luaL_checkstring(L, 1);
  lua_pushstring( L, os::read(path) );
  return 1;
}

int luaopen_oberon_os( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"cores",      lua_oberon_os_cores},
    {"microtime",  lua_oberon_os_microtime},
    {"name",       lua_oberon_os_name},
    {"read",       lua_oberon_os_read},
    {"sleep",      lua_oberon_os_sleep},
    {"uuid",       lua_oberon_os_uuid},
    {NULL, NULL}
  };
  luaL_register(L, "os", Map);
  return 1;
}
