// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
extern "C" {
#include <digest/md5.h>
#include <digest/sha1.h>
}

using namespace charon;

char * digest::md5(const char * hash)
{
  return digest::md5(hash, strlen(hash));
}

char * digest::md5(const char * hash, int length)
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

char * digest::sha1(const char * hash)
{
  return digest::sha1(hash, strlen(hash));
}

char * digest::sha1(const char * hash, int length)
{
  unsigned char x[40];
  char * out = new char[41];
  int i;
  sha1_buffer (hash, length, x);
  for (i = 0; i < 20; i++)
    sprintf (out + 2 * i, "%02x", x[i]);

  out[40] = '\0';

  return out;
}
