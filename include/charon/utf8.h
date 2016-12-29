#ifndef _CHARON_UTF8_
#define _CHARON_UTF8_

namespace charon
{

class utf8 {
  public:
  static char * upper(const char * string);
  static char * lower(const char * string);
  static int len(const char * string);
};

}

#endif
