// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstring>
#include <charon/base>
#include <QCryptographicHash>

using namespace charon::digest;

char * sha1::hash(const char * hash)
{
  return sha1::hash(hash, strlen(hash));
}

char * sha1::hash(const char * hash, int length)
{
  const char * data = QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Sha1).toHex().data();
  char * result = new char[41];
  strcpy(result, data);
  result[40] = '\0';
  return result;
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
