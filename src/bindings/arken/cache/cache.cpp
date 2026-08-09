// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/cache>
#include <arken/json.h>

using arken::cache;
using arken::json;

static int arken_cache_value( lua_State *L ) {
  const char * key = luaL_checkstring(L, 1);
  std::optional<std::string> value = cache::value(key);
  if( !value.has_value() ) {
    lua_pushnil(L);
  } else {
    json::decode(L, value->c_str());
  }
  return 1;
}

static int arken_cache_insert( lua_State *L ) {
  const char * key   = luaL_checkstring(L, 1);
  int expires = -1;
  if(lua_gettop(L) == 3) { /* número de argumentos */
    expires = lua_tointeger(L, 3);
    lua_remove(L, 3);
  }
  char * value = json::encode(L);
  cache::insert(key, value, expires);
  delete[] value;
  return 1;
}

static int arken_cache_remove( lua_State *L ) {
  const char * key   = luaL_checkstring(L, 1);
  cache::remove(key);
  return 0;
}

static int arken_cache_size( lua_State *L ) {
  double result = cache::size();
  lua_pushnumber(L, result);
  return 1;
}

static int arken_cache_gc( lua_State *L ) {
  cache::gc();
  return 0;
}

static int arken_cache_keys( lua_State *L ) {
  const char * pattern = "*";
  if( lua_gettop(L) >= 1 && !lua_isnil(L, 1) ) {
    pattern = luaL_checkstring(L, 1);
  }

  std::vector<std::string> result = cache::keys(pattern);

  lua_newtable(L);
  for( size_t i = 0; i < result.size(); i++ ) {
    lua_pushstring(L, result[i].c_str());
    lua_rawseti(L, -2, i + 1);
  }
  return 1;
}

extern "C" {
  int luaopen_arken_cache( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"value",  arken_cache_value},
      {"insert", arken_cache_insert},
      {"remove", arken_cache_remove},
      {"size",   arken_cache_size},
      {"gc",     arken_cache_gc},
      {"keys",   arken_cache_keys},
      {nullptr, nullptr}
    };
    luaL_newmetatable(L, "arken.cache");
    luaL_register(L, nullptr, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
