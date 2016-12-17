#include <charon/ByteArrayList>

using charon::ByteArrayList;

namespace charon
{

class regex {
  static bool ematch(const char * string, const char * regex);
  static int index(const char * string, const char * regex);
  static int index(const char * string, const char * regex, int offset);
  static CStringList * split(const char * string, const char * regex);
  static bool match(const char * string, const char * regex);
  static char * replace(const char * string, const char * regex, const char * after);
  static CStringList * scan(const char * string, const char * regex);
};

}
