// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/time/date.h>


namespace arken {
namespace time {

Date Date::currentDate()
{
  return (Date) QDate::currentDate();
}

Date Date::beginningOfMonth()
{
  int day = (this->day() - 1) * -1;
  return (Date) this->addDays(day);
}

Date Date::endOfMonth()
{
  int day = (this->daysInMonth() - this->day());
  return (Date) this->addDays(day);
}

Date Date::addDays(long long days)
{
  return (Date) QDate::addDays(days);
}

Date Date::addMonths(int months)
{
  return (Date) QDate::addMonths(months);
}

Date Date::addYears(int year)
{
  return (Date) QDate::addYears(year);
}

Date Date::fromString(const char * string,  const char * format)
{
  return QDate(QDate::fromString(QString(string), QString(format)));
}

string Date::toString(const char * format)
{
  return string(QDate::toString(format).toLocal8Bit().constData());
}

string Date::toString()
{
  return string(QDate::toString("yyyy/MM/dd").toLocal8Bit().constData());
}

Date * Date::parse(const char * str)
{
  char format[11];

  if(str[4] == '-') {
    strncpy(format, "yyyy-MM-dd", 10);
  } else if(str[4] == '/') {
    strncpy(format, "yyyy/MM/dd", 10);
  } else if(str[5] == '-') {
    strncpy(format, "dd-MM-yyyy", 10);
  } else {
    strncpy(format, "dd/MM/yyyy", 10);
  }
  format[10] = '\0';

  Date result = Date::fromString(str, format);
  if( result.isValid() ) {
    return new Date(result);
  } else {
    return 0;
  }
}

} // namespace time
} // namespace arken
