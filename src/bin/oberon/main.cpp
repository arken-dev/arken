#include <luajit-2.0/lua.hpp>
#include <iostream>
#include <QtCore>
#include <QCoreApplication>
#include <oberon/helper>
#include <OByteArray>

void oberonPopulateArg(lua_State *L, int argc, char * argv[])
{
  int top, i;

  lua_newtable(L);
  top = lua_gettop(L);
  for(i=0; i < argc; i++) {
    lua_pushinteger(L, i);
    lua_pushstring(L, argv[i]);
    lua_settable(L, top);
  }
  lua_setglobal( L, "arg");
}

void oberonResolvPath(lua_State *L, QCoreApplication &app,
  OByteArray &oberonPath)
{
  QString dirPath;

  dirPath = app.applicationDirPath();
  dirPath.truncate( dirPath.lastIndexOf('/') );
  oberonPath = dirPath.toLocal8Bit();
  if( strcmp(os::name(), "windows") == 0 ) {
    oberonPath = oberonPath.capitalize();
  }
  lua_pushstring(L, oberonPath);
  lua_setglobal(L, "OBERON_PATH");
}

int oberonProfileLoad(lua_State *L, OByteArray &oberonPath)
{
  int rv;
  OByteArray profile;

  profile = oberonPath;
  profile.append("/profile.lua");

  rv = luaL_loadfile(L, profile);
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

int oberonTaskLoad(lua_State *L, OByteArray &oberonPath)
{
  int rv;
  OByteArray task;

  task = oberonPath;
  task.append("/lib/task.lua");

  rv = luaL_loadfile(L, task);
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
  QFile      file(argv[1]);
  QCoreApplication app(argc, argv);
  lua_State *L = lua_open();
  luaL_openlibs(L);

  oberonPopulateArg(L, argc, argv);

  oberonResolvPath(L , app, oberonPath);

  rv = oberonProfileLoad(L, oberonPath);
  if (rv) {
    return rv;
  }

  if (file.fileName().isEmpty()) {
    rv = oberonConsoleLoad(L);
  }

  if(file.exists()) {
    rv = oberonFileLoad(L, file);
  }

  if(!file.exists()) {
    rv = oberonTaskLoad(L, oberonPath);
  }

  return rv;
}
