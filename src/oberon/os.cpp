#include <QDateTime>
#include <QThread>
#include <QUuid>
#include <QFile>

#include <oberon/base>
#include <oberon/curl/curl-read.hpp>
#include <oberon/string/oberon_string_startsWith.hpp>

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
  char * result;

  if (oberon_string_startsWith(path, "http://")) {
    result = oberon_curl_read(path);
  } else {
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    result = file.readAll().data();
  }

  return result;
}
