// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/string.h>
#include <arken/mvm.h>

using Shared = arken::mvm::Shared;
using mvm    = arken::mvm;
using string = arken::string;

/*
base *
checkTask( lua_State *L ) {
  return *static_cast<base **>(luaL_checkudata(L, 1, "arken.concurrent.base.metatable"));
}
*/

/**
 * checkShared
 */

Shared *
checkShared( lua_State *L ) {
  return *static_cast<Shared **>(luaL_checkudata(L, 1, "arken.mvm.Shared.metatable"));
}


mvm::data *
checkData( lua_State *L ) {
  return *static_cast<mvm::data **>(luaL_checkudata(L, 1, "arken.mvm.data.metatable"));
}

//-----------------------------------------------------------------------------
// ARKEN_MVM
//-----------------------------------------------------------------------------

static int
arken_mvm_current(lua_State *L) {
  auto ptr = static_cast<mvm::data **>(lua_newuserdata(L, sizeof(mvm::data *)));
  *ptr = mvm::current();

  luaL_getmetatable(L, "arken.mvm.data.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
arken_mvm_gc(lua_State *L) {
  lua_pushinteger(L, mvm::gc());
  return 1;
}

static int
arken_mvm_reload(lua_State *L) {
  lua_pushnumber(L, mvm::reload());
  return 1;
}

static int
arken_mvm_workers(lua_State *L) {
  lua_pushnumber(L, mvm::workers());
  return 1;
}

static int
arken_mvm_path(lua_State *L) {
  // TODO rename arkenPath => path
  lua_pushstring(L, mvm::path());
  return 1;
}

static int
arken_mvm_clear(lua_State *L) {
  lua_pushnumber(L, mvm::clear());
  return 1;
}

static int
arken_mvm_uptime(lua_State *L) {
  lua_pushnumber(L, mvm::uptime());
  return 1;
}

static int
arken_mvm_version(lua_State *L) {
  lua_pushnumber(L, mvm::version());
  return 1;
}

static int
arken_mvm_set(lua_State *L) {
  const char *key = luaL_checkstring(L, 1);
  int value = luaL_checkinteger(L, 2);
  mvm::set(key, value);
  return 0;
}

static int
arken_mvm_at(lua_State *L) {
  const char *key = luaL_checkstring(L, 1);
  lua_pushnumber(L, mvm::at(key));
  return 1;
}

static int
arken_mvm_pool(lua_State *L) {
  lua_pushnumber(L, mvm::pool());
  return 1;
}

static int
arken_mvm_wait(lua_State *L) {
  mvm::wait();
  return 0;
}

static int
arken_mvm_actives(lua_State *L) {
  lua_pushnumber(L, mvm::actives());
  return 1;
}

static int
arken_mvm_env(lua_State *L) {
  if(lua_gettop(L) == 1) { // number of arguments
    const char *env = luaL_checkstring(L, 1);
    mvm::env(env);
    return 0;
  } else {
    const char * env = mvm::env();
    lua_pushstring(L, env);
    return 1;
  }
}

static int
arken_mvm_threads(lua_State *L) {
  if(lua_gettop(L) == 1) { // number of arguments
    int threads = luaL_checkinteger(L, 1);
    mvm::threads(threads);
    return 0;
  } else {
    int threads = mvm::threads();
    lua_pushinteger(L, threads);
    return 1;
  }
}

static int
arken_mvm_inspect(lua_State *L) {
  lua_pushstring(L, mvm::inspect());
  return 1;
}

static int
arken_mvm_setlocale(lua_State *L) {
  char * result = nullptr;
  if(lua_gettop(L) == 2) { // number of arguments
    const char * locale   = luaL_checkstring(L, 1);
    const char * category = luaL_checkstring(L, 2);
    result = mvm::setlocale(locale, category);
  } else {
    const char * locale   = luaL_checkstring(L, 1);
    result = mvm::setlocale(locale);
  }

  if( result ) {
    lua_pushstring(L, result);
  } else {
    lua_pushnil(L);
  }

  return 1;
}

static int
arken_mvm_shared(lua_State *L) {
  Shared shared = mvm::shared();

  auto ptr = static_cast<Shared **>(lua_newuserdata(L, sizeof(Shared*)));
  *ptr = new Shared(shared);

  luaL_getmetatable(L, "arken.mvm.Shared.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static void
register_arken_mvm( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"current",   arken_mvm_current},
    {"gc",        arken_mvm_gc},
    {"version",   arken_mvm_version},
    {"reload",    arken_mvm_reload},
    {"clear",     arken_mvm_clear},
    {"uptime",    arken_mvm_uptime},
    {"pool",      arken_mvm_pool},
    {"set",       arken_mvm_set},
    {"at",        arken_mvm_at},
    {"wait",      arken_mvm_wait},
    {"path",      arken_mvm_path},
    {"env",       arken_mvm_env},
    {"threads",   arken_mvm_threads},
    {"actives",   arken_mvm_actives},
    {"inspect",   arken_mvm_inspect},
    {"workers",   arken_mvm_workers},
    {"setlocale", arken_mvm_setlocale},
    {"shared",    arken_mvm_shared},
    {nullptr, nullptr}
  };
  luaL_newmetatable(L, "arken.mvm");
  luaL_register(L, nullptr, Map);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

//-----------------------------------------------------------------------------
// ARKEN_CONCURRENT_DATA_METATABLE
//-----------------------------------------------------------------------------

static int
arken_mvm_data_version( lua_State *L ) {
  mvm::data * data = checkData( L );
  lua_pushinteger(L, data->version());
  return 1;
}

static int
arken_mvm_data_release( lua_State *L ) {
  mvm::data * data = checkData( L );
  data->release();
  return 0;
}

static int
arken_mvm_data_gc( lua_State *L ) {
  // mvm::current not collected
  // mvm::data * data = checkData( L );
  // delete data;
  return 0;
}

static int
arken_mvm_data_shared( lua_State *L ) {
  mvm::data * data = checkData( L );

  auto ptr = static_cast<Shared **>(lua_newuserdata(L, sizeof(Shared*)));
  *ptr = new Shared(data->shared());
  luaL_getmetatable(L, "arken.mvm.Shared.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const
luaL_reg arken_mvm_data_metatable[] = {
  {"release",  arken_mvm_data_release},
  {"version",  arken_mvm_data_version},
  {"shared",   arken_mvm_data_shared},
  {"__gc",     arken_mvm_data_gc},
  {nullptr, nullptr}
};

void static
register_arken_mvm_data_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.mvm.data.metatable");
  luaL_register(L, nullptr, arken_mvm_data_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

//-----------------------------------------------------------------------------
// ARKEN_CONCURRENT_BASE_METATABLE
//-----------------------------------------------------------------------------
/*
static int
arken_concurrent_base_uuid( lua_State *L ) {
  base * pointer = checkTask( L );
  lua_pushstring(L, pointer->uuid());
  return 1;
}

static int
arken_concurrent_base_finished( lua_State *L ) {
  base * pointer = checkTask( L );
  lua_pushboolean(L, pointer->finished());

  return 1;
}

static int
arken_concurrent_base_gc( lua_State *L ) {
  base * ptr = checkTask( L );
  delete ptr;
  return 0;
}

static int
arken_concurrent_base_shared( lua_State *L ) {
  base * pointer = checkTask( L );
  int rv;
  lua_getglobal(L, "require");
  lua_pushstring(L, "arken.Shared");
  rv = lua_pcall(L, 1, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  }

  auto ptr = static_cast<Shared **>(lua_newuserdata(L, sizeof(Shared*)));
  *ptr = new Shared(pointer->shared());
  luaL_getmetatable(L, "arken.Shared.metatable");
  lua_setmetatable(L, -2);

  return 1;
}


static const
luaL_reg arken_concurrent_base_metatable[] = {
  {"uuid",     arken_concurrent_base_uuid},
  {"shared",   arken_concurrent_base_shared},
  {"finished", arken_concurrent_base_finished},
  {"__gc",     arken_concurrent_base_gc},
  {nullptr, nullptr}
};

void static
register_arken_concurrent_base_metatable( lua_State *L ) {
  luaL_newmetatable(L, "arken.concurrent.base.metatable");
  luaL_register(L, nullptr, arken_concurrent_base_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}
*/

//-----------------------------------------------------------------------------
// ARKEN_MVM_SHARED
//-----------------------------------------------------------------------------

/**
 * InstanceMethods
 */

static int
arken_mvm_Shared_name( lua_State *L ) {
  Shared * shr = checkShared( L );

  if(lua_gettop(L) == 1) { /* número de argumentos */
    lua_pushstring(L, shr->name());
    return 1;
  } else {
    const char * name = luaL_checkstring(L, 2);
    shr->name(name);
    return 0;
  }
}

static int
arken_mvm_Shared_info( lua_State *L ) {
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
arken_mvm_Shared_put( lua_State *L ) {
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
arken_mvm_Shared_get( lua_State *L ) {
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
arken_mvm_Shared_setNumber( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  double value = luaL_checknumber(L, 3);
  shr->setNumber(key, value);
  return 0;
}

static int
arken_mvm_Shared_getNumber( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  lua_pushnumber(L, shr->getNumber(key));
  return 1;
}

static int
arken_mvm_Shared_increment( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  double value = luaL_checknumber(L, 3);
  lua_pushnumber(L, shr->increment(key, value));
  return 1;
}

static int
arken_mvm_Shared_setString( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  size_t len;
  const char * value = luaL_checklstring(L, 3, &len);
  shr->setString(key, string(value, len));
  return 0;
}

static int
arken_mvm_Shared_getString( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  string str = shr->getString(key);
  lua_pushlstring(L, str.data(), str.size());
  return 1;
}

static int
arken_mvm_Shared_append( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  size_t len;
  string value = luaL_checklstring(L, 3, &len);
  string str = shr->append(key, string(value, len));
  lua_pushlstring(L, str.data(), str.size());
  return 1;
}

static int
arken_mvm_Shared_prepend( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  size_t len;
  string value = luaL_checklstring(L, 3, &len);
  string str = shr->prepend(key, string(value, len));
  lua_pushlstring(L, str.data(), str.size());
  return 1;
}

static int
arken_mvm_Shared_setBool( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char  * key  = luaL_checkstring(L, 2);
  bool value = lua_toboolean(L, 3);
  shr->setBool(key, value);
  return 0;
}

static int
arken_mvm_Shared_getBool( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  bool value = shr->getBool(key);
  lua_pushboolean(L, value);
  return 1;
}

static int
arken_mvm_Shared_toggle( lua_State *L ) {
  Shared * shr = checkShared( L );
  const char * key  = luaL_checkstring(L, 2);
  bool value = shr->toggle(key);
  lua_pushboolean(L, value);
  return 1;
}

static int
arken_mvm_Shared_gc( lua_State *L ) {
  Shared * shr = checkShared( L );
  delete shr;
  return 0;
}

static const
luaL_reg arken_mvm_Shared_metatable[] = {
  {"name",      arken_mvm_Shared_name},
  {"info",      arken_mvm_Shared_info},
  {"put",       arken_mvm_Shared_put},
  {"get",       arken_mvm_Shared_get},
  {"setNumber", arken_mvm_Shared_setNumber},
  {"getNumber", arken_mvm_Shared_getNumber},
  {"increment", arken_mvm_Shared_increment},
  {"setString", arken_mvm_Shared_setString},
  {"getString", arken_mvm_Shared_getString},
  {"append",    arken_mvm_Shared_append},
  {"prepend",   arken_mvm_Shared_prepend},
  {"setBool",   arken_mvm_Shared_setBool},
  {"getBool",   arken_mvm_Shared_getBool},
  {"toggle",    arken_mvm_Shared_toggle},
  {"__gc",      arken_mvm_Shared_gc},
  {nullptr, nullptr}
};

void static
register_arken_mvm_Shared_metatable( lua_State *L ) {
  luaL_newmetatable(L,  "arken.mvm.Shared.metatable");
  luaL_register(L, nullptr, arken_mvm_Shared_metatable);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

//-----------------------------------------------------------------------------
// REGISTER
//-----------------------------------------------------------------------------

extern "C" {
  int luaopen_arken_mvm( lua_State *L ) {
    register_arken_mvm_Shared_metatable(L);
    register_arken_mvm_data_metatable(L);
    register_arken_mvm(L);
    return 1;
  }
}
