#include <luajit-2.0/lua.hpp>
#include <QtCore>

/**
 * CheckUserData
 */

QFile *
checkQFile( lua_State *L ) {
  return *(QFile **) luaL_checkudata(L, 1, "QFile.metatable");
}

/**
 * ClassMethods
 */

static int
lua_QFileClassMethodNew( lua_State *L ) {
  QFile **ptr;
  ptr = (QFile **)lua_newuserdata(L, sizeof(QFile*));

  if(lua_isnil(L, 1)) {
    *ptr= new QFile();
  } else {
    const char *str = (char *) lua_tostring(L, 1);
    *ptr= new QFile(QString(str));
  }
  luaL_getmetatable(L, "QFile.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
lua_QFileClassMethodCopy( lua_State *L ) {
  const char *fileName, *newName;
  size_t flen, nlen;
  bool result;
  fileName = luaL_checklstring(L, 1, &flen);
  newName  = luaL_checklstring(L, 2, &nlen);

  result = QFile::copy(fileName, newName);

  lua_pushboolean(L, result);
  return 1;
}

static int
lua_QFileClassMethodEncodeName( lua_State *L ) {
  const char *localFileName;
  size_t len;
  localFileName = luaL_checklstring(L, 1, &len);
  lua_pushstring(L, QFile::encodeName(QString(localFileName)));
  return 1;
}

static int
lua_QFileClassMethodExists( lua_State *L ) {
  const char *fileName;
  size_t len;
  bool result;
  fileName = luaL_checklstring(L, 1, &len);
  result = QFile::exists(QString(fileName));
  lua_pushboolean(L, result);
  return 1;
}

static int
lua_QFileClassMethodLink( lua_State *L ) {
  const char *fileName, *linkName;
  bool result;
  fileName = luaL_checkstring(L, 1);
  linkName = luaL_checkstring(L, 2);

  result = QFile::link(QString(fileName), QString(linkName));
  lua_pushboolean(L, result);
  return 1;
}

static int
lua_QFileClassMethodRemove( lua_State *L ) {
  const char *fileName;
  bool result;
  fileName = luaL_checkstring(L, 1);

  result = QFile::remove(QString(fileName));
  lua_pushboolean(L, result);
  return 1;
}

static int
lua_QFileClassMethodRename( lua_State *L ) {
  const char *oldName, *newName;
  bool result;

  oldName = luaL_checkstring(L, 1);
  newName = luaL_checkstring(L, 2);

  result = QFile::rename(QString(oldName), QString(newName));

  lua_pushboolean(L, result);
  return 1;
}

static int
lua_QFileClassMethodSymLinkTarget( lua_State *L ) {
  const char * fileName;
  QString result;

  fileName = luaL_checkstring( L , 1 );
  result   = QFile::symLinkTarget(QString(fileName));

  lua_pushstring( L , result.toLocal8Bit().data() );
  return 1;
}

static const luaL_reg QFileClassMethods[] = {
  {"new", lua_QFileClassMethodNew},
  {"copy", lua_QFileClassMethodCopy},
  {"encodeName", lua_QFileClassMethodEncodeName},
  {"exists", lua_QFileClassMethodExists},
  {"link", lua_QFileClassMethodLink},
  {"remove", lua_QFileClassMethodRemove},
  {"rename", lua_QFileClassMethodRename},
  {"symLinkTarget", lua_QFileClassMethodSymLinkTarget},
  {NULL, NULL}
};

void static
registerQFileClassMethods( lua_State *L ) {
  luaL_register(L, "QFile", QFileClassMethods);
}

/**
 * InstanceMethods
 */

static int
lua_QFileInstanceMethodCopy( lua_State *L ) {
  QFile *file;
  const char *newName;
  size_t len;
  file = checkQFile( L );
  newName = luaL_checklstring(L, 2, &len);
  lua_pushboolean(L, file->copy(newName));
  return 1;
}

static int
lua_QFileInstanceMethodClose( lua_State *L ) {
  QFile *file= checkQFile( L );
  file->close();
  return 0;
}

static int
lua_QFileInstanceMethodFileName( lua_State *L ) {
  QFile *file;
  file = checkQFile( L );
  lua_pushstring(L, file->fileName().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInstanceMethodExists( lua_State *L ) {
  QFile *file;
  file = checkQFile( L );
  lua_pushboolean(L, file->exists());
  return 1;
}

static int
lua_QFileInstanceMethodLink( lua_State *L ) {
  const char * linkName;
  QFile *file;

  file = checkQFile( L );
  linkName = luaL_checkstring(L, 2);

  lua_pushboolean(L, file->link(QString(linkName)));
  return 1;
}

static int
lua_QFileInstanceMethodRemove( lua_State *L ) {
  QFile *file;
  file = checkQFile( L );
  lua_pushboolean(L, file->remove());
  return 1;
}

static int
lua_QFileInstanceMethodRename( lua_State *L ) {
  QFile *file;
  const char *newName;
  bool result;

  file = checkQFile( L );
  newName = luaL_checkstring(L, 2);

  result = file->rename(QString(newName));

  lua_pushboolean( L, result );
  return 1;
}

static int
lua_QFileInstanceMethodSetFileName( lua_State *L ) {
  QFile *file;
  const char *name;

  file = checkQFile( L );
  name = luaL_checkstring(L, 2);

  file->setFileName(QString(name));

  return 0;
}


static int
lua_QFileInstanceMethodDestruct( lua_State *L ) {
  QFile *udata = checkQFile( L );
  delete udata;
  return 0;
}

static int
lua_QFileInstanceMethodSymLinkTarget( lua_State *L ) {
  QFile *file;
  QString result;

  file = checkQFile( L );

  result = file->symLinkTarget();
  lua_pushstring( L , result.toLocal8Bit().data() );
  return 1;
}

static const
luaL_reg QFileInstanceMethods[] = {
  {"__gc", lua_QFileInstanceMethodDestruct},
  {"copy", lua_QFileInstanceMethodCopy},
  {"close", lua_QFileInstanceMethodClose},
  {"fileName", lua_QFileInstanceMethodFileName},
  {"exists", lua_QFileInstanceMethodExists},
  {"link", lua_QFileInstanceMethodLink},
  {"remove", lua_QFileInstanceMethodRemove},
  {"rename", lua_QFileInstanceMethodRename},
  {"setFileName", lua_QFileInstanceMethodSetFileName},
  {"symLinkTarget", lua_QFileInstanceMethodSymLinkTarget},
  {NULL, NULL}
};

void static
registerQFileInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QFile.metatable");
  luaL_register(L, NULL, QFileInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QFile( lua_State *L ) {
    registerQFileClassMethods(L);
    registerQFileInstanceMethods(L);
    return 1;
  }
}
