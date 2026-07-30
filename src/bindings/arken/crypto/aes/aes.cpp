// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/crypto/aes.h>

#ifndef ARKEN_CRYPTO_AES_KEY
#define ARKEN_CRYPTO_AES_KEY ""
#endif

using aes = arken::crypto::aes;

static int arken_crypto_aes_encrypt(lua_State *L) {
  size_t valueSize;
  const char *value = luaL_checklstring(L, 1, &valueSize);
  const char *password = luaL_optstring(L, 2, ARKEN_CRYPTO_AES_KEY);
  int bits = luaL_optint(L, 3, 256);

  if (password[0] == '\0')
    return luaL_error(L, "arken.crypto.aes: no password given and ARKEN_CRYPTO_AES_KEY is not configured");

  try {
    arken::string result = aes::encrypt(arken::string(value, valueSize), password, bits);
    lua_pushlstring(L, result.data(), result.size());
  } catch (const char *msg) {
    lua_pushstring(L, msg);
    lua_error(L);
  }

  return 1;
}

static int arken_crypto_aes_decrypt(lua_State *L) {
  size_t valueSize;
  const char *value = luaL_checklstring(L, 1, &valueSize);
  const char *password = luaL_optstring(L, 2, ARKEN_CRYPTO_AES_KEY);
  int bits = luaL_optint(L, 3, 256);

  if (password[0] == '\0')
    return luaL_error(L, "arken.crypto.aes: no password given and ARKEN_CRYPTO_AES_KEY is not configured");

  try {
    arken::string result = aes::decrypt(arken::string(value, valueSize), password, bits);
    lua_pushlstring(L, result.data(), result.size());
  } catch (const char *msg) {
    lua_pushstring(L, msg);
    lua_error(L);
  }

  return 1;
}

void static
register_arken_crypto_aes( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"encrypt", arken_crypto_aes_encrypt},
    {"decrypt", arken_crypto_aes_decrypt},
    {nullptr, nullptr}
  };
  luaL_newmetatable(L, "arken.crypto.aes");
  luaL_register(L, nullptr, Map);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int luaopen_arken_crypto_aes( lua_State *L ) {
    register_arken_crypto_aes(L);
    return 1;
  }
}
