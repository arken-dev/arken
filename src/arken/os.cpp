// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>

using HttpClient = arken::net::HttpClient;
using string = arken::string;

bool os::compare(const char * path1, const char * path2)
{
  string buffer1;
  string buffer2;

  buffer1 = os::read(path1);
  buffer2 = os::read(path2);

  if( buffer1.size() != buffer2.size() ) {
    return false;
  }

  if( memcmp(buffer1.data(), buffer2.data(), buffer1.size()) == 0 ) {
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

List * os::glob(const char * dir)
{
  return os::glob(dir, false);
}

List * os::glob(const char * dir, bool sub)
{
  List * list = new List();
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

List * os::glob(const char * dir, const char * regex)
{
  return os::glob(dir, regex, false);
}

List * os::glob(const char * dir, const char * regex, bool sub)
{

  QRegExp qregex(regex);
  List * list = new List();
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

string os::read(const char * path)
{

  if (string::startsWith(path, "http://") || string::startsWith(path, "https://") ) {
    HttpClient client(path);
    return client.performGet();
  } else {
    char * buffer;
    std::ifstream file;
    int length;
    file.open(path);
    file.seekg(0, std::ios::end);
    length = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer = new char[length+1];
    file.read(buffer, length);
    file.close();
    buffer[length] = '\0';
    return string::consume( buffer, length );
  }

}
