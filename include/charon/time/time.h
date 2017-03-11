#ifndef __CHARON_TIME_TIME__
#define __CHARON_TIME_TIME__

#include <QtCore>
#include <QTime>
#include <charon/base>

namespace charon {

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

  ByteArray toString(const char * format);
  ByteArray toString();

};

} // namespace time
} // namespace charon

#endif
