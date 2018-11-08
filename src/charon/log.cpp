// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <charon/base>
#include <charon/log>
#include <ctime>

using namespace charon;

std::unordered_map<std::string, int>           Log::m_references;
std::unordered_map<std::string, std::string *> Log::m_containers;
std::unordered_map<std::string, std::mutex  *> Log::m_mutexes;
std::unordered_map<std::string, std::mutex  *> Log::m_dumps;
static std::mutex m;

Log::Log(const char * fileName)
{
  m.lock();
  m_fileName = fileName;
  if ( m_references.count(m_fileName) == 0 ) {
    m_references[m_fileName]    = 0;
    m_mutexes[m_fileName]       = new std::mutex;
    m_dumps[m_fileName]         = new std::mutex;
    m_containers[m_fileName]    = new std::string("");
  }
  m_references[m_fileName]++;
  m.unlock();
}

Log::~Log()
{
  m.lock();
  m_references[m_fileName]--;
  if( m_references[m_fileName] == 0 ) {
    delete m_mutexes[m_fileName];
    delete m_dumps[m_fileName];
    delete m_containers[m_fileName];
    m_containers.erase(m_fileName);
    m_mutexes.erase(m_fileName);
    m_dumps.erase(m_fileName);
    m_references.erase(m_fileName);
  }
  m.unlock();
}

void Log::append(const char * value)
{
  this->lock();
  m_containers[m_fileName]->append(value);
  m_containers[m_fileName]->append("\n");
  this->unlock();
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
  m.lock();
  std::string * tmp = m_containers[m_fileName];
  m_containers[m_fileName]  = new std::string("");
  m.unlock();

  m_dumps[m_fileName]->lock();
  // write file
  std::ofstream file;
  file.open (m_fileName, std::ofstream::out | std::ofstream::app);
  file << *tmp;
  file.close();
  delete tmp;
  m_dumps[m_fileName]->unlock();
}

void Log::lock()
{
  m_mutexes[m_fileName]->lock();
}

void Log::unlock()
{
  m_mutexes[m_fileName]->unlock();
}
