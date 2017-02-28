// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "charon/base"

using namespace charon;

Thread::Thread(QByteArray charonPath, QByteArray fileName)
{
  m_charonPath = charonPath;
  m_fileName   = fileName;
}

void Thread::run() {
  int rv, i;
  QByteArray profile, name;
  QVariant value;


  m_luaState = lua_open();
  luaL_openlibs(m_luaState);

  /* CHARON_PATH */
  lua_pushstring(m_luaState, m_charonPath);
  lua_setglobal(m_luaState, "CHARON_PATH");

  /* PROPERTIES */
  QList<QByteArray> list = this->dynamicPropertyNames();
  for(i=0; i < list.size(); i++) {
    name  = list.at(i);
    value = this->property(name);

    if(value.type() == QVariant::ByteArray) {
      lua_pushstring(m_luaState, value.toByteArray());
      lua_setglobal(m_luaState, name);
    }

    if(value.type() == QVariant::Double) {
      lua_pushnumber(m_luaState, value.toDouble());
      lua_setglobal(m_luaState, name);
    }

    if(value.type() == QVariant::Bool) {
      lua_pushboolean(m_luaState, value.toBool());
      lua_setglobal(m_luaState, name);
    }
  }

  /* PROFILE */
  profile = m_charonPath;
  profile.append("/profile.lua");

  rv = luaL_loadfile(m_luaState, profile);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    return;
  }

  rv = lua_pcall(m_luaState, 0, 0, lua_gettop(m_luaState) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    return;
  }

  rv = luaL_loadfile(m_luaState, m_fileName.data());
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    return;
  }

  rv = lua_pcall(m_luaState, 0, 0, lua_gettop(m_luaState) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(m_luaState, -1));
    return;
  }

}
