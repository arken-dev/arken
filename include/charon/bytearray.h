// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CBYTE_ARRAY_HEADER_
#define _CBYTE_ARRAY_HEADER_

#include <QtCore>
#include <charon/base>

namespace charon
{

class ByteArray : public QByteArray
{

  //Data *d;
  private:

  bool underscore_special_char(char chr);
  int underscore_len(void);

  public:

  ByteArray() : QByteArray() { }
  ByteArray(const char *data) : QByteArray(data) { }
  ByteArray(const char *data, int size) : QByteArray(data, size) { }

  ByteArray camelCase(void);
  ByteArray capitalize(void);
  ByteArray underscore(void);
  ByteArray reduce(void);
  ByteArray normalize(void);
  ByteArray trim(void);
  ByteArray trimLeft(void);
  ByteArray trimRight(void);
  ByteArray toUpper(void);
  ByteArray &append(const char *s);
  ByteArray &append(const QByteArray &a);
  ByteArray &operator=(const ByteArray &a);
  ByteArray &operator=(const char * a);
};

}

#endif
