#include <oberon/helper>
#include <oberon/modules/md5.h>
#include <QCryptographicHash>

char * md5::hash(const char * hash)
{
  return QCryptographicHash::hash(QByteArray(hash), QCryptographicHash::Md5).toHex().data();
}

char * md5::hash(const char * hash, int length)
{
  return QCryptographicHash::hash(QByteArray(hash, length), QCryptographicHash::Md5).toHex().data();
}
