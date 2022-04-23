// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_DOC_PDF_
#define _ARKEN_DOC_PDF_

#include <memory>
#include <hpdf.h>
#include <arken/string.h>

namespace arken {
namespace doc {

  class Pdf
  {
    using string = arken::string;

    std::unique_ptr<HPDF_Doc>  m_doc{nullptr};
    std::unique_ptr<HPDF_Page> m_page{nullptr};

    public:
    Pdf();
    ~Pdf();
    void  write(double width, double heiht, string text);
    void  writeText(double width, double heiht, string text);
    void  writeRectangle(float x, float y, float width, float height, float border = 1);
    void  setFont(string name, int size);
    float width();
    float height();
    void  save(string fileName);
  };

} // namespace doc
} // namespace arken
#endif
