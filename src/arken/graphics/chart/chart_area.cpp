// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Translated from gruff/area.rb.

#include <arken/graphics/chart.h>

namespace arken {
namespace graphics {

void
ChartArea::draw()
{
  Chart::draw();
  if (!m_hasData) return;

  double xIncrement = m_graphWidth / (double) (m_columnCount - 1);
  setStrokeColor("transparent");

  for (Dataset &row : m_normData) {
    std::vector<double> polyPoints;
    double prevX = 0.0, prevY = 0.0;

    setFillColor(row.color);

    for (size_t index = 0; index < row.values.size(); index++) {
      double dataPoint = row.values[index];
      double newX = m_graphLeft + (xIncrement * index);
      double newY = m_graphTop + (m_graphHeight - dataPoint * m_graphHeight);

      if (prevX > 0 && prevY > 0) {
        polyPoints.push_back(newX);
        polyPoints.push_back(newY);
      } else {
        polyPoints.push_back(m_graphLeft);
        polyPoints.push_back(m_graphBottom - 1);
        polyPoints.push_back(newX);
        polyPoints.push_back(newY);
      }

      drawLabel(newX, (int) index);

      prevX = newX;
      prevY = newY;
    }

    polyPoints.push_back(m_graphRight);
    polyPoints.push_back(m_graphBottom - 1);
    polyPoints.push_back(m_graphLeft);
    polyPoints.push_back(m_graphBottom - 1);

    // drawLabel() above reassigns the fill color (for the axis label text),
    // so it has to be restored before the polygon is drawn.
    setFillColor(row.color);
    drawPolygonShape(polyPoints);
  }
}

} // namespace graphics
} // namespace arken
