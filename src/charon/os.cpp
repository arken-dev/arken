// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <thread>

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QHostInfo>
#include <QRegExp>
#include <QThread>
#include <QUuid>

#include <charon/base>
#include <iostream>

extern "C" {
#include <charon/os/microtime.h>
}

char * os::abspath(const char * path)
{
  QFileInfo info(path);
  QString abspath = info.absoluteFilePath();
  char * result = new char[abspath.size() + 1];
  strcpy(result, abspath.toLocal8Bit());
  result[abspath.size()] = '\0';
  return result;
}

char * os::basename(const char * path)
{
  QFileInfo info(path);
  QString basename = info.fileName();
  char * result = new char[basename.size() + 1];
  strcpy(result, basename.toLocal8Bit());
  result[basename.size()] = '\0';
  return result;
}

uint os::atime(const char * path)
{
  return QFileInfo(path).lastRead().toTime_t();
}

bool os::compare(const char * path1, const char * path2)
{
  size_t size1;
  size_t size2;

  const char * buffer1;
  const char * buffer2;

  buffer1 = os::read(path1, &size1);
  buffer2 = os::read(path2, &size2);

  if( size1 != size2 ) {
    return false;
  }

  if( memcmp(buffer1, buffer2, size1) == 0 ) {
    return true;
  } else {
    return false;
  }
}

bool os::copy(const char * source, const char * destination, bool force = false)
{

  QFileInfo fileSource(source);
  if ( fileSource.isFile() ) {
    if( force && QFile::exists(destination) ) {
      QFile::remove(destination);
    }
    return QFile::copy(source, destination);
  }

  if( fileSource.isDir() ) {
    QDir dir(source);
    QString src(source);
    QString dest(destination);
    QStringList dirList = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for( int i = 0; i < dirList.size(); i++) {
        QString d = dirList.at(i);
        QString dst_path = dest + QDir::separator() + d;
        dir.mkpath(dst_path);
        if( os::copy((src + QDir::separator() + d).toLocal8Bit(), dst_path.toLocal8Bit()) == false ) {
          return false;
        }
    }

    QStringList fileList = dir.entryList(QDir::Files);
    for(int i=0; i < fileList.size(); i ++) {
        QString f = fileList.at(i);
        if( QFile::copy(src + QDir::separator() + f, dest + QDir::separator() + f) == false ) {
          return false;
        }
    }
  }

  return true;
}

unsigned int os::cores()
{
  return std::thread::hardware_concurrency();
}

bool os::chdir(const char * dirpath)
{
  return QDir::setCurrent( dirpath );
}

uint os::ctime(const char * path)
{
  return QFileInfo(path).lastModified().toTime_t();
}

char * os::dirpath(const char * path)
{
  QFileInfo info(path);
  QString dirpath = info.absolutePath();
  char * result = new char[dirpath.size() + 1];
  strcpy(result, dirpath.toLocal8Bit());
  result[dirpath.size()] = '\0';
  return result;
}

bool os::exists(const char * path)
{
  return QFile::exists(path);
}

ByteArrayList * os::glob(const char * dir)
{
  return os::glob(dir, false);
}

ByteArrayList * os::glob(const char * dir, bool sub)
{
  ByteArrayList * list = new ByteArrayList();
  QDirIterator::IteratorFlags flags;

  if( sub ) {
    flags = QDirIterator::Subdirectories;
  }

  QDirIterator iterator(QString(dir), flags);

  while( iterator.hasNext() ) {
    iterator.next();
    list->append( iterator.fileInfo().filePath().toLocal8Bit().data() );
  }

  return list;
}

ByteArrayList * os::glob(const char * dir, const char * regex)
{
  return os::glob(dir, regex, false);
}

ByteArrayList * os::glob(const char * dir, const char * regex, bool sub)
{

  QRegExp qregex(regex);
  ByteArrayList * list = new ByteArrayList();
  QDirIterator::IteratorFlags flags;

  if( sub ) {
    flags = QDirIterator::Subdirectories;
  }

  QDirIterator iterator(QString(dir), flags);

  while( iterator.hasNext() ) {
    iterator.next();
    if( qregex.indexIn(iterator.fileInfo().filePath(), 0) != -1 ) {
      list->append( iterator.fileInfo().filePath().toLocal8Bit().data() );
    }
  }

  return list;
}

