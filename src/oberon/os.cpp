#include <QDateTime>
#include <os.h>

double os::microtime()
{
  return QDateTime::currentMSecsSinceEpoch() / 1000.0;
}
