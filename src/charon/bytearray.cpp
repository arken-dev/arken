// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <QtCore>
#include <charon/helper>

using charon::ByteArray;

ByteArray ByteArray::camelcase(void)
{
  return ByteArray(string::camelcase(this->data()));
}

ByteArray ByteArray::capitalize(void)
{
  return ByteArray(string::capitalize(this->data()));
}

ByteArray ByteArray::normalize(void)
{
  return ByteArray(string::normalize(this->data()));
}

ByteArray ByteArray::underscore(void)
{
  return ByteArray(string::underscore(this->data()));
}

ByteArray ByteArray::reduce(void)
{
  return ByteArray(string::reduce(this->data()));
}

ByteArray ByteArray::trim(void)
{
  return ByteArray(string::trim(this->data()));
}

ByteArray ByteArray::trimLeft(void)
{
  return ByteArray(string::trimLeft(this->data()));
}

ByteArray ByteArray::trimRight(void)
{
  return ByteArray(string::trimRight(this->data()));
}

ByteArray ByteArray::toUpper(void)
{
  return (ByteArray) QByteArray::toUpper();
}

ByteArray& ByteArray::append(const char *str)
{
  QByteArray::append(str);
  return *this;
}

ByteArray& ByteArray::append(const QByteArray &a)
{
  QByteArray::append(a);
  return *this;
}

ByteArray & ByteArray::operator=(const ByteArray &a)
{
  QByteArray::operator=(a);
  return *this;
}

ByteArray & ByteArray::operator=(const char * a)
{
  QByteArray::operator=(a);
  return *this;
}
