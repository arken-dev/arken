#include <lua/lua.hpp>
#include <iostream>
#include <QtCore>
#include <QCoreApplication>
#include <oberon/helper>
#include <OByteArray>

int oberonFileLoad(lua_State *L, QFile &file)
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

int oberonTaskLoad(lua_State *L, char * task)
{
  int rv;
  OByteArray lib;

  lua_settop(L, 0);
  lua_getglobal(L, "OBERON_PATH");

  lib = lua_tostring(L, 1);

  lua_pushstring(L, task);
  lua_setglobal(L, "OBERON_TASK");

  lib.append("/lib/task.lua");

  rv = luaL_loadfile(L, lib);
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

void oberonConsolePrintAround(OByteArray &buffer)
{
  buffer.remove(0, 1);
  buffer.prepend("print(");
  buffer.append(")");
}

bool oberonConsoleIncrementLevel(OByteArray &row)
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

bool oberonConsoleDecrementLevel(OByteArray &row)
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
int oberonConsoleLoad(lua_State *L)
{
  int rv = 0;
  int level = 0;
  std::string line;
  OByteArray  row;
  OByteArray  buffer;

  while(true) {
    std::cout << "oberon" << level << "> ";

    std::getline(std::cin, line);
    row = line.c_str();

    if (level == 0) {
      buffer = line.c_str();
    } else {
      buffer.append(line.c_str());
      buffer.append("\n");
    }

    if (buffer.startsWith('=')) {
      oberonConsolePrintAround(buffer);
    }

    if (oberonConsoleIncrementLevel(row)) {
      buffer.append("\n");
      level ++ ;
    }

    if (oberonConsoleDecrementLevel(row)) {
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
  OByteArray oberonPath;
  OByteArray task;
  QString    dirPath;
  QFile      file(argv[1]);
  QCoreApplication app(argc, argv);
  lua_State  * L;
  oberonPath = app.applicationFilePath().toLocal8Bit().data();

  L = Oberon::init(argc, argv, oberonPath);
  if (L == 0) {
    fprintf(stderr, "failure allocate memory\n");
    return 1;
  }

  if (file.fileName().isEmpty()) {
    rv = oberonConsoleLoad(L);
  }

  if(file.exists()) {
    rv = oberonFileLoad(L, file);
  }

  if(!file.exists()) {
    rv = oberonTaskLoad(L, argv[1]);
  }

  return rv;
}
