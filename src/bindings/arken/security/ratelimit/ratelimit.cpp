// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/security/ratelimit.h>

using arken::security::RateLimit;

// RateLimit.new(name, limit, seconds) devolve uma tabela de métodos
// presos a `name` -- mesmo esquema de cache.bucket(name)
// (src/bindings/arken/cache/cache.cpp): sem userdata, sem __gc/delete,
// porque a instância por trás de `name` é um registro estático em C++
// que vive pelo tempo do processo (RateLimit::get), compartilhado por
// todas as VMs do pool mvm. Cada chamada resolve `name` de novo via
// RateLimit::get(); chamadas depois da primeira ignoram limit/seconds.
//
// Sintaxe de chamada: ponto, não dois-pontos (bad.exceeded(ip), não
// bad:exceeded(ip)) -- igual ao cache.bucket(); dois-pontos empurraria a
// própria tabela como primeiro argumento no lugar do ip/idle real.

// nome/limit/seconds são os upvalues 1/2/3 de cada closure devolvida por
// arken_security_RateLimit_new
static RateLimit & namedRateLimit( lua_State *L )
{
  const char * name    = luaL_checkstring(L, lua_upvalueindex(1));
  unsigned     limit   = static_cast<unsigned>(lua_tointeger(L, lua_upvalueindex(2)));
  unsigned     seconds = static_cast<unsigned>(lua_tointeger(L, lua_upvalueindex(3)));
  return RateLimit::get(name, limit, seconds);
}

static int
arken_security_RateLimit_exceeded( lua_State *L ) {
  RateLimit & rl  = namedRateLimit(L);
  const char * ip = luaL_checkstring(L, 1);
  lua_pushboolean(L, rl.exceeded(ip));
  return 1;
}

static int
arken_security_RateLimit_clear( lua_State *L ) {
  RateLimit & rl  = namedRateLimit(L);
  const char * ip = luaL_checkstring(L, 1);
  rl.clear(ip);
  return 0;
}

static int
arken_security_RateLimit_gc( lua_State *L ) {
  RateLimit & rl = namedRateLimit(L);
  unsigned idle  = static_cast<unsigned>(luaL_checkinteger(L, 1));
  rl.gc(idle);
  return 0;
}

static int
arken_security_RateLimit_size( lua_State *L ) {
  RateLimit & rl = namedRateLimit(L);
  lua_pushinteger(L, static_cast<lua_Integer>(rl.size()));
  return 1;
}

// empilha uma closure de fn com name/limit/seconds como upvalues 1/2/3, e
// guarda no campo "field" da tabela no topo da pilha
static void pushRateLimitMethod( lua_State *L, const char * name,
                                  unsigned limit, unsigned seconds,
                                  lua_CFunction fn, const char * field ) {
  lua_pushstring(L, name);
  lua_pushinteger(L, limit);
  lua_pushinteger(L, seconds);
  lua_pushcclosure(L, fn, 3);
  lua_setfield(L, -2, field);
}

// nome "new" por convenção (é o que o app chama pra obter o limiter), mas
// a semântica é get-or-create: name já existente devolve a instância que
// já está no registro, limit/seconds só valem na primeira chamada com
// esse name (ver RateLimit::get). Não confundir com um construtor que
// sempre cria algo novo.
static int
arken_security_RateLimit_new( lua_State *L ) {
  const char * name    = luaL_checkstring(L, 1);
  unsigned     limit   = static_cast<unsigned>(luaL_checkinteger(L, 2));
  unsigned     seconds = 60;
  if( lua_gettop(L) >= 3 ) {
    seconds = static_cast<unsigned>(luaL_checkinteger(L, 3));
  }

  // registra já aqui, na hora do new() -- não espera o primeiro
  // exceeded/clear/gc/size. Sem isso, "primeira chamada vence" dependeria de
  // qual closure é invocada primeiro (não determinístico entre VMs),
  // e não de qual new(name, ...) rodou primeiro (a intenção real).
  RateLimit::get(name, limit, seconds);

  lua_newtable(L);

  pushRateLimitMethod(L, name, limit, seconds, arken_security_RateLimit_exceeded, "exceeded");
  pushRateLimitMethod(L, name, limit, seconds, arken_security_RateLimit_clear, "clear");
  pushRateLimitMethod(L, name, limit, seconds, arken_security_RateLimit_gc,    "gc");
  pushRateLimitMethod(L, name, limit, seconds, arken_security_RateLimit_size,  "size");

  return 1;
}

extern "C" {
  int
  luaopen_arken_security_RateLimit( lua_State *L ) {
    static const luaL_reg Map[] = {
      {"new", arken_security_RateLimit_new},
      {nullptr, nullptr}
    };
    luaL_newmetatable(L, "arken.security.RateLimit");
    luaL_register(L, nullptr, Map);
    lua_pushvalue(L, -1);
    lua_setfield(L, -1, "__index");
    return 1;
  }
}
