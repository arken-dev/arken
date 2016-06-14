#include <cstring>
#include <QFileInfo>

namespace oberon {
  namespace os {
    char   * abspath(const char * path);
    uint     atime(const char * path);
    bool     compare(const char * path1, const char * path2);
    bool     copy(const char * source, const char * destination, bool force);
    int      cores();
    uint     ctime(const char * path);
    char   * dirpath(const char * path);
    bool     exists(const char * path);
    char   * home();
    char   * hostname();
    bool     link(const char * source, const char * destination, bool force);
    double   microtime();
    bool     mkdir(const char * dirname);
    bool     mkpath(const char * dirpath);
    char   * name();
    char   * pwd();
    bool     rmdir(const char * dirname);
    bool     rmpath(const char * dirpath);
    void     sleep(double secs);
    char   * target(const char * path);
    char   * temp();
    char   * uuid();
    char   * read(const char * path);
    char   * read(const char * path, size_t * size);
    char   * root();
  }
}
