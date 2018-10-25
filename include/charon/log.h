// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_LOG_
#define _CHARON_LOG_

#include <fstream>
#include <mutex>
#include <map>

namespace charon
{

  class Log
  {

    private:
    std::string m_fileName;

    static std::map<std::string, int>           m_references;
    static std::map<std::string, std::string *> m_containers;
    static std::map<std::string, std::mutex  *> m_mutexes;
    static std::map<std::string, std::mutex  *> m_dumps;

    void lock();
    void unlock();

    public:
    Log(const char * fileName);
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

  };

} // namespace
#endif
