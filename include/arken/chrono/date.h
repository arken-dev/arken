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
  struct tm* localtime(const time_t* time);
  char* asctime( const std::tm* time_ptr );
  char *strptime(const char *buf, const char *format, struct tm *tm);
  time_t mktime(struct std::tm *time_ptr);
  time_t time( std::time_t* arg );
  size_t strftime( char* str, std::size_t count, const char* format, const std::tm* tp );



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
    static Date currentDate();
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

    int dayOfWeek();
    int dayOfYear();
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

    bool operator <  (const Date &dt) const;
    bool operator <= (const Date &dt) const;
    bool operator >  (const Date &dt) const;
    bool operator >= (const Date &dt) const;
    bool operator == (const Date &dt) const;
  };

} // namespace chrono
} // namespace arken
#endif
