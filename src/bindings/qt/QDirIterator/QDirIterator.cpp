#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>

/**
 * checkQDirIterator
 */

QDirIterator *
checkQDirIterator( lua_State *L ) {
  return *(QDirIterator **) luaL_checkudata(L, 1, "QDirIterator.metatable");
}

/**
 * ClassMethods
 */

static QDirIterator::IteratorFlag getFlag(const char * strflag) {
  if( strcmp(strflag, "NoIteratorFlags") == 0 ) {
    return QDirIterator::NoIteratorFlags;
  }

  if( strcmp(strflag,"Subdirectories") == 0 ) {
    return QDirIterator::Subdirectories;
  }

  if( strcmp(strflag, "FollowSymlinks") == 0 ) {
    return QDirIterator::FollowSymlinks;
  }

  return QDirIterator::NoIteratorFlags;
}

static int
lua_QDirIteratorClassMethodNew( lua_State *L ) {
  QDirIterator **ptr;
  QDirIterator::IteratorFlags flags;
  const char *str = (char *) lua_tostring(L, 1);
  int i;
  if(lua_istable(L, 2)) {
    for(i=1; i <= 3; ++i) {
      lua_rawgeti(L, 2, i);
      if(lua_isstring(L, -1)) {
        if(flags) {
          flags = flags | getFlag(lua_tostring(L, -1));
        } else {
          flags = getFlag(lua_tostring(L, -1));
        }
      } else {
        break;
      }
    }

    ptr = (QDirIterator **)lua_newuserdata(L, sizeof(QDirIterator*));
    *ptr= new QDirIterator(QString(str), flags);
  } else {
    ptr = (QDirIterator **)lua_newuserdata(L, sizeof(QDirIterator*));
    *ptr= new QDirIterator(QString(str));
  }
  luaL_getmetatable(L, "QDirIterator.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static const luaL_reg QDirIteratorClassMethods[] = {
  {"new", lua_QDirIteratorClassMethodNew},
  {NULL, NULL}
};

void static
registerQDirIteratorClassMethods( lua_State *L ) {
  luaL_register(L, "QDirIterator", QDirIteratorClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_QDirIteratorInstanceMethodDestruct( lua_State *L ) {
  QDirIterator *udata = checkQDirIterator( L );
  delete udata;
  return 0;
}

static int
lua_QDirIteratorInstanceMethodFileInfo( lua_State *L ) {
  QFileInfo **ptr  = NULL;
  QDirIterator *qdirIterator = checkQDirIterator( L );
  QFileInfo fileInfo = qdirIterator->fileInfo();
  ptr  = (QFileInfo **)lua_newuserdata(L, sizeof(QFileInfo*));
  *ptr = new QFileInfo(fileInfo);
  luaL_getmetatable(L, "QFileInfo.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_QDirIteratorInstanceMethodHasNext( lua_State *L ) {
  QDirIterator *qdirIterator = checkQDirIterator( L );
  lua_pushboolean(L, qdirIterator->hasNext());
  return 1;
}

static int
lua_QDirIteratorInstanceMethodNext( lua_State *L ) {
  QDirIterator *qdirIterator = checkQDirIterator( L );
  lua_pushstring(L, qdirIterator->next().toLocal8Bit().data());
  return 1;
}

static int
lua_QDirIteratorInstanceMethodFileName(lua_State *L) {
  QDirIterator *qdirIterator = checkQDirIterator( L );
  lua_pushstring(L, qdirIterator->fileName().toLocal8Bit().data());
  return 1;
}

static int
lua_QDirIteratorInstanceMethodFilePath(lua_State *L) {
  QDirIterator *qdirIterator = checkQDirIterator( L );
  lua_pushstring(L, qdirIterator->filePath().toLocal8Bit().data());
  return 1;
}

static int
lua_QDirIteratorInstanceMethodPath(lua_State *L) {
  QDirIterator *qdirIterator = checkQDirIterator( L );
  lua_pushstring(L, qdirIterator->path().toLocal8Bit().data());
  return 1;
}

static const
luaL_reg QDirIteratorInstanceMethods[] = {
  {"fileInfo", lua_QDirIteratorInstanceMethodFileInfo},
  {"fileName", lua_QDirIteratorInstanceMethodFileName},
  {"filePath", lua_QDirIteratorInstanceMethodFilePath},
  {"path", lua_QDirIteratorInstanceMethodPath},
  {"next", lua_QDirIteratorInstanceMethodNext},
  {"hasNext", lua_QDirIteratorInstanceMethodHasNext},
  {"__gc", lua_QDirIteratorInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQDirIteratorInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QDirIterator.metatable");
  luaL_register(L, NULL, QDirIteratorInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QDirIterator( lua_State *L ) {
    registerQDirIteratorClassMethods(L);
    registerQDirIteratorInstanceMethods(L);
    return 1;
  }
}
