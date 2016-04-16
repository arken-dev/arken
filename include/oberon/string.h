#include <stdlib.h>
#include <cstring>
#include <cctype>

namespace oberon {
  namespace string {
    char * append(const char * string, const char * str);
    bool   contains(const char * string, const char * str);
    int    indexOf(const char * string, const char * str);
    char * insert(const char * string, int len, const char * ba);
    bool   endsWith(const char * string, const char * ba);
    int    lastIndexOf(const char * string, const char * str);
    char * left(const char *string, int len);
    char * mid(const char * string, int pos, int len);
    char * repeated(const char *buffer, int times);
    char * replace(const char * string, const char before, const char * after);
    char * right(const char *buffer, int len);
    char * simplified(const char *buffer);
    bool   startsWith(const char *string, const char *str);
    char * trimmed(const char *string);
    char * truncate(const char *string, int pos);
    char * underscore(const char *string);
  }
}
