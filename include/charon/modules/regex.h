#include <CStringList>

namespace charon {
  namespace regex {
    bool ematch(const char * string, const char * regex);
    int index(const char * string, const char * regex);
    int index(const char * string, const char * regex, int offset);
    CStringList * split(const char * string, const char * regex);
    bool match(const char * string, const char * regex);
    char * replace(const char * string, const char * regex, const char * after);
    CStringList * scan(const char * string, const char * regex);
  }
}
