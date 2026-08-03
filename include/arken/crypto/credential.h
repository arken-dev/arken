// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CRYPTO_CREDENTIAL_
#define _ARKEN_CRYPTO_CREDENTIAL_

#include <arken/crypto/aes.h>
#include <arken/string.h>
#include <unordered_map>

namespace arken {

namespace crypto {

  class Credential
  {

    using string = arken::string;

    public:
    static void         load(string fileName);
    static void         save(string fileName);
    static const char * get(string key);
  };

} // namespace crypto
} // namespace arken
#endif
