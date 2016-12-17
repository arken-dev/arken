#include <lua/lua.hpp>
#include <QtCore>
#include <QThread>

namespace charon
{

  class Thread : public QThread
  {
    public:
    QByteArray   m_charonPath;
    QByteArray   m_fileName;
    lua_State  * m_luaState;

    explicit Thread(QByteArray charonPath, QByteArray fileName);
    void run();
  };

}
