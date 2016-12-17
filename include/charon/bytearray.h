#ifndef _CBYTE_ARRAY_HEADER_
#define _CBYTE_ARRAY_HEADER_

#include <QtCore>
#include <charon/helper>

namespace charon
{

class ByteArray : public QByteArray
{

  //Data *d;
  private:

  bool underscore_special_char(char chr);
  int underscore_len(void);

  public:

  CByteArray() : QByteArray() { }
  CByteArray(const char *data) : QByteArray(data) { }
  CByteArray(const char *data, int size) : QByteArray(data, size) { }

  CByteArray camelcase(void);
  CByteArray capitalize(void);
  CByteArray underscore(void);
  CByteArray simplified(void);
  CByteArray trimmed(void);
  CByteArray toUpper(void);
  CByteArray &append(const char *s);
  CByteArray &append(const QByteArray &a);
  CByteArray &operator=(const CByteArray &a);
  CByteArray &operator=(const char * a);
};

}

#endif
