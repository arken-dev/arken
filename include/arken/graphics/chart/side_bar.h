// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// SideBar - translated from gruff/side_bar.rb

#ifndef _ARKEN_GRAPHICS_CHART_SIDE_BAR_
#define _ARKEN_GRAPHICS_CHART_SIDE_BAR_

#include <arken/graphics/chart.h>

namespace arken {
namespace graphics {
namespace chart {

class SideBar : public Base {

  public:
  using Base::Base;

  void   setBarSpacing(double v);
  double barSpacing();

  protected:
  void draw() override;
  void drawLineMarkers() override;

  // Hides (does not override, since Base::drawLabel is not virtual) the
  // base label placement: SideBar draws on the Y axis instead of the X.
  void drawLabel(double yOffset, int index);

  private:
  double m_barSpacing = 0.9;
  double m_barsWidth  = 0.0;
  double m_barWidth   = 0.0;
};

} // namespace chart
} // namespace graphics
} // namespace arken

#endif // _ARKEN_GRAPHICS_CHART_SIDE_BAR_
