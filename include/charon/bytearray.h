// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_BYTE_ARRAY_
#define _ARKEN_BYTE_ARRAY_

#include <QtCore>
#include <arken/base>

namespace arken
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
  ByteArray simplified(void);
  ByteArray normalize(void);
  ByteArray trimmed(void);
  ByteArray leftTrimmed(void);
  ByteArray rightTrimmed(void);
  ByteArray toUpper(void);
  ByteArray &prepend(const char * s);
  ByteArray &append(const char *s);
  ByteArray &append(const QByteArray &a);
  ByteArray &operator=(const ByteArray &a);
  ByteArray &operator=(const char * a);
};

}

#endif
