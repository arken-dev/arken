// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_TIME_TIME_
#define _ARKEN_TIME_TIME_

#include <QtCore>
#include <QTime>
#include <arken/base>

namespace arken {

namespace time {

class Time : public QTime
{

  public:
  static Time fromString(const char * string,  const char * format);
  static Time currentTime();
  static Time * parse(const char * str);

  Time() : QTime() { }
  Time(QTime time) : QTime(time) { }

  Time addSecs(int secs);

  string toString(const char * format);
  string toString();

};

} // namespace time
} // namespace arken

#endif
