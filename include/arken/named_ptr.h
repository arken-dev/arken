// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/string.h>
#include <unordered_map>
#include <mutex>

namespace arken {

template<class T>
class named_ptr
{

  string m_name;

  static std::unordered_map<string, int> & counter()
  {
    static std::unordered_map<string, int> s_counter;
    return s_counter;
  }

  static std::unordered_map<string, T *> & manager()
  {
    static std::unordered_map<string, T *> s_manager;
    return s_manager;
  }


  static std::mutex & mutex()
  {
    static std::mutex s_mutex;
    return s_mutex;
  }

  public:

  named_ptr() = default;

  named_ptr(string name)
  {
    std::unique_lock<std::mutex> lck(mutex());

    m_name = name;

    if ( manager().count(m_name) == 0 ) {
      counter()[m_name] = 0;
      manager()[m_name] = new T;
    }

    counter()[m_name]++;
  }


  ~named_ptr()
  {
    std::unique_lock<std::mutex> lck(mutex());

    counter()[m_name]--;

    if( counter()[m_name] == 0 ) {
      delete manager()[m_name];
      manager().erase(m_name);
      counter().erase(m_name);
    }

  }

  named_ptr<T> & operator=(const named_ptr<T> & obj)
  {
    std::unique_lock<std::mutex> lck(mutex());

    m_name = obj.m_name;

    if ( manager().count(m_name) == 0 ) {
      counter()[m_name] = 0;
      manager()[m_name] = new T;
    }

    counter()[m_name]++;

    return *this;
  }

  T * operator->() {
    std::unique_lock<std::mutex> lck(mutex());
    return manager()[m_name];
  }

};

} // namespace arken
