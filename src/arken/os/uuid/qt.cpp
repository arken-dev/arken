#include <arken/base>
#include <QUuid>

string os::uuid()
{
  QUuid uuid = QUuid::createUuid();
  return string(uuid.toByteArray().mid(1, 36).constData(), 36);
}
