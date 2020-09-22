// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_TIME_DATE_TIME_
#define _ARKEN_TIME_DATE_TIME_

#include <QtCore>
#include <QDateTime>
#include <arken/base>

using arken::ByteArray;

namespace arken {
namespace time {

class DateTime : public QDateTime
{

  public:

  static long long currentMSecsSinceEpoch();
  static DateTime currentDateTime();
  static DateTime fromString(const char * string, const char * format);
  static DateTime * parse(const char * str);

  DateTime() : QDateTime() { }
  DateTime(QDateTime dateTime) : QDateTime(dateTime) { }

  DateTime beginningOfDay();
  DateTime endOfDay();
  DateTime beginningOfMonth();
  DateTime endOfMonth();
  DateTime addDays(long long days);
  DateTime addSecs(long long secs);
  DateTime addMSecs(long long msecs);
  DateTime addMonths(int months);
  DateTime addYears(int year);
  string toString(const char * format);
  string toString();

};

} // namespace time
} // namespace arken

#endif
