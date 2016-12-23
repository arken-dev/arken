// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <iostream>
#include <lua/lua.hpp>
#include <charon/helper>
#include <QtCore>
#include <QCoreApplication>

using charon::ByteArray;

int main(int argc, char * argv[])
{
  QCoreApplication app(argc, argv);

  lua_State * L = Charon::init(argc, argv, app.applicationFilePath().toLocal8Bit().data());

  lua_settop(L, 0);
  lua_getglobal(L, "CHARON_PATH");

  QByteArray rhea = lua_tostring(L, 1);
  rhea.append("/lib/rhea.lua");

  int rv;
  rv = luaL_loadfile(L, rhea);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
    throw;
  }

  rv = lua_pcall(L, 0, 0, 0);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
    throw;
  }

  lua_settop(L, 0);
  lua_getglobal(L, "rhea");

  if( lua_pcall(L, 0, 0, 0) != 0 ) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
  } else {
    return 0;
  }

}
