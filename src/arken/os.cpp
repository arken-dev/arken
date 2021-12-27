// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/string.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <fstream>
#include <arken/net/HttpClient>
#include <apple/glob.h>
#include <regex>
#include <filesystem>
#include <unistd.h>
#include <limits.h>

using path   = std::filesystem::path;
using string = arken::string;
using List   = arken::string::List;

static
std::time_t to_time_t(std::filesystem::file_time_type tp)
{
  auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>
    (tp - std::filesystem::file_time_type::clock::now()
      + std::chrono::system_clock::now());
  return std::chrono::system_clock::to_time_t(sctp);
}


namespace arken {

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
  auto value = int(msecs * 1000);
  std::this_thread::sleep_for(std::chrono::milliseconds(value));
}

unsigned int os::cores()
{
  return std::thread::hardware_concurrency();
}

List os::glob(const char * pattern)
{

  List list;
  glob_t paths;
  int retval;

  paths.gl_pathc = 0;
  paths.gl_offs  = 0;
  paths.gl_pathv = NULL;

  retval = ::glob( pattern, GLOB_NOCHECK | GLOB_BRACE, NULL, &paths );
  if( retval == 0 ) {

    for( int idx = 0; idx < paths.gl_pathc; idx++ ) {
      list.append( paths.gl_pathv[idx] );
    }

    globfree( &paths );
  } else {
    puts( "glob() failed" );
  }

  return list;
}

List os::find(const char * dir, const char * rgx, bool recursive)
{
  namespace fs = std::filesystem;

  List list;
  std::regex exp(rgx);

  if( ! os::exists(dir) ) {
    return list;
  }

  if( recursive ) {
    for(auto& p: fs::recursive_directory_iterator(dir)) {
       std::smatch matches;
       std::string path(p.path().string());
       if( std::regex_search(path, matches, exp) ) {
         list.append( std::string(path).c_str() );
       }
    }
  } else {
    for(auto& p: fs::directory_iterator(dir)) {
       std::smatch matches;
       std::string path(p.path().string());
       if( std::regex_search(path, matches, exp) ) {
         list.append( std::string(path).c_str() );
       }
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
  using HttpClient = arken::net::HttpClient;

  if (string::startsWith(path, "http://") || string::startsWith(path, "https://") ) {
    HttpClient client(path);
    return client.performGet();
  } else {
    char * buffer;
    std::ifstream file;
    size_t length;
    file.open(path);
    file.seekg(0, std::ios::end);
    length = file.tellg();
    file.seekg(0, std::ios::beg);
    buffer = new char[length+1];
    file.read(buffer, length);
    file.close();
    buffer[length] = '\0';
    return string( std::move(buffer), length );
  }

}

string os::abspath(const char * p)
{
  path path = std::filesystem::absolute(p);
  return string(path.string());
}

string os::basename(const char * p)
{
  path path = std::filesystem::absolute(p).filename();
  return string(path.string());
}

size_t os::atime(const char * p)
{
  auto tp = std::filesystem::last_write_time(p);
  return to_time_t(tp);
}

bool os::copy(const char * source, const char * destination, bool force = false)
{

  if( os::isdir(source) ) {
    std::filesystem::copy(source, destination,
      std::filesystem::copy_options::overwrite_existing |
      std::filesystem::copy_options::recursive);
  } else {
    std::filesystem::copy(source, destination);
  }

  return true;
}

bool os::chdir(const char * dirpath)
{
  std::filesystem::current_path(dirpath);
  path p = std::filesystem::current_path();
  return p.string().compare(os::pwd());
}

size_t os::ctime(const char * p)
{
  auto t = std::filesystem::last_write_time(p);
  return to_time_t(t);
}

string os::dirpath(const char * p)
{
  path path = std::filesystem::absolute(p).parent_path();
  return string(path.string());
}

bool os::exists(const char * path)
{
  return std::filesystem::exists(path);
}

string os::home()
{
  const char * p = getenv("HOME");
  if( p == nullptr ) {
    return {};
  } else {
    path path = std::filesystem::absolute(p);
    return string(path.string());
  }
}

string os::hostname()
{
  #define ARKEN_HOSTNAME_MAX 64
  auto hostname = new char[ARKEN_HOSTNAME_MAX+1]();
  gethostname(hostname, ARKEN_HOSTNAME_MAX+1);
  return string(std::move(hostname));
}

bool os::isdir(const char * path)
{
  return std::filesystem::is_directory(path);
}

bool os::isfile(const char * path)
{
  return std::filesystem::is_regular_file(path);
}

bool os::islink(const char * path)
{
  return std::filesystem::is_symlink(path);
}

bool os::link(const char * source, const char * destination, bool force = false)
{
  if( force and os::exists(destination) ) {
    std::filesystem::remove(destination);
  }
  std::filesystem::create_symlink(source, destination);

  return os::exists(destination);
}

bool os::mkdir(const char * dirname)
{
  return std::filesystem::create_directory(dirname);
}

bool os::mkpath(const char * dirpath)
{
  return std::filesystem::create_directories(dirpath);
}

string os::pwd()
{
  path cwd = std::filesystem::current_path();
  return string(cwd.string());
}

bool os::rmdir(const char * dirpath)
{
  if (std::filesystem::is_directory(dirpath) && std::filesystem::is_empty(dirpath)) {
    uintmax_t result = std::filesystem::remove_all(dirpath);
    return result > 0;
  }

  return false;
}

bool os::rmpath(const char * dirpath)
{
  uintmax_t result = std::filesystem::remove_all(dirpath);
  return result > 0;
}

string os::target(const char * p)
{
  path target = std::filesystem::read_symlink(p);
  return string(target.string());
}

string os::tmp()
{
  path tmp = std::filesystem::temp_directory_path();
  return string(tmp.string());
}

string os::root()
{
  path root = std::filesystem::current_path().root_directory();
  return string(root.string());
}

} // namespace arken
