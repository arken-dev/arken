// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <QCoreApplication>
#include <QDebug>
#include <QStringList>
#include <charon/base>
#include <triton.h>
#include <QJsonObject>

using charon::ByteArray;
using charon::ByteArrayList;
using charon::mvm;

int main(int argc, char * argv[])
{

  int threads = os::cores();
  QCoreApplication app(argc, argv);
  mvm::init(argc, argv);
  charon::instance i = mvm::instance();
  lua_State * L = i.state();

  triton_register(L);

  if( argc == 1 ) {
    fprintf(stderr, "missing param, see your triton file\n");
    throw;
  }

  QByteArray fileName;
  fileName.prepend("triton.");
  fileName.append(argv[1]);

  int rv;

  lua_settop(L, 0);
  lua_getglobal(L, "require");
  lua_pushstring(L, fileName);

  rv = lua_pcall(L, 1, 1, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
    return rv;
  }

  if(!lua_istable(L, -1)) {
    luaL_error(L, "table not found");
  }

  lua_pushstring(L, "start");
  lua_gettable(L, -2);

  if( lua_pcall(L, 0, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
  }

  QFile config("config/triton.json");
  if( config.exists() ) {
    config.open(QIODevice::ReadOnly);
    QJsonParseError * error = new QJsonParseError();
    QJsonDocument json = QJsonDocument::fromJson(config.readAll(), error);
    if( error->error != 0 ) {
      qDebug() << error->errorString();
    }
    QJsonObject object = json.object();
    threads = object.value("threads").toInt();
  }

  QThreadPool * pool = new QThreadPool;
  pool->setMaxThreadCount(threads);

  /*
   * Pre initialize instances
   */
  QList<Triton *> list;
  for(int i = 0; i < threads; i++ ) {
    list.append(new Triton(argc, argv, fileName));
  }

  for(int i = 0; i < threads; i++ ) {
    pool->start(list.at(i));
  }

  pool->waitForDone();

  if(!lua_istable(L, -1)) {
    luaL_error(L, "no final nao e table table not found");
  }

  lua_pushstring(L, "stop");
  lua_gettable(L, -2);

  if( lua_pcall(L, 0, 0, 0) != 0 ) {
    fprintf(stderr, " %s\n", lua_tostring(L, -1));
  }

  return 0;
}
