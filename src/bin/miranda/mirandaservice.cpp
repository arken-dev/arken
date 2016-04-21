#include <luajit-2.0/lua.hpp>
#include "mirandastate.h"
#include <QDebug>
#include <QFile>

QMutex MirandaService::s_mutex;

void miranda_cache_register(lua_State * L);
void miranda_server_register(lua_State * L);
void miranda_service_register(lua_State * L);

MirandaService::MirandaService(QByteArray oberonPath, QByteArray fileName)
{
  m_oberonPath = oberonPath;
  m_fileName   = fileName;
  m_shutdown   = false;
  m_service    = true;
  m_luaState   = NULL;
  m_uuid       = NULL;
}

MirandaService::MirandaService(QByteArray oberonPath, QByteArray fileName, const char * uuid)
{
  m_oberonPath = oberonPath;
  m_fileName   = fileName;
  m_shutdown   = false;
  m_service    = false;
  m_luaState   = NULL;
  m_uuid       = uuid;
}


MirandaService::~MirandaService()
{
  qDebug() << "destructor service ..." << m_fileName;
}

bool MirandaService::loop(int secs)
{
  int i = 0;

  while( i < secs ) {

    QThread::msleep(1000);

    if(isShutdown()) {
      return false;
    }

    i++;
  }

  return true;
}

bool MirandaService::isShutdown()
{
  QMutexLocker ml(&s_mutex);

  if(m_shutdown) {
    return true;
  }

  return false;
}

void MirandaService::run() {

  int rv;
  QByteArray profile, name;
  QVariant value;

  m_luaState = lua_open();
  luaL_openlibs(m_luaState);

  /* OBERON_PATH */
  lua_pushstring(m_luaState, m_oberonPath);
  lua_setglobal(m_luaState, "OBERON_PATH");

  /* OBERON_TASK */
  lua_pushstring(m_luaState, m_uuid);
  lua_setglobal(m_luaState, "OBERON_TASK");

  /* PROFILE */
  profile = m_oberonPath;
  profile.append("/profile.lua");

  //loadfile
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

  //allocate
  MirandaService **ptr = (MirandaService **)lua_newuserdata(m_luaState, sizeof(MirandaService*));
  *ptr = this;
  lua_setglobal(m_luaState, "__miranda_service");

  miranda_cache_register(m_luaState);
  miranda_server_register(m_luaState);
  miranda_service_register(m_luaState);

  //call
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

  lua_close(m_luaState);

  if( m_service && QFile::exists( m_fileName ) ) {
    MirandaState::createService(m_fileName);
  }
}

void MirandaService::shutdown()
{
  QMutexLocker ml(&s_mutex);
  qDebug() << "shutdown" << m_fileName;
  m_shutdown = true;
}
