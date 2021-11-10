// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// https://cpp.hotexamples.com/examples/-/-/MagickGetImageWidth/cpp-magickgetimagewidth-function-examples.html
// https://github.com/janko/image_processing/blob/master/doc/minimagick.md
// https://github.com/rmagick/rmagick
// http://www.ostack.cn/?qa=722660/
// http://astroa.physics.metu.edu.tr/MANUALS/ImageMagick-6.2.5/www/api/magick-image.html
// https://imagemagick.org/api/magick-image.php

#ifndef _ARKEN_IMAGE_
#define _ARKEN_IMAGE_

#include <arken/base>
#include <wand/magick-wand.h>

namespace arken
{

class Image {

  private:
  MagickWand *mw;
  string m_path;
  int    m_width;
  int    m_height;

  public:
  Image(const Image & obj);
  Image(const char * path);
  Image(int width, int height, string color = "white");
  ~Image();
  void resize(int width, int height);
  void save(const char * path, int quality = -1);
  int  width();
  int  height();
  void composite(Image * img);
  void composite(Image * img, size_t width, size_t height);

};

} // namespace arken

#endif
