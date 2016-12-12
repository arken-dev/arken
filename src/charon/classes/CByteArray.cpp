// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <QtCore>
#include <CByteArray>

CByteArray CByteArray::camelcase(void)
{
  return CByteArray(string::camelcase(this->data()));
}

CByteArray CByteArray::capitalize(void)
{
  return CByteArray(string::capitalize(this->data()));
}

CByteArray CByteArray::underscore(void)
{
  return CByteArray(string::underscore(this->data()));
}

CByteArray CByteArray::simplified(void)
{
  return (CByteArray) QByteArray::simplified();
}

CByteArray CByteArray::trimmed(void)
{
  return (CByteArray) QByteArray::trimmed();
}

CByteArray CByteArray::toUpper(void)
{
  return (CByteArray) QByteArray::toUpper();
}

CByteArray& CByteArray::append(const char *str)
{
  QByteArray::append(str);
  return *this;
}

CByteArray& CByteArray::append(const QByteArray &a)
{
  QByteArray::append(a);
  return *this;
}

CByteArray & CByteArray::operator=(const CByteArray &a)
{
  QByteArray::operator=(a);
  return *this;
}

CByteArray & CByteArray::operator=(const char * a)
{
  QByteArray::operator=(a);
  return *this;
}
