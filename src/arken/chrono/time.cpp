#include <arken/chrono/time.h>

namespace arken {
namespace chrono {

Time::Time()
{
  m_time = 0;
}

Time::Time(const Time &obj)
{
  m_time     = obj.m_time;
  m_calendar = obj.m_calendar;
}

Time Time::now()
{
  Time t;
  t.m_time = std::time(nullptr);
  std::tm * timeinfo = std::localtime(&t.m_time);

  t.m_calendar.tm_sec   = timeinfo->tm_sec;
  t.m_calendar.tm_min   = timeinfo->tm_min;
  t.m_calendar.tm_hour  = timeinfo->tm_hour;
  t.m_calendar.tm_mday  = timeinfo->tm_mday;
  t.m_calendar.tm_mon   = timeinfo->tm_mon;
  t.m_calendar.tm_year  = timeinfo->tm_year;
  t.m_calendar.tm_wday  = timeinfo->tm_wday;
  t.m_calendar.tm_yday  = timeinfo->tm_yday;
  t.m_calendar.tm_isdst = timeinfo->tm_isdst;

  return t;
}

Time Time::currentDateTime()
{
  Time t;
  t.m_time = std::time(nullptr);
  std::tm * timeinfo = std::localtime(&t.m_time);

  t.m_calendar.tm_sec   = timeinfo->tm_sec;
  t.m_calendar.tm_min   = timeinfo->tm_min;
  t.m_calendar.tm_hour  = timeinfo->tm_hour;
  t.m_calendar.tm_mday  = timeinfo->tm_mday;
  t.m_calendar.tm_mon   = timeinfo->tm_mon;
  t.m_calendar.tm_year  = timeinfo->tm_year;
  t.m_calendar.tm_wday  = timeinfo->tm_wday;
  t.m_calendar.tm_yday  = timeinfo->tm_yday;
  t.m_calendar.tm_isdst = timeinfo->tm_isdst;

  return t;
}


Time Time::parse(const char * str)
{
  string format;

  if(str[4] == '-') {
    format.append("%Y-%m-%d");
  } else if(str[4] == '/') {
    format.append("%Y/%m/%d");
  } else if(str[5] == '-') {
    format.append("%d-%m-%Y");
  } else {
    format.append("%d/%m/%Y");
  }

  if(str[13] == ':') {
    format.append(" %H:%M");
  }

  if(str[16] == ':') {
    format.append(":%S");
  }

  return Time::parse(str, format);
}

Time Time::parse(const char * str, const char * fmt)
{
  Time t;

  t.m_calendar.tm_sec   = 0;
  t.m_calendar.tm_min   = 0;
  t.m_calendar.tm_hour  = 0;
  t.m_calendar.tm_mday  = 0;
  t.m_calendar.tm_mon   = 0;
  t.m_calendar.tm_year  = 0;
  t.m_calendar.tm_wday  = 0;
  t.m_calendar.tm_yday  = 0;
  t.m_calendar.tm_isdst = -1;

  if ( strptime(str, fmt, &t.m_calendar) ) {
    t.m_time = std::mktime(&t.m_calendar);
  } else {
    t.m_calendar.tm_year = 0;
  }

  return t;
}

Time Time::addYears(int years)
{
  Time t;

  t.m_calendar = m_calendar;
  t.m_calendar.tm_year += years;

  t.m_time = std::mktime(&t.m_calendar);

  return t;
}


Time Time::addMonths(int months)
{
  Time t;

  t.m_calendar = m_calendar;
  t.m_calendar.tm_mon += months;

  t.m_time = std::mktime(&t.m_calendar);

  return t;
}

Time Time::addDays(int days)
{
  Time t;

  t.m_calendar = m_calendar;
  t.m_calendar.tm_mday += days;

  t.m_time = std::mktime(&t.m_calendar);

  return t;
}

Time Time::addHours(int hours)
{
  Time t;

  t.m_calendar = m_calendar;
  t.m_calendar.tm_hour += hours;

  t.m_time = std::mktime(&t.m_calendar);

  return t;
}


Time Time::addMinutes(int minutes)
{
  Time t;

  t.m_calendar = m_calendar;
  t.m_calendar.tm_min += minutes;

  t.m_time = std::mktime(&t.m_calendar);

  return t;
}

Time Time::addSecs(int secs)
{
  Time t;

  t.m_calendar = m_calendar;
  t.m_calendar.tm_sec += secs;

  t.m_time = std::mktime(&t.m_calendar);

  return t;
}

Time Time::beginningOfDay()
{
  Time t;
  t.m_calendar = m_calendar;
  t.m_calendar.tm_sec  = 0;
  t.m_calendar.tm_min  = 0;
  t.m_calendar.tm_hour = 0;
  t.m_time = std::mktime(&t.m_calendar);
  return t;
}

Time Time::endOfDay()
{
  Time t;
  t.m_calendar = m_calendar;
  t.m_calendar.tm_sec  = 59;
  t.m_calendar.tm_min  = 59;
  t.m_calendar.tm_hour = 23;
  t.m_time = std::mktime(&t.m_calendar);
  return t;
}

Time Time::beginningOfMonth()
{
  int day = (this->day() - 1) * -1;
  return this->addDays(day).beginningOfDay();
}

Time Time::endOfMonth()
{
  int day = (this->daysInMonth() - this->day());
  return this->addDays(day).endOfDay();
}

int Time::hour()
{
  return m_calendar.tm_hour;
}

int Time::min()
{
  return m_calendar.tm_min;
}

int Time::sec()
{
  return m_calendar.tm_sec;
}

string Time::toString()
{
  return strftime("%Y-%m-%d %H:%M:%S");
}

Date Time::date()
{
  return Date::parse(this->strftime("%Y-%m-%d"));
}

} // namespace chrono
} // namespace arken
