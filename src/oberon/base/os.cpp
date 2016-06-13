#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <QDateTime>
#include <QFile>
#include <QHostInfo>
#include <QThread>
#include <QUuid>

#include <oberon/helper>
#include <oberon/modules/http.h>
#include <iostream>
#include <oberon/md5>

bool os::compare(const char * path1, const char * path2)
{
  size_t size1;
  size_t size2;

  const char * buffer1;
  const char * buffer2;

  buffer1 = os::read(path1, &size1);
  buffer2 = os::read(path2, &size2);

  if( size1 != size2 ) {
    return false;
  }

  if( memcmp(buffer1, buffer2, size1) == 0 ) {
    return true;
  } else {
    return false;
  }
}

int os::cores()
{
  return QThread::idealThreadCount();
}

char * os::hostname()
{
  QString result = QHostInfo::localHostName();
  return result.toLocal8Bit().data();
}

double os::microtime()
{
  return QDateTime::currentMSecsSinceEpoch() / 1000.0;
}

char * os::name()
{
  QString result;
  #if defined(Q_OS_ANDROID)
  result = QLatin1String("android");
  #elif defined(Q_OS_BLACKBERRY)
  result = QLatin1String("blackberry");
  #elif defined(Q_OS_IOS)
  result = QLatin1String("ios");
  #elif defined(Q_OS_MAC)
  result = QLatin1String("osx");
  #elif defined(Q_OS_WINCE)
  result = QLatin1String("wince");
  #elif defined(Q_OS_WIN)
  result = QLatin1String("windows");
  #elif defined(Q_OS_LINUX)
  result = QLatin1String("linux");
  #elif defined(Q_OS_UNIX)
  result = QLatin1String("unix");
  #else
  result = QLatin1String("unknown");
  #endif
  return result.toLocal8Bit().data();
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
  return os::read(path, 0);
}

char * os::read(const char * path, size_t * size)
{
  char * result;
  if (string::startsWith(path, "http://")) {
    result = http::read(path);
  } else {
    QFile file(path);
    QByteArray raw;

    file.open(QIODevice::ReadOnly);
    raw = file.readAll();
    if( size != 0 ) {
      *size = raw.size();
    }
    result = new char[raw.size() + 1];
    memcpy( result, raw.data(), raw.size() );
    result[raw.size()] = '\0';
  }

  return result;
}
