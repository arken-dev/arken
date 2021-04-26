// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstring>
#include <fstream>
#include <arken/digest/md5.h>
#include <QCryptographicHash>

namespace arken {
namespace digest {

char * md5::hash(const char * hash)
{
  return md5::hash(hash, strlen(hash));
}

char * md5::hash(const char * hash, int length)
{
  QByteArray hex = QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Md5).toHex();
  const char * data = hex.data();
  char * result = new char[33];
  strncpy(result, data, 32);
  result[32] = '\0';
  return result;
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
