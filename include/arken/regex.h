// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_REGEX_
#define _ARKEN_REGEX_

#include <arken/base>

namespace arken
{


class regex {
  using List = arken::string::List;

  public:
  static bool ematch(const char * string, const char * regex);
  static int index(const char * string, const char * regex);
  static int index(const char * string, const char * regex, int offset);
  static List split(const char * string, const char * regex);
  static bool match(const char * string, const char * regex);
  static char * replace(const char * string, const char * regex, const char * after);
  static List scan(const char * string, const char * regex);
};

} // namespace arken

#endif
