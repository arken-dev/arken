// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_HTTP_HANDLE_
#define _ARKEN_HTTP_HANDLE_

namespace arken {
namespace net {
  class HttpHandle
  {
    public:
    static std::string sync(const char * data, size_t size);
  };
}
}
#endif //ARKEN_HTTP_HANDLE
