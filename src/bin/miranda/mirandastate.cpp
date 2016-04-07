#include "mirandastate.h"
#include <QDebug>
#include <QStack>

static QByteArray static_oberonPath  = "";
static QByteArray static_profilePath = "";
static qint64     static_lastReload  = 0;
static QStack<MirandaState *> * stack   = new QStack<MirandaState *>;
static QMutex     mutex;

MirandaState::MirandaState()
{
  int rv;
  m_State = luaL_newstate();

  luaL_openlibs(m_State);
  lua_pushstring(m_State, static_oberonPath);
  lua_setglobal(m_State, "OBERON_PATH");

  rv = luaL_loadfile(m_State, static_profilePath);
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

void MirandaState::init(QByteArray oberonPath, QByteArray profilePath)
{
  static_oberonPath  = oberonPath;
  static_profilePath = profilePath;
  static_lastReload  = QDateTime::currentMSecsSinceEpoch();
}

MirandaState * MirandaState::pop()
{
  QMutexLocker ml(&mutex);
  MirandaState * state;

  if( stack->isEmpty() ) {
    state = new MirandaState();
  } else {
    state = stack->pop();
  }

  return state;
}

void MirandaState::push(MirandaState * state)
{
  QMutexLocker ml(&mutex);
  stack->push(state);
}

void MirandaState::reload()
{
  static_lastReload = QDateTime::currentMSecsSinceEpoch();
}

void MirandaState::clear()
{
  QMutexLocker ml(&mutex);

  while( !stack->isEmpty() ) {
    delete stack->pop();
  }
}

lua_State * MirandaState::instance()
{
  return m_State;
}
