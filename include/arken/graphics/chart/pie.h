// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Pie - translated from gruff/pie.rb

#ifndef _ARKEN_GRAPHICS_CHART_PIE_
#define _ARKEN_GRAPHICS_CHART_PIE_

#include <arken/graphics/chart.h>

namespace arken {
namespace graphics {
namespace chart {

class Pie : public Base {

  public:
  using Base::Base;

  void   setZeroDegree(double v);
  double zeroDegree();

  protected:
  void draw() override;

  private:
  double m_zeroDegree = 0.0;

  double sumsForPie();
  void   drawSliceLabel(double centerX, double centerY, double angle, double radius, const string &amount);
};

} // namespace chart
} // namespace graphics
} // namespace arken

#endif // _ARKEN_GRAPHICS_CHART_PIE_
