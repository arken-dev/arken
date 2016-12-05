// Copyright 2016 The Oberon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>

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
