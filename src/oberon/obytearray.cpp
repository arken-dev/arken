#include <QtCore>
#include <OByteArray>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "oberon/string/oberon_string_underscore.hpp"

OByteArray OByteArray::underscore(void)
{
  return OByteArray(oberon_string_underscore(this->data()));
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
