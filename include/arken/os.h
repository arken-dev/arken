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

  public:
  static char   * abspath(const char * path);
  static uint     atime(const char * path);
  static char   * basename(const char * path);
  static bool     compare(const char * path1, const char * path2);
  static bool     copy(const char * source, const char * destination, bool force);
  static unsigned int cores();
  static bool     chdir(const char * dirpath);
  static uint     ctime(const char * path);
  static char   * dirpath(const char * path);
  static bool     exists(const char * path);
  static char   * executablePath();
  static List glob(const char * dir);
  static List find(const char * dir, const char * regex = ".*", bool sub = false);
  static char   * home();
  static char   * hostname();
  static bool     isdir(const char* path);
  static bool     isfile(const char * path);
  static bool     islink(const char * path);
  static bool     link(const char * source, const char * destination, bool force);
  static double   microtime();
  static bool     mkdir(const char * dirname);
  static bool     mkpath(const char * dirpath);
  static const char * name();
  static uint64_t pid();
  static char   * pwd();
  static bool     rmdir(const char * dirname);
  static bool     rmpath(const char * dirpath);
  static void     sleep(double secs);
  static char   * target(const char * path);
  static char   * tmp();
  static bool     touch(const char * path);
  static char   * uuid();
  static string read(const char * path);
  static char   * root();
};

} // namespace arken

#endif
