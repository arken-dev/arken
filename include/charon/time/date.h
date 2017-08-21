#ifndef __CHARON_TIME_DATE__
#define __CHARON_TIME_DATE__

#include <QtCore>
#include <QDate>
#include <charon/base>

namespace charon {

namespace time {

class Date : public QDate
{

  public:

  static Date fromString(const char * string,  const char * format);
  static Date currentDate();
  static Date * parse(const char * str);

  Date() : QDate() { }
  Date(QDate date) : QDate(date) { }
  Date(const charon::time::Date& date) : QDate(date) { }

  Date beginningOfMonth();
  Date endOfMonth();
  Date addDays(long long days);
  Date addMonths(int months);
  Date addYears(int year);
  ByteArray toString(const char * format);
  ByteArray toString();

};

} // namespace time
} // namespace charon

#endif
