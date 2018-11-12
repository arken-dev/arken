// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_UTF8_
#define _CHARON_UTF8_

namespace charon
{

class utf8 {
  public:
  static char * upper(const char * string);
  static char * lower(const char * string);
  static char * format(const char * string);
  static int len(const char * string);
};

}

#endif
