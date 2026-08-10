// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_UTILS_GLOB_
#define _ARKEN_UTILS_GLOB_

#include <string>

namespace arken {
namespace utils {

class glob {
  public:
  static bool hasWildcard(const std::string & segment);
  static bool match(const std::string & name, const std::string & pattern);

  private:
  static bool matchClass(const std::string & pattern, size_t start, size_t end, char c);
};

} // namespace utils
} // namespace arken

#endif // _ARKEN_UTILS_GLOB_
