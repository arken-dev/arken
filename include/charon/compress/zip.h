// Copyright 2016 The Charon Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _CHARON_COMPRESS_ZIP_
#define _CHARON_COMPRESS_ZIP_

namespace charon {
namespace compress {

class zip
{
  public:
  static bool decompress(const char * namefile, const char * output = 0);
};


}
}

#endif
