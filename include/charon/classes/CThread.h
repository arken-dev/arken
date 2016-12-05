#include <lua/lua.hpp>
#include <QtCore>
#include <QThread>

class CThread : public QThread
{
  public:
  QByteArray   m_oberonPath;
  QByteArray   m_fileName;
  lua_State  * m_luaState;

  explicit CThread(CByteArray oberonPath, CByteArray fileName);
  void run();
};
