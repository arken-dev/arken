#include <charon/base>

double os::microtime()
{
  return QDateTime::currentMSecsSinceEpoch() / 1000.0;
}

