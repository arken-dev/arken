// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Translated from gruff/side_bar.rb.

#include <arken/graphics/chart/side_bar.h>
#include <cstdio>
#include <cstdlib>

namespace arken {
namespace graphics {
namespace chart {

static const double LABEL_MARGIN = 10.0;

void   SideBar::setBarSpacing(double v) { m_barSpacing = v; }
double SideBar::barSpacing()            { return m_barSpacing; }

void
SideBar::draw()
{
  m_hasLeftLabels = true;
  Base::draw();
  if (!m_hasData) return;

  m_barsWidth = m_graphHeight / (double) m_columnCount;
  m_barWidth  = m_barsWidth * m_barSpacing / (double) m_normData.size();

  setStrokeOpacity(0.0);

  std::vector<double> height((size_t) m_columnCount, 0.0);
  double padding = (m_barsWidth * (1 - m_barSpacing)) / 2;

  for (size_t rowIndex = 0; rowIndex < m_normData.size(); rowIndex++) {
    Dataset &row = m_normData[rowIndex];

    for (size_t pointIndex = 0; pointIndex < row.values.size(); pointIndex++) {
      double dataPoint = row.values[pointIndex];

      // temp1/temp2 cancel height algebraically (difference always equals
      // dataPoint * m_graphWidth) and length[] never actually moves off
      // m_graphLeft in upstream Gruff -- kept verbatim to mirror
      // gruff/side_bar.rb rather than "simplify" behavior we didn't design.
      double temp1 = m_graphLeft + (m_graphWidth - dataPoint * m_graphWidth - height[pointIndex]);
      double temp2 = m_graphLeft + m_graphWidth - height[pointIndex];
      double difference = temp2 - temp1;

      double leftX  = m_graphLeft - 1;
      double leftY  = m_graphTop + (m_barsWidth * pointIndex) + (m_barWidth * rowIndex) + padding;
      double rightX = leftX + difference;
      double rightY = leftY + m_barWidth;

      height[pointIndex] += dataPoint * m_graphWidth;

      // A previous point's drawLabel() call may have reassigned the fill
      // color (for the axis label text), so it has to be set again right
      // before the rectangle.
      setFillColor(row.color);
      drawRectangleShape(leftX, leftY, rightX, rightY);

      double labelCenter = m_graphTop + (m_barsWidth * pointIndex + m_barsWidth / 2.0);
      drawLabel(labelCenter, (int) pointIndex);
    }
  }
}

// Draws vertical background lines and their value labels instead of the
// base class' horizontal ones.
void
SideBar::drawLineMarkers()
{
  if (m_hideLineMarkers) return;

  setStrokeAntialias(false);
  setStrokeColor(m_markerColor);
  setStrokeWidth(1.0);

  int numberOfLines = 5;
  double increment = significant(m_maximumValue / (double) numberOfLines);

  for (int index = 0; index <= numberOfLines; index++) {
    double lineDiff = (m_graphRight - m_graphLeft) / numberOfLines;
    double x = m_graphRight - (lineDiff * index) - 1;

    drawLineShape(x, m_graphBottom, x, m_graphTop);

    int diff = index - numberOfLines;
    double markerLabel = std::abs(diff) * increment;

    if (!m_hideLineNumbers) {
      char buffer[32];
      std::snprintf(buffer, sizeof(buffer), "%g", markerLabel);
      annotateText(0.0, 0.0, x, m_graphBottom + (LABEL_MARGIN * 2.0), string((const char *) buffer),
                   Gravity::Center, m_fontColor, m_markerFontSize);
    }

    setStrokeAntialias(true);
  }
}

// Draws on the Y axis instead of the X (note: unlike the base version, this
// one is not gated by m_hideLineMarkers -- matches gruff/side_bar.rb).
void
SideBar::drawLabel(double yOffset, int index)
{
  if (m_labels.count(index) && !m_labelsSeen.count(index)) {
    annotateText(1.0, 1.0, -m_graphLeft + LABEL_MARGIN * 2.0, yOffset, m_labels[index],
                 Gravity::East, m_fontColor, m_markerFontSize);
    m_labelsSeen[index] = true;
  }
}

} // namespace chart
} // namespace graphics
} // namespace arken
