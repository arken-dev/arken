// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>

using channel = arken::concurrent::channel;

char * json_lock_encode(lua_State *L);
void   json_lock_decode(lua_State *L, const char * data);

channel *
checkChannel( lua_State *L ) {
  return *(channel **) luaL_checkudata(L, 1, "arken.concurrent.channel.metatable");
}

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_channel_start(lua_State *L) {
  bool release = false;
  const char * fileName = luaL_checkstring(L, 1);
  char * data;

  if(lua_gettop(L) == 1) { /* número de argumentos */
    data = new char[3]{'{','}','\0'};
  } else {
    if(lua_gettop(L) == 3) { // number of arguments
      release = lua_toboolean(L, 3);
      lua_settop(L, 2);
    }
    data = json_lock_encode(L);
  }
  channel * chn = channel::start( fileName, data, release );
  channel **ptr = (channel **)lua_newuserdata(L, sizeof(channel*));
  *ptr = chn;
  luaL_getmetatable(L, "arken.concurrent.channel.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
arken_channel_wait(lua_State *L) {
  channel::wait();
  return 0;
}

static const luaL_reg TaskClassMethods[] = {
  {"start", arken_channel_start},
  {"wait",  arken_channel_wait},
  {NULL, NULL}
};

void static
registerChannelClassMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.channel");
  luaL_register(L, NULL, TaskClassMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_channel_instance_method_write( lua_State *L ) {
  channel * chn = checkChannel( L );
  const char *str = luaL_checkstring(L, 2);
  chn->write(str);
  return 0;
}

static int
arken_concurrent_channel_instance_method_read( lua_State *L ) {
  channel * chn = checkChannel( L );
  lua_pushstring(L, chn->read().c_str());
  return 1;
}

static int
arken_concurrent_channel_instance_method_empty( lua_State *L ) {
  channel * chn = checkChannel( L );
  lua_pushboolean(L, chn->empty());
  return 1;
}

static int
arken_concurrent_channel_instance_method_finished( lua_State *L ) {
  channel * chn = checkChannel( L );
  lua_pushboolean(L, chn->finished());
  return 1;
}

static const
luaL_reg ChannelInstanceMethods[] = {
  {"write",     arken_concurrent_channel_instance_method_write},
  {"read",      arken_concurrent_channel_instance_method_read},
  {"empty",     arken_concurrent_channel_instance_method_empty},
  {"finished",  arken_concurrent_channel_instance_method_finished},
  {NULL, NULL}
};

void static
registerChannelInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.channel.metatable");
  luaL_register(L, NULL, ChannelInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_channel( lua_State *L ) {
    registerChannelInstanceMethods(L);
    registerChannelClassMethods(L);
    return 1;
  }
}
