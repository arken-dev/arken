#include <luajit-2.0/lua.hpp>
#include "mirandastate.h"
#include <QDebug>
#include <QFile>

QMutex MirandaService::s_mutex;

void miranda_cache_register(lua_State * L);
void miranda_server_register(lua_State * L);
void miranda_service_register(lua_State * L);

MirandaService::MirandaService(QByteArray fileName)
{
  m_fileName   = fileName;
  m_shutdown   = false;
  m_service    = true;
  m_uuid       = NULL;
}

MirandaService::MirandaService(QByteArray fileName, QByteArray uuid)
{
  m_fileName   = fileName;
  m_shutdown   = false;
  m_service    = false;
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
  MirandaState * state;
  lua_State * luaState;

  state = MirandaState::pop();
  luaState = state->instance();

  /* OBERON_TASK */
  lua_pushstring(luaState, m_uuid);
  lua_setglobal(luaState, "OBERON_TASK");

  //allocate
  MirandaService **ptr = (MirandaService **)lua_newuserdata(luaState, sizeof(MirandaService*));
  *ptr = this;
  lua_setglobal(luaState, "__miranda_service");

  //call
  rv = luaL_loadfile(luaState, m_fileName.data());
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(luaState, -1));
    return;
  }

  rv = lua_pcall(luaState, 0, 0, lua_gettop(luaState) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(luaState, -1));
    return;
  }

  // clear OBERON_TASK
  lua_pushnil(luaState);
  lua_setglobal(luaState, "OBERON_TASK");

  // clear this
  lua_pushnil(luaState);
  lua_setglobal(luaState, "__miranda_service");

  // stack push lua state
  MirandaState::push(state);

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
