// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// StackedBar - translated from gruff/stacked_bar.rb (+ gruff/stacked_mixin.rb)

#ifndef _ARKEN_GRAPHICS_CHART_STACKED_BAR_
#define _ARKEN_GRAPHICS_CHART_STACKED_BAR_

#include <arken/graphics/chart.h>

namespace arken {
namespace graphics {
namespace chart {

class StackedBar : public Base {

  public:
  using Base::Base;

  void   setBarSpacing(double v);
  double barSpacing();

  protected:
  void draw() override;

  private:
  double m_barSpacing = 0.9;
  double m_barWidth   = 0.0;

  void getMaximumByStack();
};

} // namespace chart
} // namespace graphics
} // namespace arken

#endif // _ARKEN_GRAPHICS_CHART_STACKED_BAR_
