// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Translated from gruff/stacked_area.rb + gruff/stacked_mixin.rb.

#include <arken/graphics/chart/stacked_area.h>
#include <algorithm>
#include <cmath>
#include <map>

namespace arken {
namespace graphics {
namespace chart {

void StackedArea::setLastSeriesGoesOnBottom(bool v) { m_lastSeriesGoesOnBottom = v; }
bool StackedArea::lastSeriesGoesOnBottom()           { return m_lastSeriesGoesOnBottom; }

void
StackedArea::getMaximumByStack()
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
StackedArea::draw()
{
  getMaximumByStack();
  Base::draw();
  if (!m_hasData) return;

  double xIncrement = m_graphWidth / (double) (m_columnCount - 1);
  setStrokeColor("transparent");

  std::vector<double> height((size_t) m_columnCount, 0.0);

  // Each layer is drawn as the band between its own cumulative top edge and
  // the previous layer's, so layers stack bottom-up in iteration order;
  // lastSeriesGoesOnBottom reverses which dataset ends up at the bottom.
  std::vector<Dataset *> rows;
  for (auto &row : m_normData) rows.push_back(&row);
  if (m_lastSeriesGoesOnBottom) std::reverse(rows.begin(), rows.end());

  std::vector<double> prevPoints;

  for (Dataset *rowPtr : rows) {
    Dataset &row = *rowPtr;
    std::vector<double> points;

    for (size_t index = 0; index < row.values.size(); index++) {
      double dataPoint = row.values[index];
      double newX = m_graphLeft + (xIncrement * index);
      double newY = m_graphTop + (m_graphHeight - dataPoint * m_graphHeight - height[index]);

      height[index] += dataPoint * m_graphHeight;

      points.push_back(newX);
      points.push_back(newY);

      drawLabel(newX, (int) index);
    }

    std::vector<double> polyPoints = points;

    if (!prevPoints.empty()) {
      for (size_t i = prevPoints.size() / 2; i-- > 0; ) {
        polyPoints.push_back(prevPoints[2 * i]);
        polyPoints.push_back(prevPoints[2 * i + 1]);
      }
    } else {
      polyPoints.push_back(m_graphRight);
      polyPoints.push_back(m_graphBottom - 1);
      polyPoints.push_back(m_graphLeft);
      polyPoints.push_back(m_graphBottom - 1);
    }
    polyPoints.push_back(points[0]);
    polyPoints.push_back(points[1]);

    // drawLabel() above reassigns the fill color (for the axis label text),
    // so it has to be set again right before the polygon is drawn.
    setFillColor(row.color);
    drawPolygonShape(polyPoints);

    prevPoints = points;
  }
}

} // namespace chart
} // namespace graphics
} // namespace arken
