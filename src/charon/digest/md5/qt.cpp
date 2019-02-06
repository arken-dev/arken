// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstring>
#include <charon/base>
#include <QCryptographicHash>

using namespace charon::digest;

char * md5::hash(const char * hash)
{
  return md5::hash(hash, strlen(hash));
}

char * md5::hash(const char * hash, int length)
{
  const char * data = QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Md5).toHex().data();
  char * result = new char[33];
  strcpy(result, data);
  result[32] = '\0';
  return result;
}
