#include <cstdlib>
#include <cstring>
#include <cctype>

namespace oberon {
  namespace string {
    char * append(const char * string, const char * str);
    char * camelcase(const char * string, bool lcfirst);
    char * camelcase(const char * string);
    char * capitalize(const char * string);
    bool   contains(const char * string, const char * str);
    char * escape(const char * string);
    char * escapeHtml(const char * string);
    int    indexOf(const char * string, const char * str);
    char * insert(const char * string, int len, const char * ba);
    bool   endsWith(const char * string, const char * ba);
    int    lastIndexOf(const char * string, const char * str);
    char * left(const char *string, int len);
    char * mid(const char * string, int pos, int len);
    char * normalize(const char * string);
    char * repeated(const char *buffer, int times);
    char * replace(const char * string, const char before, const char after);
    char * right(const char *buffer, int len);
    char * simplified(const char *buffer);
    bool   startsWith(const char *string, const char *str);
    char * swap(const char * string, const char * before, const char * after);
    char * trimmed(const char *string);
    char * truncate(const char *string, int pos);
    char * underscore(const char *string);
  }
}
