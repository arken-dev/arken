#include <stdlib.h>
#include <string.h>

#include <QDateTime>
#include <QThread>
#include <QUuid>
#include <QFile>

#include <oberon/base>
#include <oberon/http.h>

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
