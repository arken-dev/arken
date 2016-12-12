// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/helper>
#include <charon/modules/sha3.h>
#include <QCryptographicHash>

char * sha3::sha224(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Sha3_224).toHex().data();
}

char * sha3::sha224(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Sha3_224).toHex().data();
}

char * sha3::sha256(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Sha3_256).toHex().data();
}

char * sha3::sha256(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Sha3_256).toHex().data();
}

char * sha3::sha384(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Sha3_384).toHex().data();
}

char * sha3::sha384(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Sha3_384).toHex().data();
}

char * sha3::sha512(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Sha3_512).toHex().data();
}

char * sha3::sha512(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Sha3_512).toHex().data();
}
