// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_THREAD_
#define _CHARON_THREAD_

#include <lua/lua.hpp>
#include <QtCore>
#include <QThread>

namespace charon
{

  class Thread : public QThread
  {
    public:
    QByteArray   m_charonPath;
    QByteArray   m_fileName;
    lua_State  * m_luaState;

    explicit Thread(QByteArray charonPath, QByteArray fileName);
    void run();
  };

}

#endif
