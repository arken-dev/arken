#include <luajit-2.0/lua.hpp>
#include <oberon/helper>
#include <oberon/modules/md5.h>

static int lua_oberon_md5_hash( lua_State *L ) {
  size_t size;
  const char *path = luaL_checklstring(L, 1, &size);
  lua_pushstring( L, md5::hash(path, size) );
  return 1;
}

extern "C" {
  int luaopen_oberon_md5( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"hash", lua_oberon_md5_hash},
      {NULL, NULL}
    };
    luaL_register(L, "md5", Map);
    return 1;
  }
}
