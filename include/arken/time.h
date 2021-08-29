// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_TIME_
#define _ARKEN_TIME_

#include <ctime>
#include <arken/string.h>

namespace arken
{

  // TODO isValid ???
  // TODO fromString => parse(str, fmt) ???
  class Time
  {
    using string = arken::string;

    private:
    std::time_t m_time;
    std::tm     m_calendar;

    public:
    Time();
    Time(const Time &obj);

    static Time now();
    static Time parse(const char * value);
    static Time parse(const char * value, const char * format);

    Time addYears(int years);
    Time addMonths(int months);
    Time addDays(int days);
    Time addHours(int hours);
    Time addMinutes(int secs);
    Time addSecs(int secs);

    Time beginningOfDay();
    Time endOfDay();
    Time beginningOfMonth();
    Time endOfMonth();

    int year();
    int month();
    int mon();
    int day();
    int hour();
    int min();
    int sec();
    int mday();
    int wday();
    int yday();
    int daysInMonth();

    time_t time();

    bool isLeapYear();
    bool isSunday();
    bool isMonday();
    bool isTuesday();
    bool isWednesday();
    bool isThursday();
    bool isFriday();
    bool isSaturday();
    bool isValid();

    string asctime();
    string strftime(const char *);
    string toString();

  };

} // namespace arken
#endif
