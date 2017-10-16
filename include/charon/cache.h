#include <QMutex>
#include <QHash>
#include <charon/base>

namespace charon
{

class cache {
  public:
  static const char * value(const char * key);
  static void insert(const char *key, const char * value, int expires = -1);
  static int remove(const char * key);

  private:

  class data {
    public:
    data(const char * value, int expires);
    ~data();
    const char * value();
    bool isExpires();

    private:
    const char * m_value;
    double m_expires;
  };

  static QMutex s_mutex;
  static QHash<ByteArray, data *> * s_cache;

};

}
