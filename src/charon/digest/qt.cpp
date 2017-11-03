// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstring>
#include <charon/base>
#include <QCryptographicHash>

using namespace charon;

char * digest::md5(const char * hash)
{
  return digest::md5(hash, strlen(hash));
}

char * digest::md5(const char * hash, int length)
{
  const char * data = QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Md5).toHex().data();
  char * result = new char[33];
  strcpy(result, data);
  result[32] = '\0';
  return result;
}

char * digest::sha1(const char * hash)
{
  return digest::sha1(hash, strlen(hash));
}

char * digest::sha1(const char * hash, int length)
{
  const char * data = QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Sha1).toHex().data();
  char * result = new char[41];
  strcpy(result, data);
  result[40] = '\0';
  return result;

}
