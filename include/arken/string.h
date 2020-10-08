// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_STRING_
#define _ARKEN_STRING_

#include <cstdlib>
#include <cstring>
#include <cctype>
#include <iostream>

namespace arken {
  class string {


    private:
    char   * m_data;
    size_t   m_size;
    size_t   m_capacity;
    size_t   m_reserve;

    //-------------------------------------------------------------------------
    // CLASS METHODS
    //-------------------------------------------------------------------------

    public:

    class List;

    static char * append(const char * string, const char * str);
    static char * camelCase(const char * string, bool lcfirst = false);
    static char * capitalize(const char * string);
    static char * center(const char * string, size_t size, const char * pad);
    static bool   contains(const char * string, const char * str);
    static char * chop(const char * string, int n);
    static int    count(const char * str1, const char * str2);
    static char * dasherize(const char *string);
    static char * encode64(const char *string);
    static char * decode64(const char *string);
    static char * encode(const char *string, const char *charset);
    static char * decode(const char *string, const char *charset);
    static bool   equals(const char * str1, const char * str2);
    static char * escape(const char * string);
    static char * escapeHtml(const char * string);
    static int    indexOf(const char * string, const char * str, int i = 0);
    static char * insert(const char * string, int len, const char * ba);
    static bool   endsWith(const char * string, const char * ba);
    static int    lastIndexOf(const char * string, const char * str);
    static char * left(const char *string, int len);
    static char * leftJustified(const char * string, size_t size, const char * pad);
    static char * mid(const char * string, int pos, int len = -1, int string_len = 0);
    static char * md5(const char * string);
    static char * md5(const char * string, size_t len);
    static char * normalize(const char * string);
    static char * prefix(const char * raw, const char chr = '.');
    static char * simplified(const char *buffer);
    static char * repeated(const char *buffer, int times);
    static char * replace(const char * string, const char * before, const char * after, int start = 0);
    static char * replace(const char * string, const char before, const char after, int start = 0);
    static char * right(const char * buffer, int len);
    static char * rightJustified(const char * string, size_t size, const char * pad);
    static char * sha1(const char * string);
    static List split(const char * string, const char * pattern);
    static List split(const char * string, size_t len, const char * pattern);
    static char * suffix(const char * raw, const char chr = '.');
    static bool   startsWith(const char * string, const char *str);
    static char * trimmed(const char * string);
    static char * leftTrimmed(const char * string);
    static char * rightTrimmed(const char * string);
    static char * truncate(const char * string, int pos, const char *omission = "...", const char separator = ' ');
    static char * underscore(const char * string);

    //-------------------------------------------------------------------------
    // INSTANCE METHODS
    //-------------------------------------------------------------------------

    static string   consume(char * str);
    static string   consume(char * str, size_t size);
    static string * consume(arken::string str);
    string();
    string(const char * str);
    string(const char * str, size_t size);
    string(size_t reserve);
    string(const arken::string &str);
    ~string();

    string & append(const char * str);
    string & append(const char * str, size_t len);
    string & append(std::string str);
    string camelCase(bool lcfirst = false);
    string capitalize();
    string center(size_t size, const char * pad);
    bool   contains(const char * str);
    string chop(int n);
    void   clear();
    int    count(const char * str2);
    string dasherize();
    string encode64();
    string decode64();
    string encode(const char * charset);
    string decode(const char * charset);
    string escape();
    string escapeHtml();
    bool   empty();
    bool   equals(const char * str);
    int    indexOf(const char * str, int i = 0);
    string insert(int len, const char * ba);
    bool   endsWith(const char * ba);
    int    lastIndexOf(const char * str);
    string left(int len);
    string leftJustified(size_t size, const char * pad);
    size_t len();
    string mid(int pos, int len = -1);
    string md5();
    string normalize();
    string prefix(const char chr = '.');
    string & prepend(const char * str);
    string simplified();
    char * release();
    string repeated(int times);
    string replace(const char * before, const char * after, int start = 0);
    string replace(const char before, const char after, int start = 0);
    void   reserve(size_t reserve);
    size_t reserve();
    string right(int len);
    string rightJustified(size_t size, const char * pad);
    string sha1();
    size_t size();
    List split(const char * pattern);
    string suffix(const char chr = '.');
    bool   startsWith(const char *str);
    string trimmed();
    string leftTrimmed();
    string rightTrimmed();
    string truncate(int pos, const char * omission = "...", const char separator = ' ');
    string underscore();
    char  * data() const;
    // explicit conversion
    operator const char *() const { return m_data; }
    string & operator=(const string &str);
    string & operator=(const string *str);
    string & operator=(const char   *str);

    class List
    {
      private:
      void init();
      int m_size;
      int m_resource;
      int m_cursor;
      string ** m_array;

      public:
      List();
      List(int resource);
      List(const List &obj);
      static List * consume(List &obj);
      ~List();
      void replace(int pos, const char * value);
      List &append(const char * value);
      List &append(const char * value, int len);
      List &operator << (const char * value);
      const char *operator[](int pos);
      const char *at(int pos);
      const char *at(int pos, int * len);
      const char *first();
      const char *last();
      const char *each();
      char * join(const char * separator);
      int size();
    };

  };

}

std::ostream & operator<<(std::ostream & os, const arken::string & str);
std::ostream & operator<<(std::ostream & os, const arken::string * str);

#endif
