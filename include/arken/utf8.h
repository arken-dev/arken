// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_UTF8_
#define _ARKEN_UTF8_

namespace arken
{

class utf8 {
  public:
  static char * sub(const char * string, int i, int j=-1);
  static char * mid(const char * string, int i, int j=-1);
  static char * upper(const char * string);
  static char * lower(const char * string);
  static char * encode(const char * string);
  static char * decode(const char * string);
  static char * sanitize(const char * string);
  static char * asc(const char * string, const char chr = 0);
  static int    len(const char * string);

};

} // namespace arken

#endif
