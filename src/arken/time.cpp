#include <arken/time.h>

static int leapYearMonthDays[]   = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static int commonYearMonthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };


namespace arken {

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
  t.m_calendar.tm_isdst = 0;

  if ( strptime(str, fmt, &t.m_calendar) ) {
    t.m_time = std::mktime(&t.m_calendar);
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

int Time::year()
{
  return 1900 + m_calendar.tm_year;
}

int Time::month()
{
  return m_calendar.tm_mon + 1;
}

int Time::mon()
{
  return m_calendar.tm_mon + 1;
}

int Time::day()
{
  return m_calendar.tm_mday;
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

int Time::mday()
{
  return m_calendar.tm_mday;
}

int Time::wday()
{
  return m_calendar.tm_wday;
}

int Time::yday()
{
  return m_calendar.tm_yday;
}

time_t Time::time()
{
  return m_time;
}

bool Time::isLeapYear()
{
  return ((year() % 4 == 0) && (year() % 100 != 0)) || (year() % 400 == 0);
}

bool Time::isSunday()
{
  return m_calendar.tm_wday == 0;
}

bool Time::isMonday()
{
  return m_calendar.tm_wday == 1;
}

bool Time::isTuesday()
{
  return m_calendar.tm_wday == 2;
}

bool Time::isWednesday()
{
  return m_calendar.tm_wday == 3;
}

bool Time::isThursday()
{
  return m_calendar.tm_wday == 4;
}

bool Time::isFriday()
{
  return m_calendar.tm_wday == 5;
}

bool Time::isSaturday()
{
  return m_calendar.tm_wday == 6;
}

bool Time::isValid()
{
  return m_calendar.tm_year > 0;
}

int Time::daysInMonth()
{
  if( isLeapYear() ) {
    return leapYearMonthDays[m_calendar.tm_mon];
  } else {
    return commonYearMonthDays[m_calendar.tm_mon];
  }
}

string Time::asctime()
{
  const char * result = std::asctime(&m_calendar);
  return string(result, 24);
}

string Time::strftime(const char * format)
{
  char * result = new char[100];
  std::strftime(result, 100, format, &m_calendar);
  return string(std::move(result));
}

string Time::toString()
{
  string format("%Y-%m-%d");
  if( this->hour() > 0 ) {
    format.append(" %H:%M:%S");
  }
  char * result = new char[100];
  std::strftime(result, 100, format, &m_calendar);
  return string(std::move(result));
}

} // namespace arken
