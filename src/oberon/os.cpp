#include <QDateTime>
#include <QThread>
#include <os.h>

double os::microtime()
{
  return QDateTime::currentMSecsSinceEpoch() / 1000.0;
}

void os::sleep(double secs)
{
  QThread::msleep(secs*1000);
}
