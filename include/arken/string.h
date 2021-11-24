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

  //---------------------------------------------------------------------------
  // CLASS METHODS
  //---------------------------------------------------------------------------

  public:

  class List;

  static char * append(const char * string, const char * str);
  static bool   blank(const char * string);
  static char * camelCase(const char * string, bool lcfirst = false);
  static char * capitalize(const char * string);
  static char * center(const char * string, size_t size, const char * pad);
  static char * chop(const char * string);
  static bool   contains(const char * string, const char * str);
  static int    count(const char * str1, const char * str2);
  static char * dasherize(const char *string);
  static char * decode64(const char *string, size_t * size);
  static bool   empty(const char *string);
  static char * encode64(const char *string, size_t size);
  static bool   endsWith(const char * string, const char * ba);
  static bool   equals(const char * str1, const char * str2);
  static char * escape(const char * string);
  static char * escapeHtml(const char * string);
  static size_t hash(const char * string, size_t size);
  static int    indexOf(const char * string, const char * str, int i = 0);
  static char * insert(const char * string, int len, const char * ba);
  static int    lastIndexOf(const char * string, const char * str);
  static char * left(const char *string, int len);
  // len
  // lower
  static char * md5(const char * string, size_t len);
  static char * mid(const char * string, int pos, int len = -1, int string_len = 0, size_t * _len = nullptr);
  static char * normalize(const char * string);
  static char * padLeft(const char * string, size_t size, const char * pad);
  static char * padRight(const char * string, size_t size, const char * pad);
  static char * prefix(const char * raw, const char * chr);
  // prepend ???
  // static char * rep(const char *buffer, int times);
  static char * remove(const char *str, const char *rep, int start = 0);
  static char * removeChar(const char * string, const char rep, int start = 0);
  static char * repeated(const char *buffer, int times);
  static char * replace(const char * string, const char * before, const char * after, int start = 0);
  static char * replaceChar(const char * string, const char before, const char after, int start = 0);
  // reverse
  static char * right(const char * buffer, int len);
  static char * sha1(const char * string, size_t len);
  static char * squish(const char *buffer);
  static List split(const char * string, const char * pattern);
  static List split(const char * string, size_t len, const char * pattern);
  static bool   startsWith(const char * string, const char *str);
  // sub
  static char * suffix(const char * raw, const char * chr);
  static char * trim(const char * string);
  static char * trimLeft(const char * string);
  static char * trimRight(const char * string);
  static char * truncate(const char * string, int pos, const char *omission = "...", const char separator = ' ');
  static char * underscore(const char * string);
  // upper

  //---------------------------------------------------------------------------
  // INSTANCE METHODS
  //---------------------------------------------------------------------------

  string();
  string(const char * str);
  string(const char * str, size_t size);
  string(string && str);
  string(char * && str);
  string(char * && str, size_t size);
  string(const arken::string &str);
  string(const std::string &str);
  ~string();

  string & append(const char * str);
  string & append(const char * str, size_t len);
  string & append(std::string str);
  bool   blank();
  string camelCase(bool lcfirst = false);
  string capitalize();
  string center(size_t size, const char * pad);
  string chop();
  bool   contains(const char * str);
  int    count(const char * str2);
  void   clear();
  string dasherize();
  const char * data() const;
  string decode(const char * charset);
  string decode64();
  bool   empty();
  string encode(const char * charset);
  string encode64();
  bool   endsWith(const char * ba);
  bool   equals(const char * str);
  string escape();
  string escapeHtml();
  size_t hash() const;
  int    indexOf(const char * str, int i = 0);
  string insert(int len, const char * ba);
  int    lastIndexOf(const char * str);
  string left(int len);
  size_t len();
  // lower
  string md5();
  string mid(int pos, int len = -1);
  string normalize();
  string padLeft(size_t size, const char * pad);
  string padRight(size_t size, const char * pad);
  string prefix(const char * chr);
  string & prepend(const char * str);
  char * release();
  // rep
  string remove(const char *rep, int start = 0);
  string removeChar(const char rep, int start = 0);
  string repeated(int times);
  string replace(const char * before, const char * after, int start = 0);
  string replaceChar(const char before, const char after, int start = 0);
  // reverse
  void   reserve(size_t reserve);
  size_t reserve();
  string right(int len);
  string sha1();
  string squish();
  size_t size();
  List split(const char * pattern);
  bool   startsWith(const char *str);
  // sub
  string suffix(const char * pattern);
  string trim();
  string trimLeft();
  string trimRight();

  string truncate(int pos, const char * omission = "...", const char separator = ' ');
  string underscore();
  //upper
  // explicit conversion
  operator const char *() const { return m_data; }
  string & operator=(const string &str);
  string & operator=(string &&str);
  string & operator=(const string *str);
  string & operator=(const char   *str);
  bool operator < (const string &str) const;
  bool operator == (const string &str) const;

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
    List(List && obj);
    static List * consume(List &obj);
    ~List();
    void replace(int pos, const char * value);
    List &append(const char * value);
    List &append(const char * value, int len);
    List &operator << (const char * value);
    List &operator=(const List &list);
    const char *operator[](int pos);
    const char *at(int pos);
    const char *at(int pos, int * len);
    const char *first();
    const char *last();
    const char *each();
    char * join(const char * separator);
    int size();
    int cursor();
  };

};

} // namespace arken

std::ostream & operator<<(std::ostream & os, const arken::string & str);
std::ostream & operator<<(std::ostream & os, const arken::string * str);

namespace std {
    template<>
    struct hash<arken::string> {
        size_t operator()(const arken::string & str) const {
          return str.hash();
        }
    };
} // namespace std

#endif
