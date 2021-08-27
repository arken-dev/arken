// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_DIGEST_SHA1_
#define _ARKEN_DIGEST_SHA1_

namespace arken {
namespace digest {

class sha1 {
  public:
  static char * hash(const char * value, int length);
  static char * file(const char * path);
};

} // namespace digest
} // namespace arken

#endif
