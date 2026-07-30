// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/crypto/aes.h>

using aes = arken::crypto::aes;

static int arken_crypto_aes_encrypt(lua_State *L) {
  size_t size;
  const char *value = luaL_checklstring(L, 1, &size);
  const char *password = luaL_checklstring(L, 2, &size);
  char *result = aes::encrypt(value, size, password);
  lua_pushlstring(L, result, strlen(result));
  delete[] result;
  return 1;
}

static int arken_crypto_aes_decrypt(lua_State *L) {
  size_t size;
  const char *value = luaL_checklstring(L, 1, &size);
  const char *password = luaL_checklstring(L, 2, &size);
  char *result = aes::decrypt(value, size, password);
  lua_pushlstring(L, result, strlen(result));
  delete[] result;
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
