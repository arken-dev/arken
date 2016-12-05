// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/helper>
#include <charon/modules/digest.h>
#include <QCryptographicHash>

char * digest::md4(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Md4).toHex().data();
}

char * digest::md4(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Md4).toHex().data();
}

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

char * digest::sha224(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Sha224).toHex().data();
}

char * digest::sha224(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Sha224).toHex().data();
}

char * digest::sha256(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Sha256).toHex().data();
}

char * digest::sha256(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Sha256).toHex().data();
}

char * digest::sha384(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Sha384).toHex().data();
}

char * digest::sha384(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Sha384).toHex().data();
}

char * digest::sha512(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Sha512).toHex().data();
}

char * digest::sha512(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Sha512).toHex().data();
}
