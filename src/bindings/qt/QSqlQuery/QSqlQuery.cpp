// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <iostream>

/**
 * checkQSqlQuery
 */

QSqlQuery *
checkQSqlQuery( lua_State *L ) {
  return *(QSqlQuery **) luaL_checkudata(L, 1, "QSqlQuery.metatable");
}

/**
 * ClassMethods
 */

static int
lua_QSqlQueryClassMethodNew( lua_State *L ) {
  const char *str = (char *) lua_tostring(L, 1);
  QSqlQuery **ptr = (QSqlQuery **)lua_newuserdata(L, sizeof(QSqlQuery*));
  *ptr= new QSqlQuery(QString(str));
  luaL_getmetatable(L, "QSqlQuery.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg QSqlQueryClassMethods[] = {
  {"new", lua_QSqlQueryClassMethodNew},
  {NULL, NULL}
};

void static
registerQSqlQueryClassMethods( lua_State *L ) {
  luaL_register(L, "QSqlQuery", QSqlQueryClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_QSqlQueryInstanceMethodDestruct( lua_State *L ) {
  QSqlQuery *udata = checkQSqlQuery( L );
  delete udata;
  return 0;
}

static int
lua_QSqlQueryInstanceMethodExec( lua_State *L ) {
  QSqlQuery *query = checkQSqlQuery( L );
  const char *str  = (char *) lua_tostring(L, 2);
  lua_pushboolean(L, query->exec(QString(str)));

  return 1;
}

static int
lua_QSqlQueryInstanceMethodNext( lua_State *L ) {
  QSqlQuery *query = checkQSqlQuery( L );
  lua_pushboolean(L, query->next());

  return 1;
}

static int
lua_QSqlQueryInstanceMethodValue( lua_State *L ) {
  QSqlQuery *query = checkQSqlQuery( L );
  QByteArray byte;

  if (lua_isnumber(L, 2)) {
    lua_pushstring(L, query->value(lua_tonumber(L, 2)).toByteArray().data());
  } else {
    lua_pushstring(L, query->value(lua_tostring(L, 2)).toByteArray().data());
  }

  return 1;
}

static const
luaL_reg QSqlQueryInstanceMethods[] = {
  {"value", lua_QSqlQueryInstanceMethodValue},
  {"next", lua_QSqlQueryInstanceMethodNext},
  {"exec", lua_QSqlQueryInstanceMethodExec},
  {"__gc", lua_QSqlQueryInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQSqlQueryInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QSqlQuery.metatable");
  luaL_register(L, NULL, QSqlQueryInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QSqlQuery( lua_State *L ) {
    registerQSqlQueryClassMethods(L);
    registerQSqlQueryInstanceMethods(L);
    return 1;
  }
}
