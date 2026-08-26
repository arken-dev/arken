// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Bar - translated from gruff/bar.rb (+ gruff/bar_conversion.rb)

#ifndef _ARKEN_GRAPHICS_CHART_BAR_
#define _ARKEN_GRAPHICS_CHART_BAR_

#include <arken/graphics/chart.h>

namespace arken {
namespace graphics {
namespace chart {

class Bar : public Base {

  public:
  using Base::Base;

  void   setBarSpacing(double v);
  double barSpacing();

  protected:
  void draw() override;

  private:
  double m_barSpacing = 0.9;
  double m_barWidth   = 0.0;

  void drawBars();
  void barYRange(double dataPoint, int mode, double zero, double &y1, double &y2);
};

} // namespace chart
} // namespace graphics
} // namespace arken

#endif // _ARKEN_GRAPHICS_CHART_BAR_
