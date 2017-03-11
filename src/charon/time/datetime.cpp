// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <iostream>

using charon::time::DateTime;

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

ByteArray DateTime::toString(const char * format)
{
  return ByteArray(QDateTime::toString(format).toLocal8Bit().data());
}

ByteArray DateTime::toString()
{
  return ByteArray(QDateTime::toString("yyyy/MM/dd hh:mm:ss.zzz").toLocal8Bit().data());
}

DateTime * DateTime::parse(const char * str)
{
  char format[25];

  if(str[4] == '-') {
    strcpy(format, "yyyy-MM-dd");
  } else if(str[4] == '/') {
    strcpy(format, "yyyy/MM/dd");
  } else if(str[5] == '-') {
    strcpy(format, "dd-MM-yyyy");
  } else {
    strcpy(format, "dd/MM/yyyy");
  }

  if(str[13] == ':') {
    strcat(format, " hh:mm");
  }

  if(str[16] == ':') {
    strcat(format, ":ss");
  }

  if(str[19] == '.') {
    strcat(format, ".z");
  }
  DateTime result = DateTime::fromString(str, format);
  if( result.isValid() ) {
    return new DateTime(result);
  } else {
    return 0;
  }
}
