// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
// LINKS
// https://nicedoc.io/nu-book/zxing-cpp
// https://github.com/nu-book/zxing-cpp
// https://en.wikipedia.org/wiki/Code_128
// https://stackoverflow.com/questions/33520844/zxing-notfoundexception-with-barcode-128c
// https://www.idautomation.com/barcode-fonts/code-128/user-manual/?gclid=Cj0KCQiA4b2MBhD2ARIsAIrcB-QM-Y6RcoY9rELR6ZX2gD-lvPchatNxyBxgnPeEOR3v7e23NTe29HAaAujQEALw_wcB
// https://www.barcodefaq.com/blog/integration-and-tutorials/what-is-difference-between-code-128-b/
// https://barcodewiz.com/user-manual/activex-control/code_128_a_b_c_and_auto.aspx

#ifndef _ARKEN_BARCODE_
#define _ARKEN_BARCODE_

#include <arken/base>

namespace arken
{

class Barcode {

  int m_width;
  int m_height;
  string m_format;
  string m_text;

  public:
  Barcode(int width, int height, string format, string text);
  ~Barcode();
  void save(string path);
};

} // namespace arken

#endif
