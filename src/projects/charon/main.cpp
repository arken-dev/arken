// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <charon/base>
#include <charon/mvm>
#include <iostream>
#include <QtCore>
#include <QCoreApplication>

using charon::ByteArray;

int charonFileLoad(lua_State *L, QFile &file)
{
  int rv;

  rv = luaL_loadfile(L, file.fileName().toLocal8Bit());
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
    return rv;
  }

  rv = lua_pcall(L, 0, 0, lua_gettop(L) - 1);
  if (rv) {
    fprintf(stderr, "%s\n", lua_tostring(L, -1));
    return rv;
  }

  return rv;
}

void charonConsolePrintAround(ByteArray &buffer)
{
  buffer.remove(0, 1);
  buffer.prepend("print(");
  buffer.append(")");
}

bool charonConsoleIncrementLevel(ByteArray &row)
{
  /* if */
  if(row.startsWith("if ") or row.contains(" if ")) {
    return true;
  }

  /* for */
  if(row.startsWith("for ") or row.contains(" for ")) {
    return true;
  }

  /* while */
  if(row.startsWith("while ") or row.contains(" while ")) {
    return true;
  }

  /* function */
  if(row.startsWith("function ") or row.contains("function(")) {
    return true;
  }

  return false;
}

bool charonConsoleDecrementLevel(ByteArray &row)
{
  /* end */
  if(row.startsWith("end") or row.contains(" end ")) {
    return true;
  }

  return false;
}

/* TODO
 1) level for if, while etc
 2) set up
 3) ctrl + r
*/
int charonConsoleLoad(lua_State *L)
{
  int rv = 0;
  int level = 0;
  std::string line;
  ByteArray  row;
  ByteArray  buffer;

  while(true) {
    std::cout << "charon" << level << "> ";

    std::getline(std::cin, line);
    row = line.c_str();

    if (level == 0) {
      buffer = line.c_str();
    } else {
      buffer.append(line.c_str());
      buffer.append("\n");
    }

    if (buffer.startsWith('=')) {
      charonConsolePrintAround(buffer);
    }

    if (charonConsoleIncrementLevel(row)) {
      buffer.append("\n");
      level ++ ;
    }

    if (charonConsoleDecrementLevel(row)) {
      level -- ;
    }

    if (level == 0) {
      rv  = luaL_loadstring(L, buffer);
      rv  = lua_pcall(L, 0, 0, 0);
      if (rv) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
      }
    }
    if (level < 0) {
      std::cout << "invalid end statement\n";
      level = 0;
    }
  }
  return rv;
}

int main(int argc, char * argv[])
{
  int rv = 0;
  ByteArray charonPath;
  ByteArray task;
  QString    dirPath;
  QFile      file(argv[1]);
  QCoreApplication app(argc, argv);
  charon::mvm::init(&app);
  lua_State  * L;
  charonPath = app.applicationFilePath().toLocal8Bit().data();

  L = Charon::init(argc, argv, charonPath);
  if (L == 0) {
    fprintf(stderr, "failure allocate memory\n");
    return 1;
  }

  if (file.fileName().isEmpty()) {
    rv = charonConsoleLoad(L);
  }

  if(file.exists()) {
    rv = charonFileLoad(L, file);
  } else {
    fprintf(stderr, "No such file or directory %s\n", argv[1]);
    return 1;
  }

  return rv;
}
