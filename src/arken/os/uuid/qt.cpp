#include <arken/base>
#include <QUuid>

char * os::uuid()
{
  QUuid uuid = QUuid::createUuid();
  auto result = new char[37];
  strncpy(result, uuid.toByteArray().mid(1, 36).data(), 36);
  result[36] = '\0';
  return result;
}
