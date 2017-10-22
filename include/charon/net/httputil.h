// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef HTTP_UTIL_HEADER
#define HTTP_UTIL_HEADER

namespace charon {
namespace net {

class HttpUtil
{
  public:
  static const char * status(int code);
};

}
}
#endif
