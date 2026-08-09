// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/cache>
#include <arken/json.h>

using arken::cache;
using arken::json;

// ---------------------------------------------------------------------------
// marshaling Lua <-> cache::bucket, reaproveitado tanto pelas funções
// top-level (que operam no bucket "default") quanto pelas de cache.bucket(name)
// ---------------------------------------------------------------------------

static int bucket_value( lua_State *L, cache::bucket & bucket ) {
  const char * key = luaL_checkstring(L, 1);
  std::optional<std::string> value = bucket.value(key);
  if( !value.has_value() ) {
    lua_pushnil(L);
  } else {
    json::decode(L, value->c_str());
  }
  return 1;
}

static int bucket_insert( lua_State *L, cache::bucket & bucket ) {
  const char * key   = luaL_checkstring(L, 1);
  int expires = -1;
  if(lua_gettop(L) == 3) { /* número de argumentos */
    expires = lua_tointeger(L, 3);
    lua_remove(L, 3);
  }
  char * value = json::encode(L);
  bucket.insert(key, value, expires);
  delete[] value;
  return 1;
}

static int bucket_remove( lua_State *L, cache::bucket & bucket ) {
  const char * key = luaL_checkstring(L, 1);
  bucket.remove(key);
  return 0;
}

static int bucket_size( lua_State *L, cache::bucket & bucket ) {
  lua_pushnumber(L, bucket.size());
  return 1;
}

static int bucket_gc( lua_State *L, cache::bucket & bucket ) {
  bucket.gc();
  return 0;
}

static int bucket_keys( lua_State *L, cache::bucket & bucket ) {
  const char * pattern = "*";
  if( lua_gettop(L) >= 1 && !lua_isnil(L, 1) ) {
    pattern = luaL_checkstring(L, 1);
  }

  std::vector<std::string> result = bucket.keys(pattern);

  lua_newtable(L);
  for( size_t i = 0; i < result.size(); i++ ) {
    lua_pushstring(L, result[i].c_str());
    lua_rawseti(L, -2, i + 1);
  }
  return 1;
}

static int bucket_maxSize( lua_State *L, cache::bucket & bucket ) {
  if( lua_gettop(L) >= 1 && !lua_isnil(L, 1) ) {
    double bytes = luaL_checknumber(L, 1);
    bucket.maxSize(bytes);
    return 0;
  }
  lua_pushnumber(L, bucket.maxSize());
  return 1;
}

static int bucket_dump( lua_State *L, cache::bucket & bucket ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean(L, bucket.dump(path));
  return 1;
}

static int bucket_load( lua_State *L, cache::bucket & bucket ) {
  const char * path = luaL_checkstring(L, 1);
  lua_pushboolean(L, bucket.load(path));
  return 1;
}

// ---------------------------------------------------------------------------
// funções top-level: cache.value(key), cache.insert(key, value, [expires])...
// operam implicitamente no bucket "default"
// ---------------------------------------------------------------------------

static int arken_cache_value( lua_State *L )
{
  return bucket_value(L, cache::get());
}

static int arken_cache_insert( lua_State *L )
{
  return bucket_insert(L, cache::get());
}

static int arken_cache_remove( lua_State *L )
{
  return bucket_remove(L, cache::get());
}

static int arken_cache_size( lua_State *L )
{
  return bucket_size(L, cache::get());
}

static int arken_cache_gc( lua_State *L )
{
  return bucket_gc(L, cache::get());
}

static int arken_cache_keys( lua_State *L )
{
  return bucket_keys(L, cache::get());
}

static int arken_cache_maxSize( lua_State *L )
{
  return bucket_maxSize(L, cache::get());
}

static int arken_cache_dump( lua_State *L )
{
  return bucket_dump(L, cache::get());
}

static int arken_cache_load( lua_State *L )
{
  return bucket_load(L, cache::get());
}

static int arken_cache_buckets( lua_State *L ) {
  std::vector<std::string> result = cache::bucketNames();

  lua_newtable(L);
  for( size_t i = 0; i < result.size(); i++ ) {
    lua_pushstring(L, result[i].c_str());
    lua_rawseti(L, -2, i + 1);
  }
  return 1;
}

// ---------------------------------------------------------------------------
// cache.bucket(name): devolve uma tabela com os mesmos métodos, presos ao
// bucket "name" (criado na primeira vez que esse nome é usado).
// ---------------------------------------------------------------------------

// nome do bucket é o upvalue 1 de cada closure devolvida por
// arken_cache_bucket
static cache::bucket & namedBucket( lua_State *L )
{
  return cache::get(luaL_checkstring(L, lua_upvalueindex(1)));
}

static int arken_cache_bucket_value( lua_State *L )
{
  return bucket_value(L, namedBucket(L));
}

static int arken_cache_bucket_insert( lua_State *L )
{
  return bucket_insert(L, namedBucket(L));
}

static int arken_cache_bucket_remove( lua_State *L )
{
  return bucket_remove(L, namedBucket(L));
}

static int arken_cache_bucket_size( lua_State *L )
{
  return bucket_size(L, namedBucket(L));
}

static int arken_cache_bucket_gc( lua_State *L )
{
  return bucket_gc(L, namedBucket(L));
}

static int arken_cache_bucket_keys( lua_State *L )
{
  return bucket_keys(L, namedBucket(L));
}

static int arken_cache_bucket_maxSize( lua_State *L )
{
  return bucket_maxSize(L, namedBucket(L));
}

static int arken_cache_bucket_dump( lua_State *L )
{
  return bucket_dump(L, namedBucket(L));
}

static int arken_cache_bucket_load( lua_State *L )
{
  return bucket_load(L, namedBucket(L));
}

// empilha uma closure de fn com "name" (o nome do bucket) como upvalue 1, e
// guarda no campo "field" da tabela no topo da pilha
static void pushBucketMethod( lua_State *L, const char * name,
                               lua_CFunction fn, const char * field )
{
  lua_pushstring(L, name);
  lua_pushcclosure(L, fn, 1);
  lua_setfield(L, -2, field);
}

static int arken_cache_bucket( lua_State *L ) {
  const char * name = luaL_checkstring(L, 1);

  lua_newtable(L);

  pushBucketMethod(L, name, arken_cache_bucket_value,   "value");
  pushBucketMethod(L, name, arken_cache_bucket_insert,  "insert");
  pushBucketMethod(L, name, arken_cache_bucket_remove,  "remove");
  pushBucketMethod(L, name, arken_cache_bucket_size,    "size");
  pushBucketMethod(L, name, arken_cache_bucket_gc,      "gc");
  pushBucketMethod(L, name, arken_cache_bucket_keys,    "keys");
  pushBucketMethod(L, name, arken_cache_bucket_maxSize, "maxSize");
  pushBucketMethod(L, name, arken_cache_bucket_dump,    "dump");
  pushBucketMethod(L, name, arken_cache_bucket_load,    "load");

  return 1;
}

extern "C" {
  int luaopen_arken_cache( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"value",   arken_cache_value},
      {"insert",  arken_cache_insert},
      {"remove",  arken_cache_remove},
      {"size",    arken_cache_size},
      {"gc",      arken_cache_gc},
      {"keys",    arken_cache_keys},
      {"maxSize", arken_cache_maxSize},
      {"dump",    arken_cache_dump},
      {"load",    arken_cache_load},
      {"bucket",  arken_cache_bucket},
      {"buckets", arken_cache_buckets},
      {nullptr, nullptr}
    };
    luaL_newmetatable(L, "arken.cache");
    luaL_register(L, nullptr, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
