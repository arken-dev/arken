// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/concurrent/mailbox.h>

namespace arken {
namespace concurrent {

Mailbox::Mailbox(const char * name)
{
  m_resource = named_ptr<Mailbox::resource>(name);
}

Mailbox::Mailbox(const Mailbox &obj)
{
  m_resource = obj.m_resource;
}

Mailbox::~Mailbox() = default;

void Mailbox::send(const std::string & message)
{
  {
    std::lock_guard<std::mutex> lock(m_resource->m_mutex);

    if( m_resource->m_closed ) {
      throw "Mailbox is closed";
    }

    m_resource->m_queue.push(message);
  }

  m_resource->m_condition.notify_one();
}

std::optional<std::string> Mailbox::receive()
{
  std::unique_lock<std::mutex> lock(m_resource->m_mutex);

  m_resource->m_condition.wait(lock, [&] {
    return m_resource->m_closed || !m_resource->m_queue.empty();
  });

  if( m_resource->m_queue.empty() ) {
    return std::nullopt;
  }

  std::string message = m_resource->m_queue.front();
  m_resource->m_queue.pop();

  return message;
}

std::optional<std::string> Mailbox::fetch()
{
  std::lock_guard<std::mutex> lock(m_resource->m_mutex);

  if( m_resource->m_queue.empty() ) {
    return std::nullopt;
  }

  std::string message = m_resource->m_queue.front();
  m_resource->m_queue.pop();

  return message;
}

bool Mailbox::isClosed()
{
  std::lock_guard<std::mutex> lock(m_resource->m_mutex);
  return m_resource->m_closed;
}

void Mailbox::close()
{
  {
    std::lock_guard<std::mutex> lock(m_resource->m_mutex);
    m_resource->m_closed = true;
  }
  m_resource->m_condition.notify_all();
}

} // namespace concurrent
} // namespace arken
