// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/utf8>
#include <QTextCodec>

using namespace arken;

char * utf8::sub(const char * string, int i, int j)
{
  QString str(string);
  if( j < 0 ) {
    j= str.size();
  }
  QByteArray tmp = str.mid(i, (j-i)+1).toLocal8Bit();
  char * result  = new char[tmp.size() + 1];
  strncpy(result, tmp, tmp.size());
  result[tmp.size()] = '\0';
  return result;
}

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

char * utf8::format(const char * string)
{
  QByteArray tmp = QString(string).toLocal8Bit();
  char * result = new char[tmp.size() + 1];
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

char * utf8::decode(const char * string, const char * charset)
{
  QTextCodec *codec = QTextCodec::codecForName(charset);
  QString    tmp = codec->toUnicode(string);
  QByteArray raw = tmp.toLocal8Bit();
  char * result = new char[raw.size() + 1];
  strcpy(result, raw.data());
  result[raw.size()] = '\0';
  return result;
}

char * utf8::encode(const char * string, const char * charset)
{
  QTextCodec *codec = QTextCodec::codecForName(charset);
  QByteArray raw = codec->fromUnicode(string);
  char * result = new char[raw.size() + 1];
  strcpy(result, raw.data());
  result[raw.size()] = '\0';
  return result;
}
