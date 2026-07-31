// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <arken/base>
#include <arken/crypto/credential.h>
#include <arken/digest/sha1.h>

using Credential = arken::crypto::Credential;

static int arken_crypto_credential_load(lua_State *L) {
  size_t size;
  const char *fileName= luaL_checklstring(L, 1, &size);
  std::cout << "fileName " << fileName << std::endl;
  Credential::load(fileName);
  return 0;
}

void static
register_arken_crypto_credential( lua_State *L ) {
  static const luaL_reg Map[] = {
    {"load", arken_crypto_credential_load},
    {nullptr, nullptr}
  };
  luaL_newmetatable(L, "arken.crypto.credential");
  luaL_register(L, nullptr, Map);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int luaopen_arken_crypto_credential( lua_State *L ) {
    register_arken_crypto_credential(L);
    return 1;
  }
}
