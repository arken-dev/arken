#include "OThread"

OThread::OThread(QByteArray oberonPath, QByteArray fileName)
{
  m_oberonPath = oberonPath;
  m_fileName   = fileName;
}

void OThread::run() {
  int rv, i;
  QByteArray profile, name;
  QVariant value;


  m_luaState = lua_open();
  luaL_openlibs(m_luaState);

  /* OBERON_PATH */
  lua_pushstring(m_luaState, m_oberonPath);
  lua_setglobal(m_luaState, "OBERON_PATH");

  /* PROPERTIES */
  QList<QByteArray> list = this->dynamicPropertyNames();
  for(i=0; i < list.size(); i++) {
    name  = list.at(i);
    value = this->property(name);

    if(value.type() == QVariant::ByteArray) {
      lua_pushstring(m_luaState, value.toByteArray());
      lua_setglobal(m_luaState, name);
    }

    if(value.type() == QVariant::Double) {
      lua_pushnumber(m_luaState, value.toDouble());
      lua_setglobal(m_luaState, name);
    }

    if(value.type() == QVariant::Bool) {
      lua_pushboolean(m_luaState, value.toBool());
      lua_setglobal(m_luaState, name);
    }
  }

  /* PROFILE */
  profile = m_oberonPath;
  profile.append("/profile.lua");

  rv = luaL_loadfile(m_luaState, profile);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    return;
  }

  rv = lua_pcall(m_luaState, 0, 0, lua_gettop(m_luaState) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    return;
  }

  rv = luaL_loadfile(m_luaState, m_fileName.data());
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    return;
  }

  rv = lua_pcall(m_luaState, 0, 0, lua_gettop(m_luaState) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    return;
  }

}
