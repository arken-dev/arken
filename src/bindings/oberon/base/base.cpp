#include <luajit-2.0/lua.hpp>

int luaopen_oberon_os( lua_State *L );

extern "C" {
  int luaopen_oberon_base( lua_State *L ) {
    luaopen_oberon_os(L);
    return 1;
  }
}
