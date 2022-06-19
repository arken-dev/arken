// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CONCURRENT_BASE_
#define _ARKEN_CONCURRENT_BASE_

#include <atomic>
#include <memory>
#include <arken/base>
#include <arken/concurrent/shared.h>

namespace arken {
namespace concurrent {

class base {
  using Shared = arken::concurrent::Shared;
  using string = arken::string;

  protected:
  string m_uuid{os::uuid()};
  string m_params;
  string m_fileName;
  string m_inspect;
  double m_microtime{os::microtime()};
  bool m_purge{false};
  std::shared_ptr<std::atomic<bool>> m_finished{new std::atomic<bool>(false)};
  Shared m_shared;

  public:
  virtual void run() /*= 0*/;
  base();
  virtual ~base();
  bool   finished();
  virtual bool release();
  void   finished(bool flag);
  string inspect();
  string uuid();
  bool purge();
  Shared shared();
  double microtime();
  void wait();
  operator bool() const;
  static void swap(concurrent::base * source, concurrent::base * destination);
};

} // namespace concurrent
} // namespace arken

#endif // _ARKEN_CONCURRENT_BASE_
