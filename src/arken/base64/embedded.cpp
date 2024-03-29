// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/base64.h>

extern "C" {
  #include <base64/ap_base64.h>
}

namespace arken {

char * base64::decode(const char * encoded, size_t * size)
{
  size_t decode_len = Base64decode_len(encoded);
  auto plain = new char[decode_len];
  *size = Base64decode(plain, encoded);
  return plain;
}

char * base64::encode(const char * plain, int len)
{
  int encoded_len = Base64encode_len(len);
  auto encoded = new char[encoded_len];
  Base64encode(encoded, plain, len);
  return encoded;
}

} // namespace arken
