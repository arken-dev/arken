// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>

int luaopen_charon_math( lua_State *L );
int luaopen_charon_os( lua_State *L );
int luaopen_charon_string( lua_State *L );

extern "C" {
  int luaopen_charon_base( lua_State *L ) {
    luaopen_charon_math(L);
    luaopen_charon_os(L);
    luaopen_charon_string(L);
    return 1;
  }
}
