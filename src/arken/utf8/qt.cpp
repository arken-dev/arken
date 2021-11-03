// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// https://stackoverflow.com/questions/36897781/how-to-uppercase-lowercase-utf-8-characters-in-c
// https://www.py4u.net/discuss/75078

#include <arken/base>
#include <arken/utf8>
#include <QTextCodec>

namespace arken {

char * utf8::sub(const char * string, int i, int j)
{
  QString str(string);
  if( j < 0 ) {
    j= str.size();
  }
  QByteArray tmp = str.mid(i, (j-i)+1).toLocal8Bit();
  auto result    = new char[tmp.size() + 1];
  strncpy(result, tmp, tmp.size());
  result[tmp.size()] = '\0';
  return result;
}

char * utf8::upper(const char * str)
{
  size_t i   = 0;
  size_t len = strlen(str);
  char * res = new char[len+1];

  while( str[i] ) {

    if( str[i] == -61 && (str[i+1] >= -95 && str[i+1] <= -67) ) {
      res[i] = str[i];
      i++;
      res[i] = str[i] - 32;
    } else if( str[i] >= 97 && str[i] <= 122 ) {
      res[i] = str[i] - 32;
    } else {
      res[i] = str[i];
    }

    i++;
  }

  res[i] = '\0';

  return res;
}

char * utf8::lower(const char * str)
{
  size_t i   = 0;
  size_t len = strlen(str);
  char * res = new char[len+1];

  while( str[i] ) {

    if( str[i] == -61 && (str[i+1] >= -127 && str[i+1] <= -99) ) {
      res[i] = str[i];
      i++;
      res[i] = str[i] + 32;
    } else if( str[i] >= 65 && str[i] <= 90 ) {
      res[i] = str[i] + 32;
    } else {
      res[i] = str[i];
    }

    i++;
  }

  res[i] = '\0';

  return res;
}

char * utf8::format(const char * string)
{
  QByteArray tmp = QString(string).toLocal8Bit();
  auto result    = new char[tmp.size() + 1];
  strncpy(result, tmp, tmp.size());
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
  auto result    = new char[raw.size() + 1];
  strncpy(result, raw.data(), raw.size());
  result[raw.size()] = '\0';
  return result;
}

char * utf8::encode(const char * string, const char * charset)
{
  QTextCodec *codec = QTextCodec::codecForName(charset);
  QByteArray raw = codec->fromUnicode(string);
  auto result    = new char[raw.size() + 1];
  strncpy(result, raw.data(), raw.size());
  result[raw.size()] = '\0';
  return result;
}

char * utf8::sanitize(const char * string)
{
  size_t origlen = strlen(string);
  char * result  = new char[origlen+1];
  int i = 0, j = 0;
  while (string[i]) {
    if ( string[i] == -61 ) {
      if ( string[i + 1] < 0 && string[i + 1] != -61 ) {
        result[j] = string[i];
        j++; i++;
        result[j] = string[i];
        j++; i++;
      } else {
        i++;
      }
    } else {
      if( string[i] > 0 ) {
        result[j] = string[i];
        j++; i++;
      } else {
        i++;
      }
    }
  }
  result[j] = '\0';
  return result;
}

} // namespace arken
