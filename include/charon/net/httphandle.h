// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_HTTP_HANDLE_
#define _CHARON_HTTP_HANDLE_

namespace charon {
namespace net {
  class HttpHandle
  {
    public:
    static char * sync(const char * data, int size, int * num);
  };
}
}
#endif //CHARON_HTTP_HANDLE
