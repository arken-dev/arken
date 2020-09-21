// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/base>

using namespace arken;

std::unordered_map<std::string, int>          Lock::m_references;
std::unordered_map<std::string, std::mutex *> Lock::m_containers;

static std::mutex m;

Lock::Lock(const char * index)
{
  m.lock();
  m_index = index;
  if ( m_references.count(m_index) == 0 ) {
    m_references[m_index] = 0;
    m_containers[m_index] = new std::mutex;
  }
  m_references[m_index]++;
  m.unlock();
}

Lock::~Lock()
{
  m.lock();
  m_references[m_index]--;
  if( m_references[m_index] == 0 ) {
    delete m_containers[m_index];
    m_containers.erase(m_index);
    m_references.erase(m_index);
  }
  m.unlock();
}

void Lock::enable()
{
  m_containers[m_index]->lock();
}

void Lock::disable()
{
  m_containers[m_index]->unlock();
}
