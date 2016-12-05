// Copyright 2016 The Oberon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>

QDir *
checkQDir( lua_State *L ) {
  return *(QDir **) luaL_checkudata(L, 1, "QDir.metatable");
}

/**
 * ClassMethods
 */

static int
lua_QDirClassMethodNew( lua_State *L ) {
  const char *str = (char *) lua_tostring(L, 1) ;
  QDir **ptr = (QDir **)lua_newuserdata(L, sizeof(QDir*));
  *ptr= new QDir(QString(str));
  luaL_getmetatable(L, "QDir.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
lua_QDirClassMethodHome( lua_State *L ) {
  QDir **ptr = (QDir **)lua_newuserdata(L, sizeof(QDir*));
  *ptr= new QDir(QDir::home());
  luaL_getmetatable(L, "QDir.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
lua_QDirClassMethodTemp( lua_State *L ) {
  QDir **ptr = (QDir **)lua_newuserdata(L, sizeof(QDir*));
  *ptr= new QDir(QDir::temp());
  luaL_getmetatable(L, "QDir.metatable");
  lua_setmetatable(L, -2);
  return 1;
}
static int
lua_QDirClassMethodCurrent( lua_State *L ) {
  QDir **ptr = (QDir **)lua_newuserdata(L, sizeof(QDir*));
  *ptr= new QDir(QDir::current());
  luaL_getmetatable(L, "QDir.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
lua_QDirClassMethodHomePath( lua_State *L ) {
  const char* result = QDir::homePath().toLocal8Bit().data();
  lua_pushstring(L, result);
  return 1;
}

static int
lua_QDirClassMethodTempPath( lua_State *L ) {
  const char* result = QDir::tempPath().toLocal8Bit().data();
  lua_pushstring(L, result);
  return 1;
}

static int
lua_QDirClassMethodCurrentPath( lua_State *L ) {
  const char* result = QDir::currentPath().toLocal8Bit().data();
  lua_pushstring(L, result);
  return 1;
}

static int
lua_QDirClassMethodCleanPath( lua_State *L ) {
  const char * path;
  const char * result;

  path = luaL_checkstring(L, 1);
  result = QDir::cleanPath(path).toLocal8Bit().data();
  lua_pushstring(L, result);
  return 1;
}

static int
lua_QDirClassMethodFromNativeSeparators( lua_State *L ) {
  const char * pathName;
  QString result;
  pathName = luaL_checkstring(L, 1);
  result   = QDir::fromNativeSeparators(pathName);
  lua_pushstring(L, result.toLocal8Bit().data());
  return 1;
}

static int
lua_QDirClassMethodIsAbsolute( lua_State *L ) {
  const char * path;
  bool result;
  path   = luaL_checkstring(L, 1);
  result = QDir::isAbsolutePath(QString(path));
  lua_pushboolean(L, result);
  return 1;
}

static int
lua_QDirClassMethodIsRelative( lua_State *L ) {
  const char * path;
  bool result;
  path   = luaL_checkstring(L, 1);
  result = QDir::isRelativePath(QString(path));
  lua_pushboolean(L, result);
  return 1;
}

static int
lua_QDirClassMethodMatch( lua_State *L ) {
  const char * filter;
  const char * fileName;
  bool result;
  filter   = luaL_checkstring(L, 1);
  fileName = luaL_checkstring(L, 2);
  result = QDir::match(QString(filter), QString(fileName));
  lua_pushboolean(L, result);
  return 1;
}

static int
lua_QDirClassMethodRootPath( lua_State *L ) {
  const char* result = QDir::rootPath().toLocal8Bit().data();
  lua_pushstring(L, result);
  return 1;
}

static int
lua_QDirClassMethodRoot( lua_State *L ) {
  QDir **ptr = (QDir **)lua_newuserdata(L, sizeof(QDir*));
  *ptr= new QDir(QDir::root());
  luaL_getmetatable(L, "QDir.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
lua_QDirClassMethodSeparator( lua_State *L ) {
  QChar separator;
  separator = QDir::separator();
  lua_pushstring(L, separator.decomposition().toLocal8Bit().data());
  return 1;
}

static int
lua_QDirClassMethodSetCurrent( lua_State *L ) {
  const char * path;
  path = luaL_checkstring(L,1);
  lua_pushboolean(L, QDir::setCurrent(path));
  return 1;
}

static int
lua_QDirClassMethodToNativeSeparators( lua_State *L ) {
  const char * pathName;
  pathName = luaL_checkstring(L,1);
  lua_pushstring(L, QDir::toNativeSeparators(QString(pathName)).toLocal8Bit().data());
  return 1;
}


static const luaL_reg QDirClassMethods[] = {
  {"toNativeSeparators", lua_QDirClassMethodToNativeSeparators},
  {"temp", lua_QDirClassMethodTemp},
  {"setCurrent", lua_QDirClassMethodSetCurrent},
  {"separator", lua_QDirClassMethodSeparator},
  {"root", lua_QDirClassMethodRoot},
  {"rootPath", lua_QDirClassMethodRootPath},
  {"match", lua_QDirClassMethodMatch},
  {"isRelativePath", lua_QDirClassMethodIsRelative},
  {"isAbsolutePath", lua_QDirClassMethodIsAbsolute},
  {"fromNativeSeparators", lua_QDirClassMethodFromNativeSeparators},
  {"new", lua_QDirClassMethodNew},
  {"home", lua_QDirClassMethodHome},
  {"homePath", lua_QDirClassMethodHomePath},
  {"tempPath", lua_QDirClassMethodTempPath},
  {"currentPath", lua_QDirClassMethodCurrentPath},
  {"cleanPath", lua_QDirClassMethodCleanPath},
  {"current", lua_QDirClassMethodCurrent},
  {NULL, NULL}
};

void static
registerQDirClassMethods( lua_State *L ) {
  luaL_register(L, "QDir", QDirClassMethods);
}

/**
 * Instance Methods
 */

static int
lua_QDirInstanceMethodAbsolutePath( lua_State *L ) {
  QDir *dir   = checkQDir( L );
  lua_pushstring(L, dir->absolutePath().toLocal8Bit());
  return 1;
}


static int
lua_QDirInstanceMethodIsRelative( lua_State *L) {
  QDir *dir = checkQDir( L );
  lua_pushboolean( L, dir->isRelative() );
  return 1;
}

static int
lua_QDirInstanceMethodIsRoot( lua_State *L) {
  QDir *dir = checkQDir( L );
  lua_pushboolean( L, dir->isRoot() );
  return 1;
}

static int
lua_QDirInstanceMethodCount(lua_State *L) {
  QDir *dir = checkQDir( L );
  lua_pushnumber( L, dir->count() );
  return 1;
}

static int
lua_QDirInstanceMethodDestruct( lua_State *L ) {
  QDir *dir = checkQDir( L );
  delete dir;
  return 0;
}

static int
lua_QDirInstanceMethodDirName( lua_State *L ) {
  QDir *dir = checkQDir( L );
  lua_pushstring(L, dir->dirName().toLocal8Bit().data());
  return 1;
}

static int
lua_QDirInstanceMethodExists( lua_State *L ) {
  QDir *dir = checkQDir( L );
  lua_pushboolean(L, dir->exists());
  return 1;
}

static int
lua_QDirInstanceMethodFilePath( lua_State *L ) {
  const char *fileName;
  QString result;
  QDir *dir;

  dir = checkQDir( L );
  fileName = luaL_checkstring(L, 2);
  result = dir->filePath(QString(fileName));

  lua_pushstring(L, result.toLocal8Bit().data());
  return 1;
}

static int
lua_QDirInstanceMethodIsAbsolute( lua_State *L ) {
  QDir *dir = checkQDir( L );
  lua_pushboolean(L, dir->isAbsolute());
  return 1;
}

static int
lua_QDirInstanceMethodIsReadable( lua_State *L ) {
  QDir *dir = checkQDir( L );
  lua_pushboolean(L, dir->isReadable());
  return 1;
}

static int
lua_QDirInstanceMethodMakeAbsolute( lua_State *L ) {
  QDir *dir = checkQDir( L );
  lua_pushboolean(L, dir->makeAbsolute());
  return 1;
}

static int
lua_QDirInstanceMethodMkdir( lua_State *L ) {
  const char * dirName;
  QDir *dir = checkQDir( L );
  dirName   = luaL_checkstring(L, 2);
  lua_pushboolean(L, dir->mkdir(dirName));
  return 1;
}

static int
lua_QDirInstanceMethodMkpath( lua_State *L ) {
  const char * dirPath;
  QDir *dir = checkQDir( L );
  dirPath = luaL_checkstring(L, 2);
  lua_pushboolean(L, dir->mkdir(dirPath));
  return 1;
}

static int
lua_QDirInstanceMethodPath( lua_State *L ) {
  QDir *dir = checkQDir( L );
  lua_pushstring(L, dir->path().toLocal8Bit().data());
  return 1;
}

static int
lua_QDirInstanceMethodRelativeFilePath( lua_State *L ) {
  const char * filePath;
  QDir *dir = checkQDir( L );
  filePath  = luaL_checkstring(L, 2);
  lua_pushstring(L, dir->relativeFilePath(QString(filePath)).toLocal8Bit().data());
  return 1;
}

static int
lua_QDirInstanceMethodRemove( lua_State *L ) {
  const char * fileName;
  QDir *dir = checkQDir( L );
  fileName  = luaL_checkstring(L, 2);
  lua_pushboolean(L, dir->remove(QString(fileName)));
  return 1;
}

static int
lua_QDirInstanceMethodRemoveRecursively( lua_State *L ) {
  QDir *dir = checkQDir( L );
  lua_pushboolean(L, dir->removeRecursively());
  return 1;
}

static int
lua_QDirInstanceMethodRename( lua_State *L ) {
  const char * oldName;
  const char * newName;

  QDir *dir = checkQDir( L );
  oldName   = luaL_checkstring(L, 2);
  newName   = luaL_checkstring(L, 3);

  lua_pushboolean(L, dir->rename(QString(oldName), QString(newName)));
  return 1;
}

static int
lua_QDirInstanceMethodRmdir( lua_State *L ) {
  const char * dirName;

  QDir *dir = checkQDir( L );
  dirName   = luaL_checkstring(L, 2);

  lua_pushboolean(L, dir->rmdir(QString(dirName)));
  return 1;
}

static int
lua_QDirInstanceMethodRmpath( lua_State *L ) {
  const char * dirPath;

  QDir *dir = checkQDir( L );
  dirPath   = luaL_checkstring(L, 2);

  lua_pushboolean(L, dir->rmpath(QString(dirPath)));
  return 1;
}

static int
lua_QDirInstanceMethodSetPath( lua_State *L ) {
  const char * path;

  QDir *dir = checkQDir( L );
  path      = luaL_checkstring(L, 2);

  dir->setPath(QString(path));
  return 0;
}

static int
lua_QDirInstanceMethodSwap( lua_State *L ) {
  QDir *dir   = checkQDir( L );
  QDir *other = *(QDir **) luaL_checkudata(L, 2, "QDir.metatable");

  dir->swap(*other);
  return 0;
}

static int
lua_QDirInstanceMethodSearchPaths( lua_State *L ) {
  QDir *dir   = checkQDir( L );
  const char * prefix = luaL_checkstring(L, 2);

  QStringList **ptr = (QStringList **)lua_newuserdata(L, sizeof(QStringList*));
  *ptr= new QStringList(dir->searchPaths(QString(prefix)));

  luaL_getmetatable(L, "QStringList.metatable");
  lua_setmetatable(L, -2);

  return 1;
}


static int
lua_QDirInstanceMethodEntryList( lua_State *L ) {
  QDir *dir   = checkQDir( L );

  QStringList **ptr = (QStringList **)lua_newuserdata(L, sizeof(QStringList*));
  *ptr= new QStringList(dir->entryList());

  luaL_getmetatable(L, "QStringList.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static int
lua_QDirInstanceMethodEntryInfoList( lua_State *L ) {
  QDir *dir   = checkQDir( L );

  QList<QFileInfo> **ptr = (QList<QFileInfo> **)lua_newuserdata(L, sizeof(QList<QFileInfo>*));
  *ptr= new QList<QFileInfo>(dir->entryInfoList());

  luaL_getmetatable(L, "QFileInfoList.metatable");
  lua_setmetatable(L, -2);

  return 1;
}

static const
luaL_reg QDirMethods[] = {
  {"absolutePath", lua_QDirInstanceMethodAbsolutePath},
  {"searchPaths", lua_QDirInstanceMethodSearchPaths},
  {"entryInfoList", lua_QDirInstanceMethodEntryInfoList},
  {"entryList", lua_QDirInstanceMethodEntryList},
  {"swap", lua_QDirInstanceMethodSwap},
  {"setPath", lua_QDirInstanceMethodSetPath},
  {"rmpath", lua_QDirInstanceMethodRmpath},
  {"rmdir", lua_QDirInstanceMethodRmdir},
  {"rename", lua_QDirInstanceMethodRename},
  {"removeRecursively", lua_QDirInstanceMethodRemoveRecursively},
  {"remove", lua_QDirInstanceMethodRemove},
  {"relativeFilePath", lua_QDirInstanceMethodRelativeFilePath},
  {"path", lua_QDirInstanceMethodPath},
  {"mkpath", lua_QDirInstanceMethodMkpath},
  {"mkdir", lua_QDirInstanceMethodMkdir},
  {"makeAbsolute", lua_QDirInstanceMethodMakeAbsolute},
  {"isReadable", lua_QDirInstanceMethodIsReadable},
  {"isAbsolute", lua_QDirInstanceMethodIsAbsolute},
  {"filePath", lua_QDirInstanceMethodFilePath},
  {"exists", lua_QDirInstanceMethodExists},
  {"dirName", lua_QDirInstanceMethodDirName},
  {"isRelative", lua_QDirInstanceMethodIsRelative},
  {"isRoot", lua_QDirInstanceMethodIsRoot},
  {"count", lua_QDirInstanceMethodCount},
  {"__gc", lua_QDirInstanceMethodDestruct},
  {NULL, NULL}
};

void static
registerQDirInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QDir.metatable");
  luaL_register(L, NULL, QDirMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

/**
 * Open QDir
 */

extern "C" {
  int
  luaopen_QDir( lua_State *L ) {
    registerQDirClassMethods(L);
    registerQDirInstanceMethods(L);
    return 1;
  }
}
