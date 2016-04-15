#include "mirandastate.h"
#include <QDebug>
#include <QStack>

qint64     MirandaState::s_version     = 0;
QByteArray MirandaState::s_oberonPath  = "";
QByteArray MirandaState::s_profilePath = "";
QMutex     MirandaState::s_mutex;

QStack<MirandaState *> * MirandaState::s_stack = new QStack<MirandaState *>;
QHash<QByteArray, QByteArray> * MirandaState::s_cache = new QHash<QByteArray, QByteArray>;

void miranda_server_register(lua_State * L);
void miranda_cache_register(lua_State * L);

MirandaState::MirandaState()
{
  int rv;
  m_version = s_version;
  m_State = luaL_newstate();

  luaL_openlibs(m_State);

  miranda_server_register(m_State);
  miranda_cache_register(m_State);

  lua_pushstring(m_State, s_oberonPath);
  lua_setglobal(m_State, "OBERON_PATH");

  rv = luaL_loadfile(m_State, s_profilePath);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_State, -1));
  }

  rv = lua_pcall(m_State, 0, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_State, -1));
  }

  luaL_loadfile(m_State, "dispatch.lua" );
  lua_pcall(m_State, 0, 0, 0);
  qDebug() << "create Lua State";
}

MirandaState::~MirandaState()
{
  lua_close(m_State);
}

void MirandaState::init(QCoreApplication *app)
{
  s_oberonPath = app->applicationDirPath().toLocal8Bit();
  s_oberonPath.truncate( s_oberonPath.lastIndexOf('/') );

  s_profilePath = s_oberonPath;
  s_profilePath.append("/profile.lua");
}

MirandaState * MirandaState::pop()
{
  QMutexLocker ml(&s_mutex);
  MirandaState * state;

  if( s_stack->isEmpty() ) {
    state = new MirandaState();
  } else {
    state = s_stack->pop();
    if( s_version != state->m_version ) {
      delete state;
      state = new MirandaState();
    }
  }

  return state;
}

void MirandaState::push(MirandaState * state)
{
  QMutexLocker ml(&s_mutex);
  s_stack->push(state);
}

void MirandaState::reload()
{
  s_version++;
}

void MirandaState::clear()
{
  QMutexLocker ml(&s_mutex);
  s_version++;
  while( !s_stack->isEmpty() ) {
    delete s_stack->pop();
  }
}

lua_State * MirandaState::instance()
{
  return m_State;
}

int MirandaState::version()
{
  return s_version;
}

const char * MirandaState::value(const char * key)
{
  QMutexLocker ml(&s_mutex);
  return s_cache->value(key);
}

void MirandaState::insert(const char * key, const char * value)
{
  QMutexLocker ml(&s_mutex);
  s_cache->insert(key, value);
}
