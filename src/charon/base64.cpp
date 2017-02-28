// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>

extern "C" {
  #include <ap_base64.h>
}

using namespace charon;

char * base64::decode(const char * encoded)
{
  size_t decode_len = Base64decode_len(encoded);
  char * plain = new char[decode_len];
  Base64decode(plain, encoded);
  return plain;
}

char * base64::encode(const char * plain)
{
  int len = strlen(plain);
  return base64::encode(plain, len);
}

char * base64::encode(const char * plain, int len)
{
  int encoded_len = Base64encode_len(len);
  char * encoded  = new char[encoded_len];
  Base64encode(encoded, plain, len);
  return encoded;
}
