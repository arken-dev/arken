// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/channel.h>
#include <arken/json.h>

using channel = arken::concurrent::channel;
using Shared  = arken::mvm::Shared;
using json    = arken::json;

channel *
checkChannel( lua_State *L ) {
  return *static_cast<channel **>(luaL_checkudata(L, 1, "arken.concurrent.channel.metatable"));
}

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_concurrent_channel_start(lua_State *L) {
  bool purge = false;
  const char * fileName = luaL_checkstring(L, 1);
  char * params;

  if(lua_gettop(L) == 1) { /* número de argumentos */
    params = new char[3]{'{','}','\0'};
  } else {
    if(lua_gettop(L) == 3) { // number of arguments
      purge = lua_toboolean(L, 3);
      lua_settop(L, 2);
    }
    params = json::encode(L);
  }

  channel * chn = channel::start( fileName, params, purge );
  auto ptr = static_cast<channel **>(lua_newuserdata(L, sizeof(channel*)));
  *ptr = chn;

  luaL_getmetatable(L, "arken.concurrent.channel.metatable");
  lua_setmetatable(L, -2);

  delete[] params;

  return 1;
}

static const luaL_reg arken_concurrent_channel[] = {
  {"start", arken_concurrent_channel_start},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_channel( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.channel");
  luaL_register(L, nullptr, arken_concurrent_channel);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

static int
arken_concurrent_channel_write( lua_State *L ) {
  channel * chn = checkChannel( L );
  const char *str = luaL_checkstring(L, 2);
  chn->write(str);
  return 0;
}

static int
arken_concurrent_channel_read( lua_State *L ) {
  channel * chn = checkChannel( L );
  lua_pushstring(L, chn->read().data());
  return 1;
}

static int
arken_concurrent_channel_empty( lua_State *L ) {
  channel * chn = checkChannel( L );
  lua_pushboolean(L, chn->empty());
  return 1;
}

static int
arken_concurrent_channel_finished( lua_State *L ) {
  channel * chn = checkChannel( L );
  lua_pushboolean(L, chn->finished());
  return 1;
}

static int
arken_concurrent_channel_gc( lua_State *L ) {
  channel * chn = checkChannel( L );
  delete chn;
  return 0;
}

static int
arken_concurrent_channel_uuid( lua_State *L ) {
  channel * chn = checkChannel( L );
  lua_pushstring(L, chn->uuid());
  return 1;
}

static int
arken_concurrent_channel_shared( lua_State *L ) {
  channel * chn = checkChannel( L );
  int rv;
  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.Shared");
  rv = lua_pcall(L, 1, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  }

  auto ptr = static_cast<Shared **>(lua_newuserdata(L, sizeof(Shared*)));
  *ptr = new Shared(chn->shared());
  luaL_getmetatable(L, "arken.Shared.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const
luaL_reg arken_concurrent_channel_metatable[] = {
  {"write",    arken_concurrent_channel_write},
  {"read",     arken_concurrent_channel_read},
  {"empty",    arken_concurrent_channel_empty},
  {"finished", arken_concurrent_channel_finished},
  {"uuid",     arken_concurrent_channel_uuid},
  {"shared",   arken_concurrent_channel_shared},
  {"__gc",     arken_concurrent_channel_gc},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_channel_metatable( lua_State *L ) {
  luaL_newmetatable(L,  "arken.concurrent.channel.metatable");
  luaL_register(L, nullptr, arken_concurrent_channel_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_channel( lua_State *L ) {
    register_arken_concurrent_channel_metatable(L);
    register_arken_concurrent_channel(L);
    return 1;
  }
}
