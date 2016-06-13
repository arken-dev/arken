#include <cstring>

namespace oberon {
  namespace os {
    bool     compare(const char * path1, const char * path2);
    int      cores();
    char   * hostname();
    double   microtime();
    char   * name();
    void     sleep(double secs);
    char   * uuid();
    char   * read(const char * path);
    char   * read(const char * path, size_t * size);
  }
}
