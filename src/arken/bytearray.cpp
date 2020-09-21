// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <QtCore>
#include <arken/base>

using arken::ByteArray;

ByteArray ByteArray::camelCase(void)
{
  return ByteArray(string::camelCase(this->data()));
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

ByteArray ByteArray::simplified(void)
{
  return ByteArray(string::simplified(this->data()));
}

ByteArray ByteArray::trimmed(void)
{
  return ByteArray(string::trimmed(this->data()));
}

ByteArray ByteArray::leftTrimmed(void)
{
  return ByteArray(string::leftTrimmed(this->data()));
}

ByteArray ByteArray::rightTrimmed(void)
{
  return ByteArray(string::rightTrimmed(this->data()));
}

ByteArray ByteArray::toUpper(void)
{
  return (ByteArray) QByteArray::toUpper();
}

ByteArray& ByteArray::prepend(const char *str)
{
  QByteArray::prepend(str);
  return *this;
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
