// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_BASE64_
#define _ARKEN_BASE64_

namespace arken
{

class base64 {
  public:
  static char * decode(const char * data, size_t *size);
  static char * encode(const char * data, int len);
  static char * encode(const char * data);
};

}

#endif
