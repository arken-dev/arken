#include <QtCore>
#include <CStringList>

namespace charon {
  namespace base {
    namespace os {
      char   * abspath(const char * path);
      uint     atime(const char * path);
      bool     compare(const char * path1, const char * path2);
      bool     copy(const char * source, const char * destination, bool force);
      int      cores();
      bool     chdir(const char * dirpath);
      uint     ctime(const char * path);
      char   * dirpath(const char * path);
      bool     exists(const char * path);
      CStringList * glob(const char * dir);
      CStringList * glob(const char * dir, bool sub);
      CStringList * glob(const char * dir, const char * regex);
      CStringList * glob(const char * dir, const char * regex, bool sub);
      char   * home();
      char   * hostname();
      bool     link(const char * source, const char * destination, bool force);
      double   microtime();
      bool     mkdir(const char * dirname);
      bool     mkpath(const char * dirpath);
      char   * name();
      qint64   pid();
      char   * pwd();
      bool     rmdir(const char * dirname);
      bool     rmpath(const char * dirpath);
      void     sleep(double secs);
      char   * target(const char * path);
      char   * temp();
      bool     touch(const char * path);
      char   * uuid();
      char   * read(const char * path);
      char   * read(const char * path, size_t * size);
      char   * root();
    }
  }
}
