#include <luajit-2.0/lua.hpp>

int luaopen_extensions_os( lua_State *L );

extern "C" {
  int luaopen_extensions( lua_State *L ) {
    luaopen_extensions_os(L);
    return 1;
  }
}
