// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <iostream>
#include <lua/lua.hpp>
#include <charon/base>

using charon::mvm;

int main(int argc, char * argv[])
{
  mvm::init(argc, argv);
  charon::instance i = mvm::instance();
  lua_State * L = i.state();

  lua_settop(L, 0);
  int rv;
  lua_getglobal(L, "require");
  lua_pushstring(L, "rhea");
  rv = lua_pcall(L, 1, 1, 0);
  if (rv) {
    fprintf(stderr, "erro no inicio: %s\n", lua_tostring(L, -1));
    throw;
  }

  lua_getfield(L, -1, "run");
  lua_getglobal(L, "arg");

  if( lua_pcall(L, 1, 0, 0) != 0 ) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  } else {
    return 0;
  }

}
