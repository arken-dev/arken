// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_LOG_
#define _ARKEN_LOG_

#include <fstream>
#include <unordered_map>

namespace arken
{

  class Log
  {

    private:
    std::string m_fileName;

    static std::unordered_map<std::string, int>           m_references;
    static std::unordered_map<std::string, int>           m_count;
    static std::unordered_map<std::string, int>           m_max;
    static std::unordered_map<std::string, std::string *> m_containers;

    public:
    Log(const char * fileName, int max = -1);
    ~Log();
    void append(const char * value);
    void log(const char * level, const char * log);
    void trace(const char * value);
    void debug(const char * value);
    void info(const char * value);
    void warn(const char * value);
    void error(const char * value);
    void fatal(const char * value);
    void dump();
    void _dump();

  };

} // namespace arken
#endif
