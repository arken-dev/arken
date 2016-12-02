#include <lua/lua.hpp>
#include <oberon/helper>
#include <oberon/modules/http.h>

static int lua_oberon_http_read( lua_State *L ) {
  const char *path = luaL_checkstring(L, 1);
  lua_pushstring( L, http::read(path) );
  return 1;
}

static int lua_oberon_http_get( lua_State *L ) {
  const char *path = luaL_checkstring(L, 1);
  lua_pushstring( L, http::get(path) );
  return 1;
}

static int lua_oberon_http_post( lua_State *L ) {
  const char *path = luaL_checkstring(L, 1);
  lua_pushstring( L, http::post(path) );
  return 1;
}

static int lua_oberon_http_put( lua_State *L ) {
  const char *path = luaL_checkstring(L, 1);
  lua_pushstring( L, http::put(path) );
  return 1;
}

extern "C" {
  int luaopen_oberon_http( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"read", lua_oberon_http_read},
      {"get",  lua_oberon_http_get},
      {"post", lua_oberon_http_post},
      {"put",  lua_oberon_http_put},
      {NULL, NULL}
    };
    luaL_register(L, "http", Map);
    return 1;
  }
}
