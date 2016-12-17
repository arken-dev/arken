#include <QtCore>
#include <charon/ByteArrayList>

using charon::ByteArrayList;

namespace charon {
  namespace base {
    class os {
      public:
      static char   * abspath(const char * path);
      static uint     atime(const char * path);
      static bool     compare(const char * path1, const char * path2);
      static bool     copy(const char * source, const char * destination, bool force);
      static int      cores();
      static bool     chdir(const char * dirpath);
      static uint     ctime(const char * path);
      static char   * dirpath(const char * path);
      static bool     exists(const char * path);
      static ByteArray * glob(const char * dir);
      static ByteArray * glob(const char * dir, bool sub);
      static ByteArray * glob(const char * dir, const char * regex);
      static ByteArray * glob(const char * dir, const char * regex, bool sub);
      static char   * home();
      static char   * hostname();
      static bool     link(const char * source, const char * destination, bool force);
      static double   microtime();
      static bool     mkdir(const char * dirname);
      static bool     mkpath(const char * dirpath);
      static char   * name();
      static qint64   pid();
      static char   * pwd();
      static bool     rmdir(const char * dirname);
      static bool     rmpath(const char * dirpath);
      static void     sleep(double secs);
      static char   * target(const char * path);
      static char   * temp();
      static bool     touch(const char * path);
      static char   * uuid();
      static char   * read(const char * path);
      static char   * read(const char * path, size_t * size);
      static char   * root();
    };
  }
}
