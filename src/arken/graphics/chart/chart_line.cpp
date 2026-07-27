// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Translated from gruff/line.rb.

#include <arken/graphics/chart.h>
#include <cmath>

namespace arken {
namespace graphics {

void   ChartLine::setBaselineValue(double v) { m_baselineValue = v; }
double ChartLine::baselineValue()            { return m_baselineValue; }
void   ChartLine::setBaselineColor(string v) { m_baselineColor = v; }
string ChartLine::baselineColor()            { return m_baselineColor; }
void   ChartLine::setLineWidth(double v)     { m_lineWidth = v; }
double ChartLine::lineWidth()                { return m_lineWidth; }
void   ChartLine::setDotRadius(double v)     { m_dotRadius = v; }
double ChartLine::dotRadius()                { return m_dotRadius; }
void   ChartLine::setHideDots(bool v)        { m_hideDots = v; }
bool   ChartLine::hideDots()                 { return m_hideDots; }
void   ChartLine::setHideLines(bool v)       { m_hideLines = v; }
bool   ChartLine::hideLines()                { return m_hideLines; }

void
ChartLine::normalize(bool force)
{
  double baselineF = std::isnan(m_baselineValue) ? 0.0 : m_baselineValue;
  m_maximumValue = std::max(m_maximumValue, baselineF);

  Chart::normalize(force);

  if (!std::isnan(m_baselineValue)) {
    m_normBaseline = m_baselineValue / m_maximumValue;
  }
}

bool
ChartLine::containsOnlyOnePoint(const Dataset &row)
{
  bool found = false;
  for (double v : row.values) {
    if (!std::isnan(v)) {
      if (found) return false;
      found = true;
    }
  }
  return found;
}

void
ChartLine::draw()
{
  Chart::draw();
  if (!m_hasData) return;

  double xIncrement = (m_columnCount > 1)
    ? (m_graphWidth / (double) (m_columnCount - 1))
    : m_graphWidth;

  if (!std::isnan(m_normBaseline)) {
    double level = m_graphTop + (m_graphHeight - m_normBaseline * m_graphHeight);

    double dash[2] = { 10.0 * m_scaleRatio, 20.0 * m_scaleRatio };
    setStrokeColor(m_baselineColor);
    setFillOpacity(0.0);
    DrawSetStrokeDashArray(m_draw, 2, dash);
    setStrokeWidth(5.0);
    drawLineShape(m_graphLeft, level, m_graphLeft + m_graphWidth, level);
    DrawSetStrokeDashArray(m_draw, 0, nullptr);
  }

  for (Dataset &row : m_normData) {
    double prevX = NoValue, prevY = NoValue;
    bool onePoint = containsOnlyOnePoint(row);

    for (size_t index = 0; index < row.values.size(); index++) {
      double dataPoint = row.values[index];
      double newX = m_graphLeft + (xIncrement * index);
      if (std::isnan(dataPoint)) continue;

      drawLabel(newX, (int) index);

      double newY = m_graphTop + (m_graphHeight - dataPoint * m_graphHeight);

      setStrokeColor(row.color);
      setFillColor(row.color);
      setStrokeOpacity(1.0);

      // line_width / dot_radius are expressed in final canvas pixels (like
      // Gruff), so we convert to raw space before handing them to helpers
      // that scale by m_scaleRatio internally.
      double lineWidthFinal = !std::isnan(m_lineWidth)
        ? m_lineWidth
        : clipValueIfGreaterThan(m_columns / (row.values.size() * 4.0), 5.0);
      setStrokeWidth(lineWidthFinal / m_scaleRatio);

      double circleRadiusFinal = !std::isnan(m_dotRadius)
        ? m_dotRadius
        : clipValueIfGreaterThan(m_columns / (row.values.size() * 2.5), 5.0);
      double circleRadiusRaw = circleRadiusFinal / m_scaleRatio;

      if (!m_hideLines && !std::isnan(prevX) && !std::isnan(prevY)) {
        drawLineShape(prevX, prevY, newX, newY);
      } else if (onePoint) {
        drawCircleShape(newX, newY, newX - circleRadiusRaw, newY);
      }
      if (!m_hideDots) drawCircleShape(newX, newY, newX - circleRadiusRaw, newY);

      prevX = newX;
      prevY = newY;
    }
  }
}

} // namespace graphics
} // namespace arken
