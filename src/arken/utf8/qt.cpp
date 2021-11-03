// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// https://stackoverflow.com/questions/36897781/how-to-uppercase-lowercase-utf-8-characters-in-c
// https://www.py4u.net/discuss/75078
// https://www.ime.usp.br/~pf/algoritmos/apend/unicode.html
// https://pt.wikipedia.org/wiki/ISO/IEC_8859-1
// https://www.asciitable.com/
// https://cs.stanford.edu/people/miles/iso8859.html
// https://www.utf8-chartable.de/
// https://github.com/dart-lang/http/issues/175

#include <arken/base>
#include <arken/utf8>
#include <QTextCodec>

namespace arken {

char * utf8::sub(const char * str, int p1, int p2)
{

  int i  = 0;
  int j  = 0;
  int v1 = 0;
  int v2 = 0;

  if( p2 < 0 ) {
    p2 = utf8::len(str) - 1;
  }

  while( str[i] ) {

    if( j == p1 ) {
      v1 = i;
    }

    if( str[i] == -61 ) {
      i++;
    }

    if( j == p2 ) {
      v2 = i;
    }

    i++;
    j++;

  }

  int size   = (v2 - v1) + 1;
  char * res = new char[size+1];
  int a = 0;
  for(int b=v1; a < size; a++, b++) {
    res[a] = str[b];
  }

  res[size] = '\0';

  return res;
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

char * utf8::sanitize(const char * str)
{
  size_t len = strlen(str);
  char * res = new char[len+1];
  int i = 0, j = 0;
  while (str[i]) {
    if ( str[i] == -61 ) {
      if ( str[i + 1] < 0 && str[i + 1] != -61 ) {
        res[j] = str[i];
        j++; i++;
        res[j] = str[i];
        j++; i++;
      } else {
        i++;
      }
    } else {
      if( str[i] > 0 ) {
        res[j] = str[i];
        j++; i++;
      } else {
        i++;
      }
    }
  }
  res[j] = '\0';
  return res;
}

} // namespace arken
