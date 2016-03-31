#include <luajit-2.0/lua.hpp>
#include <extensions/os.h>

extern "C" {
  int luaopen_extensions( lua_State *L ) {
    luaopen_extensions_os(L);
    return 1;
  }
}
