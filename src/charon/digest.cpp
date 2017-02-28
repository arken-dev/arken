// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <QCryptographicHash>

using namespace charon;

char * digest::md5(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Md5).toHex().data();
}

char * digest::md5(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Md5).toHex().data();
}

char * digest::sha1(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Sha1).toHex().data();
}

char * digest::sha1(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Sha1).toHex().data();
}
