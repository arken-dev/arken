// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_TIME_DATE_
#define _ARKEN_TIME_DATE_

#include <QtCore>
#include <QDate>
#include <arken/base>

namespace arken {

namespace time {

class Date : public QDate
{

  public:

  static Date fromString(const char * string,  const char * format);
  static Date currentDate();
  static Date * parse(const char * str);

  Date() : QDate() { }
  Date(QDate date) : QDate(date) { }
  Date(const arken::time::Date& date) : QDate(date) { }

  Date beginningOfMonth();
  Date endOfMonth();
  Date addDays(long long days);
  Date addMonths(int months);
  Date addYears(int year);
  string toString(const char * format);
  string toString();

};

} // namespace time
} // namespace arken

#endif
