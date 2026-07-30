// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CRYPTO_AES_
#define _ARKEN_CRYPTO_AES_

namespace arken {
namespace crypto {

class aes {
public:
  static char *encrypt(const char *value, int length, const char *password);
  static char *decrypt(const char *value, int length, const char *password);
};

} // namespace crypto
} // namespace arken

#endif
