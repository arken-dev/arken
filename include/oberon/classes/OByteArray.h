#ifndef _OBYTE_ARRAY_HEADER_
#define _OBYTE_ARRAY_HEADER_

#include <QtCore>
#include <oberon/helper>

class OByteArray : public QByteArray
{

  //Data *d;
  private:

  bool underscore_special_char(char chr);
  int underscore_len(void);

  public:

  OByteArray() : QByteArray() { }
  OByteArray(const char *data) : QByteArray(data) { }
  OByteArray(const char *data, int size) : QByteArray(data, size) { }

  OByteArray capitalize(void);
  OByteArray underscore(void);
  OByteArray simplified(void);
  OByteArray trimmed(void);
  OByteArray toUpper(void);
  OByteArray &append(const char *s);
  OByteArray &append(const QByteArray &a);
  OByteArray &operator=(const OByteArray &a);
  OByteArray &operator=(const char * a);
};

#endif
