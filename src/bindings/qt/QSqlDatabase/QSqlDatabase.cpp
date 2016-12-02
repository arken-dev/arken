#include <lua/lua.hpp>
#include <QtCore>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <iostream>

/**
 * checkQSqlDatabase
 */

QSqlDatabase *
checkQSqlDatabase( lua_State *L ) {
  return *(QSqlDatabase **) luaL_checkudata(L, 1, "QSqlDatabase.metatable");
}

/**
 * ClassMethods
 */

static int
lua_QSqlDatabaseClassMethodAddDatabase( lua_State *L ) {
  QSqlDatabase **ptr = (QSqlDatabase **)lua_newuserdata(L, sizeof(QSqlDatabase*));
  QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
  *ptr =  db;
  std::cout << "primeiro\n";
  luaL_getmetatable(L, "QSqlDatabase.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg QSqlDatabaseClassMethods[] = {
  {"addDatabase", lua_QSqlDatabaseClassMethodAddDatabase},
  {NULL, NULL}
};

void static
registerQSqlDatabaseClassMethods( lua_State *L ) {
  luaL_register(L, "QSqlDatabase", QSqlDatabaseClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_QSqlDatabaseInstanceMethodDestruct( lua_State *L ) {
  QSqlDatabase *db= checkQSqlDatabase( L );
  delete db;
  return 0;
}

static int
lua_QSqlDatabaseInstanceMethodSetDatabaseName( lua_State *L ) {
  QSqlDatabase *db = checkQSqlDatabase( L );
  const char * name = luaL_checkstring(L, 2);
  db->setDatabaseName(QString(name));
  return 0;
}

static int
lua_QSqlDatabaseInstanceMethodSetHostName( lua_State *L ) {
  QSqlDatabase *db  = checkQSqlDatabase( L );
  const char * host = luaL_checkstring(L, 2);
  db->setHostName(QString(host));
  return 0;
}

static int
lua_QSqlDatabaseInstanceMethodSetPassword( lua_State *L ) {
  QSqlDatabase *db      = checkQSqlDatabase( L );
  const char * password = luaL_checkstring(L, 2);
  db->setPassword(QString(password));
  return 0;
}

static int
lua_QSqlDatabaseInstanceMethodSetPort( lua_State *L ) {
  QSqlDatabase *db      = checkQSqlDatabase( L );
  int port = luaL_checkinteger(L, 2);
  db->setPort(port);
  return 0;
}

static int
lua_QSqlDatabaseInstanceMethodSetUserName( lua_State *L ) {
  QSqlDatabase *db      = checkQSqlDatabase( L );
  const char * userName = luaL_checkstring(L, 2);
  db->setUserName(QString(userName));
  return 0;
}

static int
lua_QSqlDatabaseInstanceMethodOpen( lua_State *L ) {
  QSqlDatabase *db = checkQSqlDatabase( L );
  lua_pushboolean(L, db->open());
  return 1;
}

static int
lua_QSqlDatabaseInstanceMethodClose( lua_State *L ) {
  QSqlDatabase *db = checkQSqlDatabase( L );
  db->close();
  return 0;
}

static int
lua_QSqlDatabaseInstanceMethodIsOpenError( lua_State *L ) {
  QSqlDatabase *db = checkQSqlDatabase( L );
  lua_pushboolean(L, db->isOpenError());
  return 1;
}

static int
lua_QSqlDatabaseInstanceMethodQuery( lua_State *L ) {
  QSqlDatabase *db = checkQSqlDatabase( L );

  QSqlQuery **ptr = (QSqlQuery**)lua_newuserdata(L, sizeof(QSqlQuery*));
  *ptr = new QSqlQuery(*db);

  luaL_getmetatable(L, "QSqlQuery.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const
luaL_reg QSqlDatabaseInstanceMethods[] = {
  {"query", lua_QSqlDatabaseInstanceMethodQuery},
  {"isOpenError", lua_QSqlDatabaseInstanceMethodIsOpenError},
  {"close", lua_QSqlDatabaseInstanceMethodClose},
  {"open", lua_QSqlDatabaseInstanceMethodOpen},
  {"setPort", lua_QSqlDatabaseInstanceMethodSetPort},
  {"setPassword", lua_QSqlDatabaseInstanceMethodSetPassword},
  {"setHostName", lua_QSqlDatabaseInstanceMethodSetHostName},
  {"setDatabaseName", lua_QSqlDatabaseInstanceMethodSetDatabaseName},
  {"setUserName", lua_QSqlDatabaseInstanceMethodSetUserName},
  {"__gc", lua_QSqlDatabaseInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQSqlDatabaseInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QSqlDatabase.metatable");
  luaL_register(L, NULL, QSqlDatabaseInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QSqlDatabase( lua_State *L ) {
    registerQSqlDatabaseClassMethods(L);
    registerQSqlDatabaseInstanceMethods(L);
    return 1;
  }
}
