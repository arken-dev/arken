// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Translated from gruff/stacked_bar.rb + gruff/stacked_mixin.rb.

#include <arken/graphics/chart/stacked_bar.h>
#include <cmath>
#include <map>

namespace arken {
namespace graphics {
namespace chart {

void   StackedBar::setBarSpacing(double v) { m_barSpacing = v; }
double StackedBar::barSpacing()            { return m_barSpacing; }

void
StackedBar::getMaximumByStack()
{
  std::map<size_t, double> stackSums;

  for (auto &row : m_data) {
    for (size_t i = 0; i < row.values.size(); i++) {
      if (std::isnan(row.values[i])) continue;
      stackSums[i] += row.values[i];
    }
  }

  for (auto &kv : stackSums) {
    if (kv.second > m_maximumValue) m_maximumValue = kv.second;
  }

  m_minimumValue = 0;
}

void
StackedBar::draw()
{
  getMaximumByStack();
  Base::draw();
  if (!m_hasData) return;

  m_barWidth = m_graphWidth / (double) m_columnCount;
  double padding = (m_barWidth * (1 - m_barSpacing)) / 2;

  setStrokeOpacity(0.0);

  std::vector<double> height((size_t) m_columnCount, 0.0);

  for (auto &row : m_normData) {
    for (size_t pointIndex = 0; pointIndex < row.values.size(); pointIndex++) {
      double dataPoint = row.values[pointIndex];

      double labelCenter = m_graphLeft + (m_barWidth * pointIndex) + (m_barWidth * m_barSpacing / 2.0);
      drawLabel(labelCenter, (int) pointIndex);

      if (std::isnan(dataPoint) || dataPoint == 0.0) continue;

      double leftX  = m_graphLeft + (m_barWidth * pointIndex) + padding;
      double leftY  = m_graphTop + (m_graphHeight - dataPoint * m_graphHeight - height[pointIndex]) + 1;
      double rightX = leftX + m_barWidth * m_barSpacing;
      double rightY = m_graphTop + m_graphHeight - height[pointIndex] - 1;

      height[pointIndex] += dataPoint * m_graphHeight;

      // drawLabel() above may have reassigned the fill color (for the axis
      // label text), so it has to be set again right before the rectangle.
      setFillColor(row.color);
      drawRectangleShape(leftX, leftY, rightX, rightY);
    }
  }
}

} // namespace chart
} // namespace graphics
} // namespace arken
