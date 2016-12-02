#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>

/**
 * checkQRegExp
 */

QRegExp *
checkQRegExp( lua_State *L ) {
  return *(QRegExp **) luaL_checkudata(L, 1, "QRegExp.metatable");
}

/**
 * ClassMethods
 */

static int
lua_QRegExpClassMethodNew( lua_State *L ) {
  const char *str = (char *) lua_tostring(L, 1);
  QRegExp **ptr = (QRegExp **)lua_newuserdata(L, sizeof(QRegExp*));
  *ptr= new QRegExp(QString(str));
  luaL_getmetatable(L, "QRegExp.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg QRegExpClassMethods[] = {
  {"new", lua_QRegExpClassMethodNew},
  {NULL, NULL}
};

void static
registerQRegExpClassMethods( lua_State *L ) {
  luaL_register(L, "QRegExp", QRegExpClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_QRegExpInstanceMethodDestruct( lua_State *L ) {
  QRegExp *udata = checkQRegExp( L );
  delete udata;
  return 0;
}

static int
lua_QRegExpInstanceMethodExactMatch(lua_State *L) {
  QRegExp *regex   = checkQRegExp( L );
  const char * str = luaL_checkstring(L, 2);
  lua_pushboolean(L, regex->exactMatch(QString(str)));

  return 1;
}

static int
lua_QRegExpInstanceMethodCapturedTexts(lua_State *L) {
  QRegExp *regex   = checkQRegExp( L );

  QStringList **ptr = (QStringList **)lua_newuserdata(L, sizeof(QStringList*));
  *ptr= new QStringList(regex->capturedTexts());

  luaL_getmetatable(L, "QStringList.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
lua_QRegExpInstanceMethodIndexIn(lua_State *L) {
  QRegExp *regex   = checkQRegExp( L );
  const char * str = luaL_checkstring(L, 2);
  int offset;
  if(lua_isnumber(L, 3)) {
    offset = lua_tonumber(L, 3);
    lua_pushinteger(L, regex->indexIn(QString(str), offset));
  } else {
    lua_pushinteger(L, regex->indexIn(QString(str)));
  }
  return 1;
}

static int
lua_QRegExpInstanceMethodMatchedLength(lua_State *L) {
  QRegExp *regex    = checkQRegExp( L );
  lua_pushinteger(L, regex->matchedLength());
  return 1;
}

static int
lua_QRegExpInstanceMethodCap(lua_State *L) {
  QRegExp *regex    = checkQRegExp( L );
  int nth = luaL_checkinteger(L, 2);
  lua_pushstring(L, regex->cap(nth).toLocal8Bit().data());
  return 1;
}

static int
lua_QRegExpInstanceMethodScan(lua_State *L) {
  QRegExp *regex    = checkQRegExp( L );
  const char * str  = luaL_checkstring(L, 2);
  QString qstr      = QString(str);
  QStringList **ptr = (QStringList **)lua_newuserdata(L, sizeof(QStringList*));
  *ptr = new QStringList();
  int poss = 0;
  while ((poss = regex->indexIn(qstr, poss)) != -1) {
    (*ptr)->append(regex->cap(0));
    poss += regex->matchedLength();
  }
  luaL_getmetatable(L, "QStringList.metatable");
  lua_setmetatable(L, -2);

  return 1;
}


static const
luaL_reg QRegExpInstanceMethods[] = {
  {"scan", lua_QRegExpInstanceMethodScan},
  {"cap", lua_QRegExpInstanceMethodCap},
  {"matchedLength", lua_QRegExpInstanceMethodMatchedLength},
  {"indexIn", lua_QRegExpInstanceMethodIndexIn},
  {"capturedTexts", lua_QRegExpInstanceMethodCapturedTexts},
  {"exactMatch", lua_QRegExpInstanceMethodExactMatch},
  {"__gc", lua_QRegExpInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQRegExpInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QRegExp.metatable");
  luaL_register(L, NULL, QRegExpInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QRegExp( lua_State *L ) {
    registerQRegExpClassMethods(L);
    registerQRegExpInstanceMethods(L);
    return 1;
  }
}
