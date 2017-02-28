// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <charon/ByteArrayList>

namespace charon {
  class string {
    public:
    static char * append(const char * string, const char * str);
    static char * camelcase(const char * string, bool lcfirst);
    static char * camelcase(const char * string);
    static char * capitalize(const char * string);
    static char * center(const char * string, size_t size, const char * pad);
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
    static char * padLeft(const char * string, size_t size, const char * pad);
    static char * padRight(const char * string, size_t size, const char * pad);
    static char * reduce(const char *buffer);
    static char * repeated(const char *buffer, int times);
    static char * replaceAll(const char * string, const char * before, const char * after);
    static char * replaceFirst(const char * string, const char * before, const char * after);
    static char * replaceChar(const char * string, const char before, const char after);
    static char * right(const char *buffer, int len);
    static ByteArrayList * split(const char * string, const char * pattern);
    static ByteArrayList * split(const char * string, size_t len, const char * pattern);
    static char * suffix(const char * raw);
    static char * suffix(const char * raw, const char chr);
    static bool   startsWith(const char *string, const char *str);
    static char * trim(const char *string);
    static char * trimLeft(const char *string);
    static char * trimRight(const char *string);
    static char * truncate(const char *string, int pos);
    static char * truncate(const char *string, int pos, const char *omission);
    static char * truncate(const char *string, int pos, const char *omission, const char separator);
    static char * underscore(const char *string);
  };
}