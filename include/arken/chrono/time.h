// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CHRONO_TIME_
#define _ARKEN_CHRONO_TIME_

#include <ctime>
#include <arken/string.h>
#include <arken/chrono/date.h>

namespace arken {
namespace chrono {

  class Time: public Date
  {
    using string = arken::string;

    public:
    Time();
    Time(const Time &obj);
    Time(const Date &obj) : Date(obj) { };

    static Time now();
    static Time currentDateTime();
    static Time parse(const char * value);
    static Time parse(const char * value, const char * format);

    Time addYears(int years);
    Time addMonths(int months);
    Time addDays(int days);
    Time addHours(int hours);
    Time addMinutes(int minutes);
    Time addSecs(int secs);

    Time beginningOfDay();
    Time endOfDay();
    Time beginningOfMonth();
    Time endOfMonth();

    int hour();
    int min();
    int sec();

    string toString();
    Date date();

  };

} // namespace chrono
} // namespace arken
#endif
