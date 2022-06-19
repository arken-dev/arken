// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/concurrent/base.h>

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

bool base::purge()
{
  return m_purge;
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

arken::concurrent::Shared base::shared()
{
  return m_shared;
}

bool base::release()
{
  return true;
}

string base::uuid()
{
  return m_uuid;
}

void base::finished(bool flag)
{
  (*m_finished.get()) = flag;
}

string base::inspect()
{
  string tmp = m_inspect;
  if( !m_shared.info().empty() ) {
    tmp.append("#info:").append(m_shared.info());
  }
  return tmp;
}

} // namespace concurrent
} // namespace arken
