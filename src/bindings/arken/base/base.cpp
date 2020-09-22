// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>

int luaopen_arken_math( lua_State *L );
int luaopen_arken_os( lua_State *L );
int luaopen_arken_string( lua_State *L );

extern "C" {
  int luaopen_arken_base( lua_State *L ) {
    luaopen_arken_math(L);
    luaopen_arken_os(L);
    luaopen_arken_string(L);
    return 1;
  }
}
