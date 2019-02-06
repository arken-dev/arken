// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
extern "C" {
#include <digest/md5.h>
}

using namespace charon::digest;

char * md5::hash(const char * hash)
{
  return md5::hash(hash, strlen(hash));
}

char * md5::hash(const char * hash, int length)
{
  unsigned char x[16];
  char * out = new char[33];
  int i;
  md5_buffer (hash, length, x);
  for (i = 0; i < 16; i++)
    sprintf (out + 2 * i, "%02x", x[i]);

  out[32] = '\0';

  return out;
}
