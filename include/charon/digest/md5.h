// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_DIGEST_MD5_
#define _CHARON_DIGEST_MD5_

namespace charon {
namespace digest {

class md5 {
  public:
  static char * hash(const char * value);
  static char * hash(const char * value, int length);
  static char * file(const char * path);
};

}
}

#endif
