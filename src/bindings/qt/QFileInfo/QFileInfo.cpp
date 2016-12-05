// Copyright 2016 The Oberon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <lua/lua.hpp>
#include <QtCore>
#include <iostream>

QFileInfo *
checkQFileInfo( lua_State *L ) {
  return *(QFileInfo **) luaL_checkudata(L, 1, "QFileInfo.metatable");
}

static int
lua_QFileInfoClassMethodNew( lua_State *L ) {
  const char *str;
  QFileInfo **file = NULL;
  QFileInfo **ptr  = NULL;
  if(lua_isnil(L, 1)) {
    ptr  = (QFileInfo **)lua_newuserdata(L, sizeof(QFileInfo*));
    *ptr = new QFileInfo();
  } else if(lua_isstring(L, 1)) {
    ptr  = (QFileInfo **)lua_newuserdata(L, sizeof(QFileInfo*));
    str  = lua_tostring(L, 1);
    *ptr = new QFileInfo(QString(str));
  } else {
    *file = *(QFileInfo **) luaL_checkudata(L, 1, "QFileInfo.metatable");
    ptr   = (QFileInfo **)lua_newuserdata(L, sizeof(QFileInfo*));
    *ptr  = new QFileInfo(**file);
  }

  luaL_getmetatable(L, "QFileInfo.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
lua_QFileInfoClassMethodExists( lua_State *L ) {
  const char *str = luaL_checkstring(L, 1);
  lua_pushboolean(L, QFileInfo::exists(str));
  return 1;
}

/**
 * Register ClassMethods
 */

static const luaL_reg QFileInfoClassMethods[] = {
  {"exists", lua_QFileInfoClassMethodExists},
  {"new", lua_QFileInfoClassMethodNew},
  {NULL, NULL}
};

void static
registerQFileInfoClassMethods( lua_State *L ) {
  luaL_register(L, "QFileInfo", QFileInfoClassMethods);
}

/**
 * Register Instance Methods
 */
static int
lua_QFileInfoInstanceMethodDestruct( lua_State *L ) {
  QFileInfo *udata = checkQFileInfo( L );
  delete udata;
  return 0;
}

static int
lua_QFileInfoInstanceMethodClass( lua_State *L ) {
  lua_pushstring(L, "QFileInfo");
  return 1;
}

static int
lua_QFileInfoInstanceMethodAbsoluteFilePath( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->absoluteFilePath().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodAbsolutePath( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->absolutePath().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodBaseName( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->baseName().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodBundleName( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->bundleName().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodCaching( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->caching());
  return 1;
}

static int
lua_QFileInfoInstanceMethodCanonicalFilePath( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->canonicalFilePath().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodCanonicalPath( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->canonicalPath().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodCompleteBaseName( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->completeBaseName().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodCompleteSuffix( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->completeSuffix().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodExists( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->exists());
  return 1;
}

static int
lua_QFileInfoInstanceMethodFileName( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->fileName().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodFilePath( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->filePath().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodGroup( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->group().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodGroupId( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushinteger(L, fileInfo->groupId());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsAbsolute( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isAbsolute());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsBundle( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isBundle());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsDir( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isDir());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsExecutable( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isExecutable());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsFile( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isFile());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsHidden( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isHidden());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsNativePath( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isNativePath());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsReadable( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isReadable());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsRelative( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isRelative());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsRoot( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isRoot());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsSymLink( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isSymLink());
  return 1;
}

static int
lua_QFileInfoInstanceMethodIsWritable( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->isWritable());
  return 1;
}

static int
lua_QFileInfoInstanceMethodMakeAbsolute( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushboolean(L, fileInfo->makeAbsolute());
  return 1;
}

static int
lua_QFileInfoInstanceMethodOwner( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->owner().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodOwnerId( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushinteger(L, fileInfo->ownerId());
  return 1;
}

static int
lua_QFileInfoInstanceMethodPath( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  lua_pushstring(L, fileInfo->path().toLocal8Bit().data());
  return 1;
}

static int
lua_QFileInfoInstanceMethodRefresh( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  fileInfo->refresh();
  return 0;
}

static int
lua_QFileInfoInstanceMethodSetCaching( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo( L );
  bool cache = lua_toboolean(L, 1);
  fileInfo->setCaching(cache);
  return 0;
}

static int
lua_QFileInfoInstanceMethodSetFile( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo(L);
  char const * file = luaL_checkstring(L, 2);
  fileInfo->setFile(file);
  return 0;
}

static int
lua_QFileInfoInstanceMethodSize( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo(L);
  qint64 size = fileInfo->size();
  lua_pushinteger(L, size);
  return 1;
}

static int
lua_QFileInfoInstanceMethodSuffix( lua_State *L ) {
  QFileInfo *fileInfo = checkQFileInfo(L);
  QString suffix = fileInfo->suffix();
  QByteArray byteArray = suffix.toLocal8Bit();
  lua_pushlstring(L, byteArray.data(), byteArray.size());
  return 1;
}

static int
lua_QFileInfoInstanceMethodSymLinkTarget( lua_State *L ) {
  QFileInfo *fileInfo  = checkQFileInfo(L);
  QByteArray byteArray = fileInfo->symLinkTarget().toLocal8Bit();
  lua_pushlstring(L, byteArray.data(), byteArray.size());
  return 1;
}

static int
lua_QFileInfoInstanceMethodLastModified( lua_State *L ) {
  QDateTime **ptr        = NULL;
  QFileInfo *fileInfo    = checkQFileInfo(L);
  QDateTime lastModified;


  //lua_getglobal(L, "QDateTime");
  //if(lua_isnil(L, -1)) {
  //  std::cout << "QDateTime not loaded require 'QDateTime'";
  //} else {
    lastModified = fileInfo->lastModified();
    ptr  = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
    *ptr = new QDateTime(lastModified);
    luaL_getmetatable(L, "QDateTime.metatable");
    lua_setmetatable(L, -2);
  //}
  return 1;
}

static int
lua_QFileInfoInstanceMethodLastRead( lua_State *L ) {
  QDateTime **ptr     = NULL;
  QFileInfo *fileInfo = checkQFileInfo(L);
  QDateTime lastRead  = fileInfo->lastRead();

  ptr  = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
  *ptr = new QDateTime(lastRead);

  luaL_getmetatable(L, "QDateTime.metatable");
  lua_setmetatable(L, -2);
  return 1;
}

static int
lua_QFileInfoInstanceMethodCreated( lua_State *L ) {
  QDateTime **ptr        = NULL;
  QFileInfo *fileInfo    = checkQFileInfo(L);
  QDateTime lastModified = fileInfo->created();

  ptr  = (QDateTime **)lua_newuserdata(L, sizeof(QDateTime*));
  *ptr = new QDateTime(lastModified);

  luaL_getmetatable(L, "QDateTime.metatable");
  lua_setmetatable(L, -2);
  return 1;
}


static const
luaL_reg QFileInfoInstanceMethods[] = {
  {"created", lua_QFileInfoInstanceMethodCreated},
  {"lastModified", lua_QFileInfoInstanceMethodLastModified},
  {"lastRead", lua_QFileInfoInstanceMethodLastRead},
  {"symLinkTarget", lua_QFileInfoInstanceMethodSymLinkTarget},
  {"suffix", lua_QFileInfoInstanceMethodSuffix},
  {"size", lua_QFileInfoInstanceMethodSize},
  {"setFile", lua_QFileInfoInstanceMethodSetFile},
  {"setCaching", lua_QFileInfoInstanceMethodSetCaching},
  {"refresh", lua_QFileInfoInstanceMethodRefresh},
  {"path", lua_QFileInfoInstanceMethodPath},
  {"ownerId", lua_QFileInfoInstanceMethodOwnerId},
  {"owner", lua_QFileInfoInstanceMethodOwner},
  {"makeAbsolute", lua_QFileInfoInstanceMethodMakeAbsolute},
  {"isWritable", lua_QFileInfoInstanceMethodIsWritable},
  {"isSymLink", lua_QFileInfoInstanceMethodIsSymLink},
  {"isRoot", lua_QFileInfoInstanceMethodIsRoot},
  {"isRelative", lua_QFileInfoInstanceMethodIsRelative},
  {"isReadable", lua_QFileInfoInstanceMethodIsReadable},
  {"isNativePath", lua_QFileInfoInstanceMethodIsNativePath},
  {"isHidden", lua_QFileInfoInstanceMethodIsHidden},
  {"isFile", lua_QFileInfoInstanceMethodIsFile},
  {"isExecutable", lua_QFileInfoInstanceMethodIsExecutable},
  {"isDir", lua_QFileInfoInstanceMethodIsDir},
  {"isBundle", lua_QFileInfoInstanceMethodIsBundle},
  {"isAbsolute", lua_QFileInfoInstanceMethodIsAbsolute},
  {"groupId", lua_QFileInfoInstanceMethodGroupId},
  {"group", lua_QFileInfoInstanceMethodGroup},
  {"filePath", lua_QFileInfoInstanceMethodFilePath},
  {"fileName", lua_QFileInfoInstanceMethodFileName},
  {"exists", lua_QFileInfoInstanceMethodExists},
  {"completeSuffix", lua_QFileInfoInstanceMethodCompleteSuffix},
  {"completeBaseName", lua_QFileInfoInstanceMethodCompleteBaseName},
  {"canonicalPath", lua_QFileInfoInstanceMethodCanonicalPath},
  {"canonicalFilePath", lua_QFileInfoInstanceMethodCanonicalFilePath},
  {"caching", lua_QFileInfoInstanceMethodCaching},
  {"bundleName", lua_QFileInfoInstanceMethodBundleName},
  {"baseName", lua_QFileInfoInstanceMethodBaseName},
  {"__gc", lua_QFileInfoInstanceMethodDestruct},
  {"absolutePath", lua_QFileInfoInstanceMethodAbsolutePath},
  {"absoluteFilePath", lua_QFileInfoInstanceMethodAbsoluteFilePath},
  {"class", lua_QFileInfoInstanceMethodClass},
  {NULL, NULL}
};

/**
 * Register
 */

void static
registerQFileInfoInstanceMethods( lua_State *L ) {
  luaL_newmetatable(L, "QFileInfo.metatable");
  luaL_register(L, NULL, QFileInfoInstanceMethods);
  lua_pushvalue(L, -1);
  lua_setfield(L, -1, "__index");
}

extern "C" {
  int
  luaopen_QFileInfo( lua_State *L ) {
    registerQFileInfoClassMethods(L);
    registerQFileInfoInstanceMethods(L);
    return 1;
  }
}
