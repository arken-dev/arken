// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/helper>
#include <QRegExp>
#include <QString>
#include <QStringList>
#include <cstring>

using namespace charon;
using charon::ByteArray;
using charon::ByteArrayList;

bool regex::ematch(const char * string, const char * regex)
{
  return QRegExp(regex).exactMatch(string);
}

int regex::index(const char * string, const char * regex)
{
  return QRegExp(regex).indexIn(string, 0);
}

int regex::index(const char * string, const char * regex, int offset)
{
  return QRegExp(regex).indexIn(string, offset);
}

ByteArrayList * regex::split(const char * string, const char * regex)
{
  QRegExp qregex(regex);
  ByteArrayList *list = new ByteArrayList();
  QString qstr(string);
  int older = 0;
  int poss  = 0;
  while ((poss = qregex.indexIn(string, poss)) != -1) {
    list->append(qstr.mid(older, poss-older).replace(qregex, "").toLocal8Bit());
    older = poss;
    poss  += qregex.matchedLength();
  }
  if( older < qstr.size() ) {
     list->append(qstr.mid(older, qstr.size() -older).replace(qregex, "").toLocal8Bit());
  }

  return list;
}

bool regex::match(const char * string, const char * regex)
{
  return QRegExp(regex).indexIn(string, 0) != -1;
}

char * regex::replace(const char * string, const char * regex, const char * after)
{
  QString str(string);
  QString aft = str.replace(QRegExp(regex), after);
  char * result = new char[aft.size() + 1];
  strcpy(result, aft.toLocal8Bit().data());
  return result;
}

ByteArrayList * regex::scan(const char * string, const char * regex)
{
  QRegExp qregex(regex);
  ByteArrayList *list = new ByteArrayList();
  QString qstr(string);
  int older = 0;
  int poss  = 0;
  while ((poss = qregex.indexIn(string, poss)) != -1) {
    QStringList qlist = qregex.capturedTexts();
    if( qlist.size() > 0 ) {
       QString value = qlist.at(0);
       if( ! value.isEmpty() ) {
         list->append(value.toLocal8Bit());
       }
    }
    older = poss;
    poss  += qregex.matchedLength();
  }
  if( older < qstr.size() ) {
     QStringList qlist = qregex.capturedTexts();
     if( qlist.size() > 0 ) {
       QString value = qlist.at(0);
       if( ! value.isEmpty() ) {
         list->append(value.toLocal8Bit());
       }
     }
  }
  return list;
}
