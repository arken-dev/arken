// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_BYTE_ARRAY_LIST_
#define _CHARON_BYTE_ARRAY_LIST_

#include <charon/base>

namespace charon
{

class string;

class ByteArrayList
{
  private:
  void init();
  int m_size;
  int m_resource;
  int m_cursor;
  string ** m_array;

  public:
  ByteArrayList();
  ByteArrayList(int resource);
  ~ByteArrayList();
  void replace(int pos, const char * value);
  ByteArrayList &append(const char * value);
  ByteArrayList &append(const char * value, int len);
  ByteArrayList &operator << (const char * value);
  const char *operator[](int pos);
  const char *at(int pos);
  const char *at(int pos, int * len);
  const char *first();
  const char *last();
  const char *each();
  char * join(const char * separator);
  int size();
};

}
#endif
