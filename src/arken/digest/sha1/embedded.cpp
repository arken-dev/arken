// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
extern "C" {
#include <digest/sha1.h>
}

using namespace arken::digest;

char * sha1::hash(const char * hash)
{
  return sha1::hash(hash, strlen(hash));
}

char * sha1::hash(const char * hash, int length)
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

char * sha1::file(const char * path)
{
  char * buffer;
  std::ifstream file;
  int length;
  file.open(path);
  file.seekg(0, std::ios::end);
  length = file.tellg();
  file.seekg(0, std::ios::beg);
  buffer = new char[length];
  file.read(buffer, length);
  file.close();
  char * result = hash(buffer, length);
  delete[] buffer;

  return result;
}
