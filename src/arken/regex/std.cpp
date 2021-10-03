// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <cstring>
#include <regex>
#include <arken/regex.h>
#include <arken/string.h>

using List = arken::string::List;

namespace arken {

bool regex::ematch(const char * string, const char * regex)
{
  std::string str(string);
  std::regex exp(regex);
  return std::regex_match(str, exp);
}

int regex::index(const char * string, const char * regex)
{
  std::smatch matches;
  std::string str(string);
  std::regex exp(regex);
  std::regex_search(str, matches, exp);
  return matches.position();
}

int regex::index(const char * string, const char * regex, int offset)
{
  if( offset > (int) strlen(string) ) {
    return -1;
  }
  std::smatch matches;
  std::string str(&string[offset]);
  std::regex exp(regex);
  std::regex_search(str, matches, exp);
  return matches.position() + offset;
}

List * regex::split(const char * string, const char * regex)
{
  auto list = new List();

  std::string str(string);
  std::regex exp(regex);
  std::smatch matches;

  while (std::regex_search(str, matches, exp)) {
    list->append(matches.prefix().str().c_str());
    str = matches.suffix().str();
  }

  if( str.size() > 0 ) {
    list->append(str.c_str());
  }

  return list;
}

bool regex::match(const char * string, const char * regex)
{
  std::smatch matches;
  std::string str(string);
  std::regex exp(regex);
  return std::regex_search(str, matches, exp);
}

char * regex::replace(const char * string, const char * regex, const char * after)
{
  std::string str(string);
  std::regex exp(regex);
  std::string r = std::regex_replace(str, exp, after);
  auto result = new char[r.size() + 1];
  strncpy(result, r.c_str(), r.size());
  result[r.size()] = '\0';

  return result;
}

List * regex::scan(const char * string, const char * regex)
{
  auto list = new List();

  std::string str(string);
  std::regex exp(regex);
  std::smatch matches;

  while (std::regex_search(str, matches, exp)) {
    list->append(matches.str().c_str());
    str = matches.suffix().str();
    std::cout << "str " << str << std::endl;
  }

  return list;
}

} // namespace arken
