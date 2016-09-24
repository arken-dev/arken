#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHostInfo>
#include <QThread>
#include <QUuid>

#include <oberon/helper>
#include <oberon/modules/http.h>
#include <iostream>

char * os::abspath(const char * path)
{
  QFileInfo info(path);
  return info.absoluteFilePath().toLocal8Bit().data();
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
  if( force && QFile::exists(destination) ) {
    QFile::remove(destination);
  }
  return QFile::copy(source, destination);
}

int os::cores()
{
  return QThread::idealThreadCount();
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
  return info.absolutePath().toLocal8Bit().data();
}

bool os::exists(const char * path)
{
  return QFile::exists(path);
}

char * os::home()
{
  return QDir::homePath().toLocal8Bit().data();
}

char * os::hostname()
{
  QString result = QHostInfo::localHostName();
  return result.toLocal8Bit().data();
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
  return QDateTime::currentMSecsSinceEpoch() / 1000.0;
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
  QString result;
  #if defined(Q_OS_ANDROID)
  result = QLatin1String("android");
  #elif defined(Q_OS_BLACKBERRY)
  result = QLatin1String("blackberry");
  #elif defined(Q_OS_IOS)
  result = QLatin1String("ios");
  #elif defined(Q_OS_MAC)
  result = QLatin1String("osx");
  #elif defined(Q_OS_WINCE)
  result = QLatin1String("wince");
  #elif defined(Q_OS_WIN)
  result = QLatin1String("windows");
  #elif defined(Q_OS_LINUX)
  result = QLatin1String("linux");
  #elif defined(Q_OS_UNIX)
  result = QLatin1String("unix");
  #else
  result = QLatin1String("unknown");
  #endif
  return result.toLocal8Bit().data();
}

qint64 os::pid()
{
  return QCoreApplication::applicationPid();
}

char * os::pwd()
{
  return QDir::currentPath().toLocal8Bit().data();
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

void os::sleep(double secs)
{
  QThread::msleep(secs*1000);
}

char * os::target(const char * path)
{
  return QFile::symLinkTarget(path).toLocal8Bit().data();
}

char * os::temp()
{
  return QDir::tempPath().toLocal8Bit().data();
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

char * os::uuid()
{
  QUuid uuid = QUuid::createUuid();
  char * result = new char[37];
  strcpy(result, uuid.toByteArray().mid(1, 36).data());
  result[37] = '\0';
  return result;
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
  return QDir::rootPath().toLocal8Bit().data();
}


