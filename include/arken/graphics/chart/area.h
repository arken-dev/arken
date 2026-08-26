// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Area - translated from gruff/area.rb

#ifndef _ARKEN_GRAPHICS_CHART_AREA_
#define _ARKEN_GRAPHICS_CHART_AREA_

#include <arken/graphics/chart.h>

namespace arken {
namespace graphics {
namespace chart {

class Area : public Base {

  public:
  using Base::Base;

  protected:
  void draw() override;
};

} // namespace chart
} // namespace graphics
} // namespace arken

#endif // _ARKEN_GRAPHICS_CHART_AREA_
