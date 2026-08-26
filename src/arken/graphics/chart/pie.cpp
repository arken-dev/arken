// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Translated from gruff/pie.rb.

#include <arken/graphics/chart/pie.h>
#include <cmath>
#include <cstdio>
#include <algorithm>

namespace arken {
namespace graphics {
namespace chart {

static const double TEXT_OFFSET_PERCENTAGE = 0.15;

void   Pie::setZeroDegree(double v) { m_zeroDegree = v; }
double Pie::zeroDegree()            { return m_zeroDegree; }

double
Pie::sumsForPie()
{
  double total = 0.0;
  for (auto &row : m_data) {
    if (!row.values.empty()) total += row.values[0];
  }
  return total;
}

void
Pie::drawSliceLabel(double centerX, double centerY, double angle, double radius, const string &amount)
{
  double rOffset       = 20.0;
  double radiusOffset  = radius + rOffset;
  double ellipseFactor = radiusOffset * 0.15;
  double rad = angle * (M_PI / 180.0);

  double x = centerX + (radiusOffset + ellipseFactor) * std::cos(rad);
  double y = centerY + radiusOffset * std::sin(rad);

  annotateText(0, 0, x, y, amount, Gravity::Center, m_fontColor, m_markerFontSize, true);
}

void
Pie::draw()
{
  m_hideLineMarkers = true;

  Base::draw();
  if (!m_hasData) return;

  double radius  = std::min(m_graphWidth, m_graphHeight) / 2.0 * 0.8;
  double centerX = m_graphLeft + (m_graphWidth / 2.0);
  double centerY = m_graphTop + (m_graphHeight / 2.0) - 10;
  double totalSum = sumsForPie();
  double prevDegrees = m_zeroDegree;

  std::vector<Dataset> data = m_data;
  if (m_sort) {
    std::sort(data.begin(), data.end(), [](const Dataset &a, const Dataset &b) {
      double av = a.values.empty() ? 0.0 : a.values[0];
      double bv = b.values.empty() ? 0.0 : b.values[0];
      return av < bv;
    });
  }

  for (auto &row : data) {
    if (row.values.empty() || !(row.values[0] > 0)) continue;

    double value = row.values[0];

    setStrokeColor(row.color);
    setFillColor("transparent");
    setStrokeWidth(radius); // stroke is drawn centered on the ellipse path, so
                             // width == radius makes it fill from center to edge

    double currentDegrees = (value / totalSum) * 360.0;

    // +0.5 degree fudge factor avoids visible gaps between slices
    drawEllipseShape(centerX, centerY, radius / 2.0, radius / 2.0,
                      prevDegrees, prevDegrees + currentDegrees + 0.5);

    double halfAngle = prevDegrees + currentDegrees / 2.0;

    char buffer[32];
    std::snprintf(buffer, sizeof(buffer), "%ld%%", std::lround((value / totalSum) * 100.0));

    // arken::string(char*&&) takes ownership of the pointer instead of
    // copying, so a non-const char* must be cast to const char* here or the
    // destructor ends up delete[]-ing this stack buffer.
    drawSliceLabel(centerX, centerY, halfAngle, radius + (radius * TEXT_OFFSET_PERCENTAGE),
                   string((const char *) buffer));

    prevDegrees += currentDegrees;
  }
}

} // namespace chart
} // namespace graphics
} // namespace arken
