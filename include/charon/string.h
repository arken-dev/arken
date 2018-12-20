// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_STRING_
#define _CHARON_STRING_

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <charon/ByteArrayList>
#include <iostream>

namespace charon {
  class string {

    private:
    char   * m_data;
    size_t   m_size;
    size_t   m_capacity;

    //-------------------------------------------------------------------------
    // CLASS METHODS
    //-------------------------------------------------------------------------

    public:
    static char * append(const char * string, const char * str);
    static char * camelCase(const char * string, bool lcfirst = false);
    static char * capitalize(const char * string);
    static char * center(const char * string, size_t size, const char * pad);
    static bool   contains(const char * string, const char * str);
    static char * chop(const char * string, int n);
    static int    count(const char * str1, const char * str2);
    static char * dasherize(const char *string);
    static char * escape(const char * string);
    static char * escapeHtml(const char * string);
    static int    indexOf(const char * string, const char * str, int i = 0);
    static char * insert(const char * string, int len, const char * ba);
    static bool   endsWith(const char * string, const char * ba);
    static int    lastIndexOf(const char * string, const char * str);
    static char * left(const char *string, int len);
    static char * leftJustified(const char * string, size_t size, const char * pad);
    static char * mid(const char * string, int pos, int len);
    static char * md5(const char * string);
    static char * normalize(const char * string);
    static char * simplified(const char *buffer);
    static char * repeated(const char *buffer, int times);
    static char * replace(const char * string, const char * before, const char * after, int start = 0);
    static char * replace(const char * string, const char before, const char after, int start = 0);
    static char * right(const char * buffer, int len);
    static char * rightJustified(const char * string, size_t size, const char * pad);
    static char * sha1(const char * string);
    static ByteArrayList * split(const char * string, const char * pattern);
    static ByteArrayList * split(const char * string, size_t len, const char * pattern);
    static char * suffix(const char * raw);
    static char * suffix(const char * raw, const char chr);
    static bool   startsWith(const char * string, const char *str);
    static char * trimmed(const char * string);
    static char * leftTrimmed(const char * string);
    static char * rightTrimmed(const char * string);
    static char * truncate(const char * string, int pos, const char *omission = "...", const char separator = ' ');
    static char * underscore(const char * string);

    //-------------------------------------------------------------------------
    // INSTANCE METHODS
    //-------------------------------------------------------------------------

    static string * consume(char * str);
    string();
    string(char * str);
    string(const char * str);
    ~string();

    void     append(const char * str);
    string * camelCase(bool lcfirst = false);
    string * capitalize();
    string * center(size_t size, const char * pad);
    bool     contains(const char * str);
    string * chop(int n);
    int      count(const char * str2);
    string * dasherize();
    string * escape();
    string * escapeHtml();
    int      indexOf(const char * str, int i = 0);
    string * insert(int len, const char * ba);
    bool     endsWith(const char * ba);
    int      lastIndexOf(const char * str);
    string * left(int len);
    string * leftJustified(size_t size, const char * pad);
    string * mid(int pos, int len);
    string * md5();
    string * normalize();
    string * simplified();
    string * repeated(int times);
    string * replace(const char * before, const char * after, int start = 0);
    string * replace(const char before, const char after, int start = 0);
    string * right(int len);
    string * rightJustified(size_t size, const char * pad);
    string * sha1();
    size_t   size();
    ByteArrayList * split(const char * pattern);
    string * suffix(const char chr = '.');
    bool     startsWith(const char *str);
    string * trimmed();
    string * leftTrimmed();
    string * rightTrimmed();
    string * truncate(int pos, const char * omission = "...", const char separator = ' ');
    string * underscore();
    char   * data() const;
  };
}

std::ostream & operator<<(std::ostream & os, const charon::string & str);
std::ostream & operator<<(std::ostream & os, const charon::string * str);

#endif
