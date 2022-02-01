// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_JSON_
#define _ARKEN_JSON_

#include <lua/lua.hpp>
#include <lua/json/lock.h>

namespace arken
{

class json {
  public:
  static char * encode(lua_State *L);
  static void   decode(lua_State *L, const char * data);
};

} // namespace arken

#endif // _ARKEN_JSON_
