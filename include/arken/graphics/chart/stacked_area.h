// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// StackedArea - translated from gruff/stacked_area.rb (+ gruff/stacked_mixin.rb)

#ifndef _ARKEN_GRAPHICS_CHART_STACKED_AREA_
#define _ARKEN_GRAPHICS_CHART_STACKED_AREA_

#include <arken/graphics/chart.h>

namespace arken {
namespace graphics {
namespace chart {

class StackedArea : public Base {

  public:
  using Base::Base;

  void setLastSeriesGoesOnBottom(bool v);
  bool lastSeriesGoesOnBottom();

  protected:
  void draw() override;

  private:
  bool m_lastSeriesGoesOnBottom = false;

  void getMaximumByStack();
};

} // namespace chart
} // namespace graphics
} // namespace arken

#endif // _ARKEN_GRAPHICS_CHART_STACKED_AREA_
