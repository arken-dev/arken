// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>


using charon::time::Date;
using charon::string;

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
  return string(QDate::toString(format).toLocal8Bit().data());
}

string Date::toString()
{
  return string(QDate::toString("yyyy/MM/dd").toLocal8Bit().data());
}

Date * Date::parse(const char * str)
{
  char format[15];

  if(str[4] == '-') {
    strcpy(format, "yyyy-MM-dd");
  } else if(str[4] == '/') {
    strcpy(format, "yyyy/MM/dd");
  } else if(str[5] == '-') {
    strcpy(format, "dd-MM-yyyy");
  } else {
    strcpy(format, "dd/MM/yyyy");
  }

  Date result = Date::fromString(str, format);
  if( result.isValid() ) {
    return new Date(result);
  } else {
    return 0;
  }
}
