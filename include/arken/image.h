// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef _ARKEN_IMAGE_
#define _ARKEN_IMAGE_

#include<arken/base>

namespace arken
{

class Image {

  private:
  string m_path;

  public:
  Image(const char * path);
  void save(const char * path, int quality = -1);

};

}

#endif
