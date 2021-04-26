// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/time/time.h>

namespace arken {
namespace time {

Time Time::currentTime()
{
  return (Time) QTime::currentTime();
}

Time Time::addSecs(int secs)
{
  return (Time) QTime::addSecs(secs);
}

Time Time::fromString(const char * string,  const char * format)
{
  return QTime(QTime::fromString(QString(string), QString(format)));
}

string Time::toString(const char * format)
{
  return string(QTime::toString(format).toLocal8Bit().constData());
}

string Time::toString()
{
  return string(QTime::toString("hh:mm:ss.z").toLocal8Bit().constData());
}

Time * Time::parse(const char * str)
{
  char format[11];
  strncpy(format, "hh:mm", 5);
  size_t size = 5;

  if(str[5] == ':') {
    strncat(format, ":ss", 3);
    size += 3;
  }

  if(str[8] == '.') {
    strncat(format, ".z", 2);
    size += 2;
  }
  format[size] = '\0';

  Time result = Time::fromString(str, format);
  if( result.isValid() ) {
    return new Time(result);
  } else {
    return 0;
  }
}

} // namespace time
} // namespace arken
