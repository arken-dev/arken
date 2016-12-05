#include <lua/lua.hpp>
#include <QtCore>
#include <QThread>

class OThread : public QThread
{
  public:
  QByteArray   m_oberonPath;
  QByteArray   m_fileName;
  lua_State  * m_luaState;

  explicit OThread(QByteArray oberonPath, QByteArray fileName);
  void run();
};
