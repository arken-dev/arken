#include <luajit-2.0/lua.hpp>
#include <os.h>

static int lua_extensions_os_microtime( lua_State *L ) {
  double result = os::microtime();
  lua_pushnumber( L, result );
  return 1;
}

static int lua_extensions_os_sleep( lua_State *L ) {
  double secs = luaL_checknumber(L, 1);
  os::sleep( secs );
  return 0;
}

static int lua_extensions_os_uuid( lua_State *L ) {
  lua_pushlstring( L, os::uuid(), 37 );
  return 1;
}

int luaopen_extensions_os( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"microtime",  lua_extensions_os_microtime},
    {"sleep",      lua_extensions_os_sleep},
    {"uuid",       lua_extensions_os_uuid},
    {NULL, NULL}
  };
  luaL_register(L, "os", Map);
  return 1;
}
