// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/concurrent/base.h>
#include <arken/concurrent/core.h>

namespace arken {
namespace concurrent {

base::base() : m_uuid{""}, m_microtime{0}
{ }

void base::run()
{ }

base::~base() = default;

bool base::finished()
{
  return (*m_finished.get());
}

bool base::release()
{
  return m_release;
}

base::operator bool() const {
  return m_microtime > 0;
}

void base::wait()
{
  while ((*m_finished.get()) == false) {
    os::sleep(0.05);
  }
}

double base::microtime()
{
  return m_microtime;
}

arken::mvm::Shared base::shared()
{
  return m_shared;
}

string base::uuid()
{
  return m_uuid;
}

string base::params()
{
  return m_params;
}

void base::finished(bool flag)
{
  (*m_finished.get()) = flag;
}

string base::inspect()
{
  string tmp(m_shared.name());
    tmp.
    append(m_shared.name()).
    append(m_fileName).
    append("#").
    append(m_params.escape());

  if( !m_shared.info().empty() ) {
    tmp.append("#info:").append(m_shared.info());
  }

  return tmp;
}

void concurrent::base::swap(Shared shared)
{
  std::unique_lock<std::mutex> lck(core::mutex());
  m_shared = shared;
}

} // namespace concurrent
} // namespace arken
