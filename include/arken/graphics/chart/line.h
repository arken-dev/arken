// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Line - translated from gruff/line.rb

#ifndef _ARKEN_GRAPHICS_CHART_LINE_
#define _ARKEN_GRAPHICS_CHART_LINE_

#include <arken/graphics/chart.h>

namespace arken {
namespace graphics {
namespace chart {

class Line : public Base {

  public:
  using Base::Base;

  void   setBaselineValue(double v); double baselineValue();
  void   setBaselineColor(string v); string baselineColor();
  void   setLineWidth(double v);     double lineWidth();
  void   setDotRadius(double v);     double dotRadius();
  void   setHideDots(bool v);        bool   hideDots();
  void   setHideLines(bool v);       bool   hideLines();

  protected:
  void draw() override;
  void normalize(bool force = false) override;

  private:
  double m_baselineValue = NoValue;
  string m_baselineColor = "red";
  double m_lineWidth     = NoValue;
  double m_dotRadius     = NoValue;
  bool   m_hideDots      = false;
  bool   m_hideLines     = false;
  double m_normBaseline  = NoValue;

  static bool containsOnlyOnePoint(const Dataset &row);
};

} // namespace chart
} // namespace graphics
} // namespace arken

#endif // _ARKEN_GRAPHICS_CHART_LINE_