char * os::home()
{
  QString homePath = QDir::homePath();
  char * result = new char[homePath.size()+1];
  strcpy(result, homePath.toLocal8Bit());
  result[homePath.size()] = '\0';
  return result;
}

char * os::hostname()
{
  QString hostname = QHostInfo::localHostName();
  char * result = new char[hostname.size() + 1];
  strcpy(result, hostname.toLocal8Bit());
  result[hostname.size()] = '\0';
  return result;
}

bool os::isdir(const char * path)
{
  QFileInfo info(path);
  return info.isDir();
}

bool os::isfile(const char * path)
{
  QFileInfo info(path);
  return info.isFile();
}

bool os::islink(const char * path)
{
  QFileInfo info(path);
  return info.isSymLink();
}

bool os::link(const char * source, const char * destination, bool force = false)
{
  if( force && QFile::exists(destination) ) {
    QFile::remove(destination);
  }
  return QFile::link(source, destination);
}

double os::microtime()
{
  //return QDateTime::currentMSecsSinceEpoch() / 1000.0;
  return charon_os_microtime();
}

bool os::mkdir(const char * dirname)
{
  QDir dir;
  return dir.mkdir(dirname);
}

bool os::mkpath(const char * dirpath)
{
  QDir dir;
  return dir.mkpath(dirpath);
}

char * os::name()
{
  QString name;
  #if defined(Q_OS_ANDROID)
  name = QString("android");
  #elif defined(Q_OS_BLACKBERRY)
  name = QString("blackberry");
  #elif defined(Q_OS_IOS)
  name = QString("ios");
  #elif defined(Q_OS_MAC)
  name = QString("osx");
  #elif defined(Q_OS_WINCE)
  name = QString("wince");
  #elif defined(Q_OS_WIN)
  name = QString("windows");
  #elif defined(Q_OS_LINUX)
  name = QString("linux");
  #elif defined(Q_OS_UNIX)
  name = QString("unix");
  #else
  name = QString("unknown");
  #endif

  char * result = new char[name.size() + 1];
  strcpy(result, name.toLocal8Bit());
  result[name.size()] = '\0';

  return result;
}

qint64 os::pid()
{
  return QCoreApplication::applicationPid();
}

char * os::pwd()
{
  QString pwd = QDir::currentPath();
  char * result = new char[pwd.size() + 1];
  strcpy(result, pwd.toLocal8Bit());
  result[pwd.size()] = '\0';
  return result;
}

bool os::rmdir(const char * dirname)
{
  QDir dir;
  return dir.rmdir(dirname);
}

bool os::rmpath(const char * dirpath)
{
  QDir dir;
  return dir.rmpath(dirpath);
}

void os::sleep(double msecs)
{
  int value = int(msecs * 1000);
  std::this_thread::sleep_for(std::chrono::milliseconds(value));
}

char * os::target(const char * path)
{
  QString target = QFile::symLinkTarget(path);
  char * result  = new char[target.size() + 1];
  strcpy(result, target.toLocal8Bit());
  result[target.size()] = '\0';
  return result;
}

char * os::temp()
{
  QString temp = QDir::tempPath();
  char * result = new char[temp.size() + 1];
  strcpy(result, temp.toLocal8Bit());
  result[temp.size()] = '\0';
  return result;
}

bool os::touch(const char * path)
{
  bool flag;
  std::fstream fs;

  flag = false;
  fs.open(path, std::ios::out);
  if( fs.is_open() ) {
    flag = true;
  }

  fs.close();
  return flag;
}

char * os::read(const char * path)
{
  return os::read(path, 0);
}

char * os::read(const char * path, size_t * size)
{
  char * result;
  if (string::startsWith(path, "http://")) {
    result = http::read(path);
    *size  = strlen(result);
  } else {
    QFile file(path);
    QByteArray raw;

    file.open(QIODevice::ReadOnly);
    raw = file.readAll();
    if( size != 0 ) {
      *size = raw.size();
    }
    result = new char[raw.size() + 1];
    memcpy( result, raw.data(), raw.size() );
    result[raw.size()] = '\0';
  }

  return result;
}

char * os::root()
{
  QString root = QDir::rootPath();
  char * result = new char[root.size() + 1];
  strcpy(result, root.toLocal8Bit());
  result[root.size()] = '\0';
  return result;
}
