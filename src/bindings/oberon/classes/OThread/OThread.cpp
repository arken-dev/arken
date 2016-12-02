#include <lua/lua.hpp>
#include <QtCore>
#include <OThread>

/**
 * checkOThread
 */

OThread *
checkOThread( lua_State *L ) {
  return *(OThread **) luaL_checkudata(L, 1, "OThread.metatable");
}

/**
 * ClassMethods
 */

static int
lua_OThreadClassMethodNew( lua_State *L ) {

  /* PATH */
  lua_getglobal(L, "OBERON_PATH");
  const char * OBERON_PATH = lua_tostring(L, -1);

  /* Thread */
  const char *str = (char *) lua_tostring(L, 1);
  OThread   **ptr = (OThread **)lua_newuserdata(L, sizeof(OThread*));
  *ptr= new OThread(OBERON_PATH, str);
  luaL_getmetatable(L, "OThread.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const luaL_reg OThreadClassMethods[] = {
  {"new", lua_OThreadClassMethodNew},
  {NULL, NULL}
};

void static
registerOThreadClassMethods( lua_State *L ) {
  luaL_register(L, "OThread", OThreadClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_OThreadInstanceMethodDestruct( lua_State *L ) {
  OThread *thread = checkOThread( L );
  delete thread;
  return 0;
}

static int
lua_OThreadInstanceMethodStart( lua_State *L ) {
  OThread *thread = checkOThread( L );
  thread->start();
  return 0;
}

static int
lua_OThreadInstanceMethodSetProperty( lua_State *L ) {
  bool result = false;
  bool flag   = true;

  OThread    * thread = checkOThread( L );
  const char * name   = luaL_checkstring(L, 2);

  if(lua_isboolean(L, 3)) {
    flag   = false;
    result = thread->setProperty(name, (bool) lua_toboolean(L, 3));
  }

  if(flag && lua_isnumber(L, 3)) {
    flag   = false;
    result = thread->setProperty(name, (double) lua_tonumber(L, 3));
  }

  if(flag && lua_isstring(L, 3)) {
    flag   = false;
    result = thread->setProperty(name, QByteArray(lua_tostring(L, 3)));
  }

  lua_pushboolean(L, result);

  return 1;
}

static int
lua_OThreadInstanceMethodWait( lua_State *L ) {
  OThread *thread = checkOThread( L );
  bool result  = thread->wait();
  lua_pushboolean(L, result);
  return 1;
}

static int
lua_OThreadInstanceMethodProperty( lua_State *L ) {

  bool flag = true;
  OThread *thread = checkOThread( L );
  const char *property = lua_tostring(L, 2);
  lua_getglobal(thread->m_luaState, property);

  if(lua_isboolean(thread->m_luaState, -1)) {
    flag = false;
    lua_pushboolean(L, lua_toboolean(thread->m_luaState, -1));
  }

  if(flag && lua_isnumber(thread->m_luaState, -1)) {
    flag = false;
    lua_pushnumber(L, lua_tonumber(thread->m_luaState, -1));
  }

  if(flag && lua_isstring(thread->m_luaState, -1)) {
    flag = false;
    lua_pushstring(L, lua_tostring(thread->m_luaState, -1));
  }

  if(flag) {
    lua_pushnil(L);
  }

  return 1;
}

static const
luaL_reg OThreadInstanceMethods[] = {
  {"wait", lua_OThreadInstanceMethodWait},
  {"setProperty", lua_OThreadInstanceMethodSetProperty},
  {"property", lua_OThreadInstanceMethodProperty},
  {"start", lua_OThreadInstanceMethodStart},
  {"__gc", lua_OThreadInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerOThreadInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "OThread.metatable");
  luaL_register(L, NULL, OThreadInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_OThread( lua_State *L ) {
    registerOThreadClassMethods(L);
    registerOThreadInstanceMethods(L);
    return 1;
  }
}
