#include <OStringList>

namespace oberon {
  namespace regex {
    bool ematch(const char * string, const char * regex);
    int index(const char * string, const char * regex);
    int index(const char * string, const char * regex, int offset);
    OStringList * split(const char * string, const char * regex);
    OStringList * capture(const char * string, const char * regex);
    OStringList * capture(const char * string, const char * regex, int offset);
    bool match(const char * string, const char * regex);
    char * replace(const char * string, const char * regex, const char * after);
    OStringList * scan(const char * string, const char * regex);
  }
}
