// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/log>
#include <ctime>
#include <mutex>

namespace arken {

std::unordered_map<std::string, int>           Log::m_count;
std::unordered_map<std::string, int>           Log::m_references;
std::unordered_map<std::string, int>           Log::m_max;
std::unordered_map<std::string, std::string *> Log::m_containers;

static std::mutex mtx;

Log::Log(const char * fileName, int max)
{
  std::unique_lock<std::mutex> lck(mtx);

  if( string::contains(fileName, "/") ) {
    m_fileName = fileName;
  } else {
    m_fileName = os::pwd();
    m_fileName.append("/logs/");
    m_fileName.append(fileName);
    m_fileName.append(".log");
  }

  if ( m_references.count(m_fileName) == 0 ) {
    m_max[m_fileName]           = max;
    m_count[m_fileName]         = 1;
    m_references[m_fileName]    = 0;
    m_containers[m_fileName]    = new std::string("");
  }

  m_references[m_fileName]++;
}

Log::~Log()
{
  std::unique_lock<std::mutex> lck(mtx);

  m_references[m_fileName]--;

  if( m_references[m_fileName] == 0 ) {
    delete m_containers[m_fileName];
    m_containers.erase(m_fileName);
    m_references.erase(m_fileName);
    m_max.erase(m_fileName);
    m_count.erase(m_fileName);
  }
}

void Log::append(const char * value)
{
  std::unique_lock<std::mutex> lck(mtx);

  std::string * container = m_containers[m_fileName];
  container->append(value);
  container->append("\n");

  int  max   = m_max[m_fileName];
  int  count = m_count[m_fileName];

  if( max > 0 && count >= max ) {
    this->_dump();
    count = 0;
  }

  count++;
  m_count[m_fileName] = count;
}

void Log::log(const char * level, const char * value)
{
  std::string log(level);

  // time
  std::time_t result = std::time(nullptr);
  char * t = std::asctime(std::localtime(&result));
  t[24] = 32;
  log.append(t);
  log.append(value);
  this->append(log.c_str());
}

void Log::trace(const char * value)
{
  this->log("TRACE|", value);
}

void Log::debug(const char * value)
{
  this->log("DEBUG|", value);
}

void Log::info(const char * value)
{
  this->log("INFO |", value);
}

void Log::warn(const char * value)
{
  this->log("WARN |", value);
}

void Log::error(const char * value)
{
  this->log("ERROR|", value);
}

void Log::fatal(const char * value)
{
  this->log("FATAL|", value);
}

void Log::dump()
{
  std::unique_lock<std::mutex> lck(mtx);
  this->_dump();
}

void Log::_dump()
{
  std::string *tmp = m_containers[m_fileName];
  m_containers[m_fileName]  = new std::string("");

  // write file
  std::ofstream file;
  file.open(m_fileName, std::ofstream::out | std::ofstream::app);
  file << *tmp;
  file.close();
  delete tmp;
}

} // namespace arken
