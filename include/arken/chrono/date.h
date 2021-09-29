// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CHRONO_DATE_
#define _ARKEN_CHRONO_DATE_

#include <ctime>
#include <arken/string.h>

namespace arken {
namespace chrono {

  class Date
  {

    using string = arken::string;

    protected:
    std::time_t m_time;
    std::tm     m_calendar;

    public:
    Date();
    Date(const Date &obj);

    static Date today();
    static Date parse(const char * value);
    static Date parse(const char * value, const char * format);

    Date addYears(int years);
    Date addMonths(int months);
    Date addDays(int days);

    Date beginningOfMonth();
    Date endOfMonth();

    int year();
    int month();
    int mon();
    int day();
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

} // namespace chrono
} // namespace arken
#endif
