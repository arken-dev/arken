// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_COMPRESS_ZIP_
#define _ARKEN_COMPRESS_ZIP_

#include <zip.h>

namespace arken {
namespace compress {

class Zip
{
  private:
  zip * m_zip;
  bool  m_closed;

  public:
  static bool decompress(const char * namefile, const char * output = nullptr);
  Zip(const char * namefile);
  ~Zip();
  void addFile(const char * path);
  void addBuffer(const char * name, const char * buf, size_t size);
  void save();
};


} // namespace compress
} // namespace arken

#endif
