// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CRYPTO_AES_
#define _ARKEN_CRYPTO_AES_

#include <arken/string.h>

namespace arken {
namespace crypto {

class aes {

#include <arken/crypto/aes_key.h>

public:
  static string encrypt(string value, const char *password, int bits = 256);
  static string decrypt(string value, const char *password, int bits = 256);
};

} // namespace crypto
} // namespace arken

#endif
