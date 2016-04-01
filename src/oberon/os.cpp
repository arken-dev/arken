#include <QDateTime>
#include <QThread>
#include <QUuid>

#include <oberon/base>
#include <oberon/curl/curl-read.hpp>

double os::microtime()
{
  return QDateTime::currentMSecsSinceEpoch() / 1000.0;
}

void os::sleep(double secs)
{
  QThread::msleep(secs*1000);
}

char * os::uuid()
{
  QUuid uuid = QUuid::createUuid();
  return uuid.toByteArray().mid(1, 36).data();
}

char * os::read(const char * path)
{
  return oberon_curl_read(path);
}
