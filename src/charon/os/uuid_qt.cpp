#include <charon/base>

char * os::uuid()
{
  QUuid uuid = QUuid::createUuid();
  char * result = new char[37];
  strcpy(result, uuid.toByteArray().mid(1, 36).data());
  result[37] = '\0';
  return result;
}
