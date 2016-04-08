#include <QtCore>
#include <OByteArray>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "oberon/algorithms/string/underscore.hpp"

OByteArray OByteArray::underscore(void)
{
  return OByteArray(oberon_algorithms_string_underscore(this->data(), this->size()));
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
