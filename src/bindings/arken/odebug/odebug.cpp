// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>

static int arken_odebug_info( lua_State *L ) {
  lua_Debug info;

  lua_getstack(L, 2, &info);
  lua_getinfo(L, "Sl", &info);

  lua_pushstring(L, info.source);
  lua_pushinteger(L, info.currentline);

  return 2;
}

void static
register_arken_odebug( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"info", arken_odebug_info},
    {nullptr, nullptr}
  };
  luaL_newmetatable(L, "odebug");
  luaL_register(L, nullptr, Map);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int luaopen_arken_odebug( lua_State *L ) {
    register_arken_odebug( L );
    return 1;
  }
}
