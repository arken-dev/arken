#include <luajit-2.0/lua.hpp>
#include <QtCore>
#include <QThread>
#include <QMutex>

class MirandaService : public QThread
{

  private:
  static QMutex s_mutex;
  QByteArray    m_oberonPath;
  QByteArray    m_fileName;
  lua_State   * m_luaState;
  const char  * m_uuid;
  bool m_shutdown;
  bool m_service;

  public:
  MirandaService(QByteArray oberonPath, QByteArray fileName);
  MirandaService(QByteArray oberonPath, QByteArray fileName, const char * uuid);
  ~MirandaService();
  bool loop(int secs);
  void shutdown();
  bool isShutdown();
  void run();

};
