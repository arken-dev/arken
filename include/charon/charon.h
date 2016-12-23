// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef CHARON_H
#define CHARON_H

#include <lua/lua.hpp>

class Charon
{
  public:
  static void args(lua_State * L, int argc, char * argv[]);
  static void profile(lua_State * L, const char * path);
  static lua_State * init(int argc, char * argv[], const char * path);
};

#endif // CHARON_H
