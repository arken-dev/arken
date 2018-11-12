// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_DIGEST_
#define _CHARON_DIGEST_

namespace charon
{

class digest {
  public:
  static char * md5(const char * hash);
  static char * md5(const char * hash, int length);
  static char * sha1(const char * hash);
  static char * sha1(const char * hash, int length);
};

}

#endif
