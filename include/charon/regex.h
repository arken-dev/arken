// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/ByteArrayList>

using charon::ByteArrayList;

namespace charon
{

class regex {
  public:
  static bool ematch(const char * string, const char * regex);
  static int index(const char * string, const char * regex);
  static int index(const char * string, const char * regex, int offset);
  static ByteArrayList * split(const char * string, const char * regex);
  static bool match(const char * string, const char * regex);
  static char * replace(const char * string, const char * regex, const char * after);
  static ByteArrayList * scan(const char * string, const char * regex);
};

}
