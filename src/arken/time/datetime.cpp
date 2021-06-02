// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/time/datetime.h>
#include <iostream>


namespace arken {
namespace time {

DateTime DateTime::currentDateTime()
{
  return (DateTime) QDateTime::currentDateTime();
}

DateTime DateTime::beginningOfDay()
{
  return DateTime::fromString(this->toString("yyyy/MM/dd 00:00:00"), "yyyy/MM/dd hh:mm:ss");
}

DateTime DateTime::endOfDay()
{
  return DateTime::fromString(this->toString("yyyy/MM/dd 23:59:59"), "yyyy/MM/dd hh:mm:ss");
}

DateTime DateTime::beginningOfMonth()
{
  int day = (this->date().day() - 1) * -1;
  return (DateTime) this->addDays(day).beginningOfDay();
}

DateTime DateTime::endOfMonth()
{
  int day = (this->date().daysInMonth() - this->date().day());
  return (DateTime) this->addDays(day).endOfDay();
}

DateTime DateTime::addDays(long long days)
{
  return (DateTime) QDateTime::addDays(days);
}

DateTime DateTime::addMonths(int months)
{
  return (DateTime) QDateTime::addMonths(months);
}

DateTime DateTime::addSecs(long long secs)
{
  return (DateTime) QDateTime::addSecs(secs);
}

DateTime DateTime::addMSecs(long long msecs)
{
  return (DateTime) QDateTime::addMSecs(msecs);
}

DateTime DateTime::addYears(int year)
{
  return (DateTime) QDateTime::addYears(year);
}

long long DateTime::currentMSecsSinceEpoch()
{
  return QDateTime::currentMSecsSinceEpoch();
}

DateTime DateTime::fromString(const char * string,  const char * format)
{
  return QDateTime(QDateTime::fromString(QString(string), QString(format)));
}

string DateTime::toString(const char * format)
{
  return string(QDateTime::toString(format).toLocal8Bit().constData());
}

string DateTime::toString()
{
  return string(QDateTime::toString("yyyy/MM/dd hh:mm:ss.zzz").toLocal8Bit().constData());
}

DateTime * DateTime::parse(const char * str)
{
  char format[25];
  size_t size = 10;

  if(str[4] == '-') {
    strncpy(format, "yyyy-MM-dd", 10);
  } else if(str[4] == '/') {
    strncpy(format, "yyyy/MM/dd", 10);
  } else if(str[5] == '-') {
    strncpy(format, "dd-MM-yyyy", 10);
  } else {
    strncpy(format, "dd/MM/yyyy", 10);
  }

  if(str[13] == ':') {
    strncat(format, " hh:mm", 6);
    size += 6;
  }

  if(str[16] == ':') {
    strncat(format, ":ss", 3);
    size += 3;
  }

  if(str[19] == '.') {
    strncat(format, ".z", 2);
    size += 2;
  }
  format[size] = '\0';

  DateTime result = DateTime::fromString(str, format);
  if( result.isValid() ) {
    return new DateTime(result);
  } else {
    return nullptr;
  }
}

} // namespace time
} // namespace arken
