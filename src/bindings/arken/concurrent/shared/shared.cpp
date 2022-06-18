// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/concurrent/shared.h>

using Shared = arken::concurrent::Shared;
using string = arken::string;

/**
 * checkShared
 */

Shared *
checkShared( lua_State *L ) {
  return *static_cast<Shared **>(luaL_checkudata(L, 1, "arken.concurrent.Shared.metatable"));
}

//-----------------------------------------------------------------------------
// Class Methods
//-----------------------------------------------------------------------------

static int
arken_concurrent_Shared_global(lua_State *L) {
  Shared instance = Shared::global();

  auto ptr = static_cast<Shared **>(lua_newuserdata(L, sizeof(Shared*)));
  *ptr = new Shared(instance);

  luaL_getmetatable(L, "arken.concurrent.Shared.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const luaL_reg arken_concurrent_Shared[] = {
  {"global", arken_concurrent_Shared_global},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_Shared( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.Shared");
  luaL_register(L, nullptr, arken_concurrent_Shared);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * InstanceMethods
 */

static int
arken_concurrent_Shared_info( lua_State *L ) {
  Shared * shr = checkShared( L );

  if(lua_gettop(L) == 1) { /* número de argumentos */
    lua_pushstring(L, shr->info());
    return 1;
  } else {
    const char * info = luaL_checkstring(L, 2);
    shr->info(info);
    return 0;
  }
}

static int
arken_concurrent_Shared_put( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);

  if( lua_isboolean(L, 3) ) {
    shr->setBool(key, lua_toboolean(L, 3));
    return 0;
  }

  if( lua_isnumber(L, 3) ) {
    shr->setNumber(key, lua_tonumber(L, 3));
    return 0;
  }

  if( lua_isstring(L, 3) ) {
    shr->setString(key, lua_tostring(L, 3));
  }

  return 0;
}

static int
arken_concurrent_Shared_get( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key = luaL_checkstring(L, 2);

  Shared::data data = shr->get(key);

  if( data.flag() == 0 ) {
    lua_pushnil(L);
  }

  if( data.flag() == 1 ) {
    lua_pushboolean(L, data.getBool());
  }

  if( data.flag() == 2 ) {
    lua_pushnumber(L, data.getNumber());
  }

  if( data.flag() == 3 ) {
    string str = data.getString();
    lua_pushlstring(L, str.data(), str.size());
  }

  return 1;
}


static int
arken_concurrent_Shared_setNumber( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  double value = luaL_checknumber(L, 3);
  shr->setNumber(key, value);
  return 0;
}

static int
arken_concurrent_Shared_getNumber( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  lua_pushnumber(L, shr->getNumber(key));
  return 1;
}

static int
arken_concurrent_Shared_increment( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  double value = luaL_checknumber(L, 3);
  lua_pushnumber(L, shr->increment(key, value));
  return 1;
}

static int
arken_concurrent_Shared_setString( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  size_t len;
  const char * value = luaL_checklstring(L, 3, &len);
  shr->setString(key, string(value, len));
  return 0;
}

static int
arken_concurrent_Shared_getString( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  string str = shr->getString(key);
  lua_pushlstring(L, str.data(), str.size());
  return 1;
}

static int
arken_concurrent_Shared_append( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  size_t len;
  string value = luaL_checklstring(L, 3, &len);
  string str = shr->append(key, string(value, len));
  lua_pushlstring(L, str.data(), str.size());
  return 1;
}

static int
arken_concurrent_Shared_prepend( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  size_t len;
  string value = luaL_checklstring(L, 3, &len);
  string str = shr->prepend(key, string(value, len));
  lua_pushlstring(L, str.data(), str.size());
  return 1;
}

static int
arken_concurrent_Shared_setBool( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  bool value = lua_toboolean(L, 3);
  shr->setBool(key, value);
  return 0;
}

static int
arken_concurrent_Shared_getBool( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  bool value = shr->getBool(key);
  lua_pushboolean(L, value);
  return 1;
}

static int
arken_concurrent_Shared_toggle( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  bool value = shr->toggle(key);
  lua_pushboolean(L, value);
  return 1;
}

static int
arken_concurrent_Shared_gc( lua_State *L ) {
  Shared * shr = checkShared( L );
  delete shr;
  return 0;
}

static const
luaL_reg arken_concurrent_Shared_metatable[] = {
  {"info",      arken_concurrent_Shared_info},
  {"put",       arken_concurrent_Shared_put},
  {"get",       arken_concurrent_Shared_get},
  {"setNumber", arken_concurrent_Shared_setNumber},
  {"getNumber", arken_concurrent_Shared_getNumber},
  {"increment", arken_concurrent_Shared_increment},
  {"setString", arken_concurrent_Shared_setString},
  {"getString", arken_concurrent_Shared_getString},
  {"append",    arken_concurrent_Shared_append},
  {"prepend",   arken_concurrent_Shared_prepend},
  {"setBool",   arken_concurrent_Shared_setBool},
  {"getBool",   arken_concurrent_Shared_getBool},
  {"toggle",    arken_concurrent_Shared_toggle},
  {"__gc",      arken_concurrent_Shared_gc},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_Shared_metatable( lua_State *L ) {
  luaL_newmetatable(L,  "arken.concurrent.Shared.metatable");
  luaL_register(L, nullptr, arken_concurrent_Shared_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_arken_concurrent_Shared( lua_State *L ) {
    register_arken_concurrent_Shared_metatable(L);
    register_arken_concurrent_Shared(L);
    return 1;
  }
}
