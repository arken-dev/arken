// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <charon/ByteArrayList>

namespace charon {
  namespace base {
    class string {
      public:
      static char * append(const char * string, const char * str);
      static char * camelcase(const char * string, bool lcfirst);
      static char * camelcase(const char * string);
      static char * capitalize(const char * string);
      static bool   contains(const char * string, const char * str);
      static char * escape(const char * string);
      static char * escapeHtml(const char * string);
      static int    indexOf(const char * string, const char * str);
      static char * insert(const char * string, int len, const char * ba);
      static bool   endsWith(const char * string, const char * ba);
      static int    lastIndexOf(const char * string, const char * str);
      static char * left(const char *string, int len);
      static char * mid(const char * string, int pos, int len);
      static char * normalize(const char * string);
      static char * repeated(const char *buffer, int times);
      static char * replace(const char * string, const char * before, const char * after);
      static char * right(const char *buffer, int len);
      static char * simplified(const char *buffer);
      static ByteArrayList * split(const char * string, const char * pattern);
      static ByteArrayList * split(const char * string, size_t len, const char * pattern);
      static char * suffix(const char * raw);
      static char * suffix(const char * raw, const char chr);
      static bool   startsWith(const char *string, const char *str);
      static char * trimmed(const char *string);
      static char * truncate(const char *string, int pos);
      static char * underscore(const char *string);
    };
  }
}
