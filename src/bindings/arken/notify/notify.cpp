// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/notify.h>

using arken::notify;

static int arken_notify_send( lua_State *L ) {
  const char * title = luaL_checkstring(L, 1);
  const char * body  = luaL_checkstring(L, 2);
  if(lua_gettop(L) == 3) { // number of arguments
    const char * icon = luaL_checkstring(L, 3);
    notify::send(title, body, icon);
  } else {
    notify::send(title, body);
  }

  return 0;
}

extern "C" {
  int luaopen_arken_notify( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"send", arken_notify_send},
      {NULL, NULL}
    };
    luaL_newmetatable(L, "arken.notify");
    luaL_register(L, NULL, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
