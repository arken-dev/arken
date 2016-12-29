// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/helper>
#include <charon/utf8>

using namespace charon;

char * utf8::upper(const char * string)
{
  QByteArray tmp = QString(string).toUpper().toLocal8Bit();
  char * result  = new char[tmp.size() + 1];
  strcpy(result, tmp);
  result[tmp.size()] = '\0';
  return result;
}

char * utf8::lower(const char * string)
{
  QByteArray tmp = QString(string).toLower().toLocal8Bit();
  char * result  = new char[tmp.size() + 1];
  strcpy(result, tmp);
  result[tmp.size()] = '\0';
  return result;
}

int utf8::len(const char * string)
{
   int i = 0, j = 0;
   while (string[i]) {
     if ((string[i] & 0xc0) != 0x80) j++;
     i++;
   }
  return j;
}

