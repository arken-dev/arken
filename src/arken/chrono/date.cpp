#include <mutex>
#include <arken/chrono/date.h>


namespace arken {
namespace chrono {

static std::mutex s_chrono_mutex;

struct std::tm* localtime(const time_t* t)
{
  std::unique_lock<std::mutex> lck(s_chrono_mutex);
  return std::localtime(t);
}

char* asctime( const std::tm* time_ptr )
{
  std::unique_lock<std::mutex> lck(s_chrono_mutex);
  return std::asctime(time_ptr);
}

char *strptime(const char *buf, const char *format, struct tm *tm)
{
  std::unique_lock<std::mutex> lck(s_chrono_mutex);
  return ::strptime(buf, format, tm);
}

time_t mktime(struct std::tm *time_ptr)
{
  std::unique_lock<std::mutex> lck(s_chrono_mutex);
  return ::std::mktime(time_ptr);
}

Date::Date()
{
  m_time = 0;
}

Date::Date(const Date &obj)
{
  m_time     = obj.m_time;
  m_calendar = obj.m_calendar;
}

Date Date::today()
{
  Date t;
  t.m_time = std::time(nullptr);
  std::tm * timeinfo = arken::chrono::localtime(&t.m_time);

  t.m_time -= (timeinfo->tm_sec + (timeinfo->tm_min * 60) + (timeinfo->tm_hour * 60 * 60));

  t.m_calendar.tm_sec   = 0;
  t.m_calendar.tm_min   = 0;
  t.m_calendar.tm_hour  = 0;
  t.m_calendar.tm_mday  = timeinfo->tm_mday;
  t.m_calendar.tm_mon   = timeinfo->tm_mon;
  t.m_calendar.tm_year  = timeinfo->tm_year;
  t.m_calendar.tm_wday  = timeinfo->tm_wday;
  t.m_calendar.tm_yday  = timeinfo->tm_yday;
  t.m_calendar.tm_isdst = timeinfo->tm_isdst;

  return t;
}

Date Date::currentDate()
{
  Date t;
  t.m_time = std::time(nullptr);
  std::tm * timeinfo = arken::chrono::localtime(&t.m_time);

  t.m_time -= (timeinfo->tm_sec + (timeinfo->tm_min * 60) + (timeinfo->tm_hour * 60 * 60));

  t.m_calendar.tm_sec   = 0;
  t.m_calendar.tm_min   = 0;
  t.m_calendar.tm_hour  = 0;
  t.m_calendar.tm_mday  = timeinfo->tm_mday;
  t.m_calendar.tm_mon   = timeinfo->tm_mon;
  t.m_calendar.tm_year  = timeinfo->tm_year;
  t.m_calendar.tm_wday  = timeinfo->tm_wday;
  t.m_calendar.tm_yday  = timeinfo->tm_yday;
  t.m_calendar.tm_isdst = timeinfo->tm_isdst;

  return t;
}


Date Date::parse(const char * str)
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

  return Date::parse(str, format);
}

Date Date::parse(const char * str, const char * fmt)
{
  Date t;

  t.m_calendar.tm_sec   = 0;
  t.m_calendar.tm_min   = 0;
  t.m_calendar.tm_hour  = 0;
  t.m_calendar.tm_mday  = 0;
  t.m_calendar.tm_mon   = 0;
  t.m_calendar.tm_year  = 0;
  t.m_calendar.tm_wday  = 0;
  t.m_calendar.tm_yday  = 0;
  t.m_calendar.tm_isdst = 0;

  if ( arken::chrono::strptime(str, fmt, &t.m_calendar) ) {
    t.m_calendar.tm_sec  = 0;
    t.m_calendar.tm_min  = 0;
    t.m_calendar.tm_hour = 0;
    t.m_time = arken::chrono::mktime(&t.m_calendar);
  } else {
    t.m_calendar.tm_year = 0;
  }

  return t;
}

Date Date::addYears(int years)
{
  Date t;

  t.m_calendar = m_calendar;
  t.m_calendar.tm_year += years;

  t.m_time = arken::chrono::mktime(&t.m_calendar);

  return t;
}


Date Date::addMonths(int months)
{
  Date t;

  t.m_calendar = m_calendar;
  t.m_calendar.tm_mon += months;

  t.m_time = arken::chrono::mktime(&t.m_calendar);

  return t;
}

Date Date::addDays(int days)
{
  Date t;

  t.m_calendar = m_calendar;
  t.m_calendar.tm_mday += days;

  t.m_time = arken::chrono::mktime(&t.m_calendar);

  return t;
}

Date Date::beginningOfMonth()
{
  int day = (this->day() - 1) * -1;
  return this->addDays(day);
}

Date Date::endOfMonth()
{
  int day = (this->daysInMonth() - this->day());
  return this->addDays(day);
}

int Date::year()
{
  return 1900 + m_calendar.tm_year;
}

int Date::month()
{
  return m_calendar.tm_mon + 1;
}

int Date::mon()
{
  return m_calendar.tm_mon + 1;
}

int Date::day()
{
  return m_calendar.tm_mday;
}

int Date::mday()
{
  return m_calendar.tm_mday;
}

int Date::wday()
{
  return m_calendar.tm_wday;
}

int Date::dayOfWeek()
{
  return m_calendar.tm_wday;
}

int Date::dayOfYear()
{
  return m_calendar.tm_yday;
}

int Date::yday()
{
  return m_calendar.tm_yday;
}

time_t Date::time()
{
  return m_time;
}

bool Date::isLeapYear()
{
  return ((year() % 4 == 0) && (year() % 100 != 0)) || (year() % 400 == 0);
}

bool Date::isSunday()
{
  return m_calendar.tm_wday == 0;
}

bool Date::isMonday()
{
  return m_calendar.tm_wday == 1;
}

bool Date::isTuesday()
{
  return m_calendar.tm_wday == 2;
}

bool Date::isWednesday()
{
  return m_calendar.tm_wday == 3;
}

bool Date::isThursday()
{
  return m_calendar.tm_wday == 4;
}

bool Date::isFriday()
{
  return m_calendar.tm_wday == 5;
}

bool Date::isSaturday()
{
  return m_calendar.tm_wday == 6;
}

bool Date::isValid()
{
  return m_calendar.tm_year > 0;
}

int Date::daysInMonth()
{
  static int leapYearMonthDays[]   = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  static int commonYearMonthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

  if( isLeapYear() ) {
    return leapYearMonthDays[m_calendar.tm_mon];
  } else {
    return commonYearMonthDays[m_calendar.tm_mon];
  }
}

string Date::asctime()
{
  const char * result = arken::chrono::asctime(&m_calendar);
  return string(result, 24);
}

string Date::strftime(const char * format)
{
  auto result = new char[100]();
  std::strftime(result, 100, format, &m_calendar);
  return string(std::move(result));
}

string Date::toString()
{
  return strftime("%Y-%m-%d");
}

bool Date::operator<(const Date &dt) const
{
  return m_time < dt.m_time;
}

bool Date::operator<=(const Date &dt) const
{
  return m_time <= dt.m_time;
}

bool Date::operator>(const Date &dt) const
{
  return m_time > dt.m_time;
}

bool Date::operator>=(const Date &dt) const
{
  return m_time >= dt.m_time;
}

bool Date::operator==(const Date &dt) const
{
  return m_time == dt.m_time;
}

} // namespace chrono
} // namespace arken
