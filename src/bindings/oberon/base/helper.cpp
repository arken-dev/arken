#include <luajit-2.0/lua.hpp>

int luaopen_oberon_math( lua_State *L );
int luaopen_oberon_os( lua_State *L );
int luaopen_oberon_string( lua_State *L );

extern "C" {
  int luaopen_oberon_helper( lua_State *L ) {
    luaopen_oberon_math(L);
    luaopen_oberon_os(L);
    luaopen_oberon_string(L);
    return 1;
  }
}
