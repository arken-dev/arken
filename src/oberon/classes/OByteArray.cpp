#include <QtCore>
#include <OByteArray>

OByteArray OByteArray::underscore(void)
{
  return OByteArray(string::underscore(this->data()));
}

OByteArray OByteArray::simplified(void)
{
  return (OByteArray) QByteArray::simplified();
}

OByteArray OByteArray::trimmed(void)
{
  return (OByteArray) QByteArray::trimmed();
}

OByteArray OByteArray::toUpper(void)
{
  return (OByteArray) QByteArray::toUpper();
}

OByteArray& OByteArray::append(const char *str)
{
  QByteArray::append(str);
  return *this;
}

OByteArray& OByteArray::append(const QByteArray &a)
{
  QByteArray::append(a);
  return *this;
}
