#include <luajit-2.0/lua.hpp>
#include <oberon/helper>
#include <oberon/modules/base64.h>

static int lua_oberon_base64_decode( lua_State *L ) {
  const char * data = luaL_checkstring(L, 1);
  lua_pushstring( L, base64::decode(data) );
  return 1;
}

static int lua_oberon_base64_encode( lua_State *L ) {
  size_t size;
  const char * data = luaL_checklstring(L, 1, &size);
  char * encoded = base64::encode(data, size);
  lua_pushstring( L, encoded );
  delete[] encoded;
  return 1;
}

extern "C" {
  int luaopen_oberon_base64( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"encode", lua_oberon_base64_encode},
      {"decode", lua_oberon_base64_decode},
      {NULL, NULL}
    };
    luaL_register(L, "base64", Map);
    return 1;
  }
}
