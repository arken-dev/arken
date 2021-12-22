// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>
#include <arken/log.h>
#include <ctime>
#include <mutex>

std::mutex mtx;

namespace arken {

Log::Log(const char * fileName, int max)
{

  m_max   = max;
  m_count = 0;

  if( string::contains(fileName, "/") ) {
    m_fileName = fileName;
  } else {
    m_fileName = os::pwd();
    m_fileName.append("/logs/");
    m_fileName.append(fileName);
    m_fileName.append(".log");
  }

  string name = os::basename(fileName);
  m_resource  = named_ptr<std::string>(name);
}

Log::~Log() {
  std::unique_lock<std::mutex> lck(mtx);
  this->_dump();
}

void Log::append(const char * value)
{
  std::unique_lock<std::mutex> lck(mtx);

  m_resource->append(value);
  m_resource->append("\n");

  m_count++;

  if( m_max > 0 && m_count >= m_max ) {
    this->_dump();
    m_count = 0;
  }

}

void Log::log(const char * level, const char * value)
{
  std::string log(level);

  // time
  std::time_t result = std::time(nullptr);
  char * t = std::asctime(std::localtime(&result));
  t[24] = 32; // space
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
  std::ofstream file;
  file.open(m_fileName, std::ofstream::out | std::ofstream::app);
  file << m_resource->c_str();
  file.close();
  m_resource->clear();
}

} // namespace arken
