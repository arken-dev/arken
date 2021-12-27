// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_OS_
#define _ARKEN_OS_

#include <arken/string.h>


namespace arken {


class os {

  using List   = arken::string::List;
  using string = arken::string;

  public:
  static string   abspath(const char * path);
  static size_t   atime(const char * path);
  static string   basename(const char * path);
  static bool     compare(const char * path1, const char * path2);
  static bool     copy(const char * source, const char * destination, bool force);
  static unsigned int cores();
  static bool     chdir(const char * dirpath);
  static size_t   ctime(const char * path);
  static string   dirpath(const char * path);
  static bool     exists(const char * path);
  static string   executablePath();
  static List glob(const char * dir);
  static List find(const char * dir, const char * regex = ".*", bool sub = false);
  static string   home();
  static string   hostname();
  static bool     isdir(const char* path);
  static bool     isfile(const char * path);
  static bool     islink(const char * path);
  static bool     link(const char * source, const char * destination, bool force);
  static size_t   mem();
  static double   microtime();
  static bool     mkdir(const char * dirname);
  static bool     mkpath(const char * dirpath);
  static const char * name();
  static const char * cext();
  static uint64_t pid();
  static string   pwd();
  static bool     rmdir(const char * dirname);
  static bool     rmpath(const char * dirpath);
  static void     sleep(double secs);
  static string   target(const char * path);
  static string   tmp();
  static bool     touch(const char * path);
  static string   uuid();
  static string   read(const char * path);
  static string   root();
};

} // namespace arken

#endif
