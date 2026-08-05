// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Translated from gruff/bar.rb + gruff/bar_conversion.rb.

#include <arken/graphics/chart.h>
#include <cmath>

namespace arken {
namespace graphics {

void
ChartBar::setBarSpacing(double v) { m_barSpacing = v; }
double
ChartBar::barSpacing() { return m_barSpacing; }

void
ChartBar::draw()
{
  // Labels are centered over the left edge of the bar when there are more
  // labels than columns (same rule used for line graphs).
  m_centerLabelsOverPoint = ((int) m_labels.size() > m_columnCount);

  Chart::draw();
  if (!m_hasData) return;

  drawBars();
}

void
ChartBar::barYRange(double dataPoint, int mode, double zero, double &y1, double &y2)
{
  switch (mode) {
    case 1: // all values >= 0
      y1 = m_graphTop + m_graphHeight * (1 - dataPoint) + 1;
      y2 = m_graphTop + m_graphHeight - 1;
      break;
    case 2: // all values <= 0
      y1 = m_graphTop + 1;
      y2 = m_graphTop + m_graphHeight * (1 - dataPoint) - 1;
      break;
    case 3: { // values span both sides of zero
      double val = dataPoint - m_minimumValue / m_spread;
      y1 = m_graphTop + m_graphHeight * (1 - (val - zero)) + 1;
      y2 = m_graphTop + m_graphHeight * (1 - zero) - 1;
      break;
    }
    default:
      y1 = y2 = 0.0;
  }
}

void
ChartBar::drawBars()
{
  m_barWidth = m_graphWidth / (double) (m_columnCount * m_data.size());
  double padding = (m_barWidth * (1 - m_barSpacing)) / 2;

  setStrokeOpacity(0.0);

  int mode;
  double zero = 0.0;
  if (m_minimumValue >= 0) {
    mode = 1;
  } else if (m_maximumValue <= 0) {
    mode = 2;
  } else {
    mode = 3;
    zero = -m_minimumValue / m_spread;
  }

  for (size_t rowIndex = 0; rowIndex < m_normData.size(); rowIndex++) {
    Dataset &row = m_normData[rowIndex];

    for (size_t pointIndex = 0; pointIndex < row.values.size(); pointIndex++) {
      double dataPoint = row.values[pointIndex];
      if (std::isnan(dataPoint)) continue;

      double leftX = m_graphLeft
        + (m_barWidth * (rowIndex + pointIndex + ((m_data.size() - 1) * pointIndex)))
        + padding;
      double rightX = leftX + m_barWidth * m_barSpacing;

      double y1, y2;
      barYRange(dataPoint, mode, zero, y1, y2);

      setFillColor(row.color);
      drawRectangleShape(leftX, y1, rightX, y2);

      double labelCenter = m_graphLeft
        + (m_data.size() * m_barWidth * pointIndex)
        + (m_data.size() * m_barWidth / 2.0);

      drawLabel(labelCenter - (m_centerLabelsOverPoint ? m_barWidth / 2.0 : 0.0), (int) pointIndex);
    }
  }

  if (m_centerLabelsOverPoint) drawLabel(m_graphRight, m_columnCount);
}

} // namespace graphics
} // namespace arken
