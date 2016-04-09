#include "mirandastate.h"
#include <QDebug>
#include <QStack>

QByteArray MirandaState::m_oberonPath  = "";
QByteArray MirandaState::m_profilePath = "";
QMutex     MirandaState::m_mutex;

static int
miranda_server_reload(lua_State *) {
  MirandaState::reload();
  qDebug() << "reload...";
  return 0;
}

static int
miranda_server_clear(lua_State *) {
  MirandaState::clear();
  qDebug() << "clear...";
  return 0;
}


static void
miranda_server_register(lua_State * L) {
  static const         luaL_reg Map[] = {
    {"reload",  miranda_server_reload},
    {"clear",   miranda_server_clear},
    {NULL, NULL}
  };

  luaL_register(L, "server", Map);
}


static qint64     static_lastReload  = 0;
static QStack<MirandaState *> * stack   = new QStack<MirandaState *>;

MirandaState::MirandaState()
{
  int rv;
  m_lastReload = QDateTime::currentMSecsSinceEpoch();
  m_State = luaL_newstate();

  luaL_openlibs(m_State);

  miranda_server_register(m_State);

  lua_pushstring(m_State, m_oberonPath);
  lua_setglobal(m_State, "OBERON_PATH");

  rv = luaL_loadfile(m_State, m_profilePath);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_State, -1));
  }

  rv = lua_pcall(m_State, 0, 0, lua_gettop(m_State) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_State, -1));
  }

  luaL_loadfile(m_State, "process_http.lua" );
  lua_pcall(m_State, 0, 0, 0);
  qDebug() << "create Lua State";
}

MirandaState::~MirandaState()
{
  lua_close(m_State);
}

void MirandaState::init(QByteArray oberonPath, QByteArray profilePath)
{
  m_oberonPath  = oberonPath;
  m_profilePath = profilePath;
  static_lastReload  = QDateTime::currentMSecsSinceEpoch();
}

MirandaState * MirandaState::pop()
{
  QMutexLocker ml(&m_mutex);
  MirandaState * state;

  if( stack->isEmpty() ) {
    state = new MirandaState();
  } else {
    state = stack->pop();
    if( static_lastReload > state->m_lastReload ) {
      delete state;
      state = new MirandaState();
    }
  }

  return state;
}

void MirandaState::push(MirandaState * state)
{
  QMutexLocker ml(&m_mutex);
  stack->push(state);
}

void MirandaState::reload()
{
  static_lastReload = QDateTime::currentMSecsSinceEpoch();
}

void MirandaState::clear()
{
  QMutexLocker ml(&m_mutex);
  static_lastReload = QDateTime::currentMSecsSinceEpoch();
  while( !stack->isEmpty() ) {
    delete stack->pop();
  }
}

lua_State * MirandaState::instance()
{
  return m_State;
}
