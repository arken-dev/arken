// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_COMPRESS_ZIP_
#define _ARKEN_COMPRESS_ZIP_

namespace arken {
namespace compress {

class zip
{
  public:
  static bool decompress(const char * namefile, const char * output = 0);
};


} // namespace compress
} // namespace arken

#endif
