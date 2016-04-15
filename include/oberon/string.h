#include <stdlib.h>

namespace oberon {
  namespace string {
    char * append(const char * string, const char * str);
    char * insert(const char * string, int len, const char * ba);
    bool   endsWith(const char * string, const char * ba);
    char * mid(const char * string, size_t string_len, int pos, int len);
    char * repeated(char *buffer, int times);
    char * replace(const char * string, const char before, const char * after);
    char * right(char *buffer, int len);
    char * simplified(char *buffer);
    bool   startsWith(const char *string, const char *str);
    char * trimmed(const char *string);
    char * truncate(const char *string, int pos);
    char * underscore(const char *string);
  }
}
