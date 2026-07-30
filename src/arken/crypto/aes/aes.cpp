// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/crypto/aes.h>

namespace arken {
namespace crypto {

char *aes::encrypt(const char *string, int length, const char *password)
{
  return new char[18]{ "encrypt ola mundo" };
}

char *aes::decrypt(const char *string, int length, const char *password)
{
  return new char[18]{ "decrypt ola mundo" };
}

} // namespace crypto
} // namespace arken
