#include <luajit-2.0/lua.hpp>
#include <oberon/base>
#include <oberon/http.h>

static int lua_oberon_http_read( lua_State *L ) {
  const char *path = luaL_checkstring(L, 1);
  lua_pushstring( L, http::read(path) );
  return 1;
}

extern "C" {
  int luaopen_oberon_http( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"read", lua_oberon_http_read},
      {NULL, NULL}
    };
    luaL_register(L, "http", Map);
    return 1;
  }
}
