// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Isolates the handful of differences between ImageMagick 6 and 7 so that
// image.cpp/chart*.cpp can be written once and compiled against either.
// IMAGEMAGICK_VERSION (6 or 7) must be supplied by the build (see
// cmake/arken-graphics-{image,chart}.cmake).

#ifndef _ARKEN_GRAPHICS_IM_COMPAT_
#define _ARKEN_GRAPHICS_IM_COMPAT_

#ifndef IMAGEMAGICK_VERSION
  #error "IMAGEMAGICK_VERSION nao definida (defina via CMake: -DIMAGEMAGICK_VERSION=6 ou 7)"
#endif

#if IMAGEMAGICK_VERSION == 7
  #include <MagickWand/MagickWand.h>
#elif IMAGEMAGICK_VERSION == 6
  #include <wand/magick-wand.h>
#else
  #error "IMAGEMAGICK_VERSION deve ser 6 ou 7"
#endif

namespace arken
{
namespace graphics
{

// O enum de filtro de resize foi renomeado de FilterTypes (v6) para
// FilterType (v7).
#if IMAGEMAGICK_VERSION == 7
typedef FilterType  FilterTypeCompat;
#else
typedef FilterTypes FilterTypeCompat;
#endif

// MagickCompositeImage ganhou o parametro clip_to_self na v7.
inline MagickBooleanType
CompositeImageCompat(MagickWand *wand, MagickWand *source,
                      CompositeOperator op, ssize_t x, ssize_t y)
{
#if IMAGEMAGICK_VERSION == 7
  return MagickCompositeImage(wand, source, op, MagickTrue, x, y);
#else
  return MagickCompositeImage(wand, source, op, x, y);
#endif
}

// MagickResizeImage perdeu o parametro blur na v7.
inline MagickBooleanType
ResizeImageCompat(MagickWand *wand, size_t columns, size_t rows, FilterTypeCompat filter)
{
#if IMAGEMAGICK_VERSION == 7
  return MagickResizeImage(wand, columns, rows, filter);
#else
  return MagickResizeImage(wand, columns, rows, filter, 1.0);
#endif
}

} // namespace graphics
} // namespace arken

#endif
