#include <stdlib.h>
#include <string.h>

#include <QDateTime>
#include <QThread>
#include <QUuid>
#include <QFile>

#include <oberon/base>
#include <oberon/http.h>

int os::cores()
{
  return QThread::idealThreadCount();
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
  char * result;
  if (string::startsWith(path, "http://")) {
    result = http::read(path);
  } else {
    QFile file(path);
    QByteArray data;
    int size;

    file.open(QIODevice::ReadOnly);
    data = file.readAll();
    size = data.size() + 1;
    result = (char *) malloc( size * sizeof(char) );
    strcpy(result, data);
    result[size] = '\0';
  }

  return result;
}
