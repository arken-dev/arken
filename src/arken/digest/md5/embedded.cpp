// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <fstream>
#include <arken/base>
#include <arken/digest/md5.h>

extern "C" {
#include <digest/md5.h>
}

namespace arken {
namespace digest {

char * md5::hash(const char * hash, int length)
{
  unsigned char x[16];
  auto out = new char[33];
  int i;
  md5_buffer (hash, length, x);
  for (i = 0; i < 16; i++)
    sprintf (out + 2 * i, "%02x", x[i]);

  out[32] = '\0';

  return out;
}

char * md5::file(const char * path)
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

} // namespace digest
} // namespace arken
