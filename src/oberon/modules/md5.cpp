#include <oberon/helper>
#include <oberon/modules/md5.h>
#include <QCryptographicHash>

char * md5::hash(const char * hash)
{
  return QCryptographicHash::hash(hash, QCryptographicHash::Md5).toHex().data();
}
