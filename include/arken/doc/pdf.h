// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_DOC_PDF_
#define _ARKEN_DOC_PDF_

#include <arken/string.h>

namespace arken {
namespace doc {

  class Pdf
  {
    using string = arken::string;

    void * m_resource = nullptr;
    void * m_page     = nullptr;

    public:
    Pdf();
    ~Pdf();
    void  write(double width, double heiht, string text);
    void  writeText(double width, double heiht, string text);
    void  writeRectangle(float x, float y, float width, float height, float line_width = 1);
    float width();
    float height();
    void  save(string fileName);
  };

} // namespace doc
} // namespace arken
#endif
