#ifndef __CHARON_TIME_DATE_TIME__
#define __CHARON_TIME_DATE_TIME__

#include <QtCore>
#include <QDateTime>
#include <charon/base>

using charon::ByteArray;

namespace charon {
namespace time {

class DateTime : public QDateTime
{

  public:

  static long long currentMSecsSinceEpoch();
  static DateTime currentDateTime();
  static DateTime fromString(const char * string, const char * format);
  static DateTime * parse(const char * str);

  DateTime() : QDateTime() { }
  DateTime(QDateTime dateTime) : QDateTime(dateTime) { }

  DateTime beginningOfDay();
  DateTime endOfDay();
  DateTime beginningOfMonth();
  DateTime endOfMonth();
  DateTime addDays(long long days);
  DateTime addSecs(long long secs);
  DateTime addMSecs(long long msecs);
  DateTime addMonths(int months);
  DateTime addYears(int year);
  ByteArray toString(const char * format);
  ByteArray toString();

};

} // namespace time
} // namespace charon

#endif
