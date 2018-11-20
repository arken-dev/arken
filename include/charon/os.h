// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_OS_
#define _CHARON_OS_

#include <QtCore>
#include <charon/ByteArrayList>

using charon::ByteArrayList;

namespace charon {
  class os {
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
    static ByteArrayList * glob(const char * dir);
    static ByteArrayList * glob(const char * dir, bool sub);
    static ByteArrayList * glob(const char * dir, const char * regex);
    static ByteArrayList * glob(const char * dir, const char * regex, bool sub);
    static char   * home();
    static char   * hostname();
    static bool     isdir(const char * path);
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
    static char   * temp();
    static bool     touch(const char * path);
    static char   * uuid();
    static char   * read(const char * path);
    static char   * read(const char * path, size_t * size);
    static char   * root();
  };
}

#endif
