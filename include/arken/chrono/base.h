// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_CHRONO_BASE_
#define _ARKEN_CHRONO_BASE_

#include <ctime>
#include <arken/string.h>

namespace arken {

namespace chrono {
  struct tm* localtime(const time_t* time);
  char* asctime( const std::tm* time_ptr );
  char *strptime(const char *buf, const char *format, struct tm *tm);
  time_t mktime(struct std::tm *time_ptr);
  time_t time( std::time_t* arg );
  size_t strftime( char* str, std::size_t count, const char* format, const std::tm* tp );

} // namespace chrono
} // namespace arken
#endif
