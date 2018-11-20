// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

using HttpClient = charon::net::HttpClient;

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

void os::sleep(double msecs)
{
  int value = int(msecs * 1000);
  std::this_thread::sleep_for(std::chrono::milliseconds(value));
}

unsigned int os::cores()
{
  return std::thread::hardware_concurrency();
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

double os::microtime()
{
  uint64_t value = std::chrono::high_resolution_clock::now().time_since_epoch() / std::chrono::microseconds(1);
  return (double) value / 1.0e6;
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
  std::string buffer;

  if (string::startsWith(path, "http://")) {
    HttpClient client(path);
    char * perform = client.performGet();
    buffer.append(perform);
    delete[] perform;
  } else {
    std::string line;
    std::ifstream file(path);
    if (file.is_open()) {
      while ( getline (file, line) ) {
        if( buffer.size() > 0 ) {
          buffer.push_back('\n');
        }
        buffer.append(line);
      }
      file.close();
    }
  }

  if( size ) {
    *size = buffer.size();
  }

  result = new char[buffer.size() + 1];
  memcpy( result, buffer.c_str(), buffer.size() );
  return result;
}
