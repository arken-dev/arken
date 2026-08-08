// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Translated from Gruff 0.3.6 (https://github.com/topfunky/gruff), a Ruby
// charting library built on RMagick/ImageMagick. This port targets the
// MagickWand + DrawingWand C API instead of RMagick/MagickCore directly.

#ifndef _ARKEN_GRAPHICS_CHART_
#define _ARKEN_GRAPHICS_CHART_

#include <arken/base>
#include <vector>
#include <map>
#include <limits>

typedef struct _MagickWand  MagickWand;
typedef struct _DrawingWand DrawingWand;

namespace arken
{
namespace graphics
{
namespace chart
{

class Base {

  public:

  static constexpr double NoValue = std::numeric_limits<double>::quiet_NaN();

  struct Theme {
    std::vector<string> colors;
    string markerColor = "white";
    string fontColor   = "white";
    // 0 entries: transparent background
    // 1 entry:  solid background color
    // 2 entries: top->bottom gradient
    std::vector<string> backgroundColors;
  };

  // If height <= 0 the graph is drawn at 4/3 ratio based on width.
  Base(int width = 800, int height = -1);
  virtual ~Base();

  Base(const Base &) = delete;
  Base & operator=(const Base &) = delete;

  // -- data ------------------------------------------------------------

  void data(string name, std::vector<double> values, string color = "");
  void data(string name, double value, string color = "");

  // -- colors / theme ----------------------------------------------------

  void addColor(string color);
  void setColors(std::vector<string> colors);
  std::vector<string> colors();

  void setTheme(Theme theme);
  void themeKeynote();
  void theme37signals();
  void themeRailsKeynote();
  void themeOdeo();
  void themePastel();
  void themeGreyscale();

  // -- labels ------------------------------------------------------------

  void addLabel(int index, string label);
  void setLabels(std::map<int, string> labels);

  // -- attr_accessor equivalents ------------------------------------------

  void   setTopMargin(double v);    double topMargin();
  void   setBottomMargin(double v); double bottomMargin();
  void   setLeftMargin(double v);   double leftMargin();
  void   setRightMargin(double v);  double rightMargin();
  void   setMargins(double v);
  void   setTitleMargin(double v);  double titleMargin();
  void   setLegendMargin(double v); double legendMargin();

  void   setCenterLabelsOverPoint(bool v); bool centerLabelsOverPoint();
  void   setHasLeftLabels(bool v);         bool hasLeftLabels();

  void   setXAxisLabel(string v); string xAxisLabel();
  void   setYAxisLabel(string v); string yAxisLabel();
  void   setYAxisIncrement(double v); double yAxisIncrement();

  void   setTitle(string v); string title();

  void   setFont(string fontPath); string font();
  void   setFontColor(string v);   string fontColor();

  void   setHideLineMarkers(bool v); bool hideLineMarkers();
  void   setHideLegend(bool v);      bool hideLegend();
  void   setHideTitle(bool v);       bool hideTitle();
  void   setHideLineNumbers(bool v); bool hideLineNumbers();

  void   setNoDataMessage(string v); string noDataMessage();

  void   setTitleFontSize(double v);  double titleFontSize();
  void   setLegendFontSize(double v); double legendFontSize();
  void   setMarkerFontSize(double v); double markerFontSize();

  void   setMarkerColor(string v); string markerColor();
  void   setMarkerCount(int v);    int    markerCount();

  void   setMinimumValue(double v); double minimumValue();
  void   setMaximumValue(double v); double maximumValue();

  void   setSort(bool v); bool sort();

  void   setLegendBoxSize(double v); double legendBoxSize();

  // -- output --------------------------------------------------------------

  void write(string filename = "graph.png");

  protected:

  struct Dataset {
    string name;
    std::vector<double> values;
    string color;
  };

  enum class Gravity { North, East, West, Center };

  // -- canvas ----------------------------------------------------------

  MagickWand  * m_wand = nullptr;
  DrawingWand * m_draw = nullptr;

  double m_columns;
  double m_rows;
  double m_rawColumns = 800.0;
  double m_rawRows    = 600.0;
  double m_scaleRatio = 1.0;

  // -- data ------------------------------------------------------------

  std::vector<Dataset> m_data;
  std::vector<Dataset> m_normData;
  bool   m_normDataValid = false;
  int    m_columnCount   = 0;
  bool   m_hasData       = false;
  double m_spread        = 1.0;

  std::map<int, string> m_labels;
  std::map<int, bool>   m_labelsSeen;

  std::vector<string> m_colors;
  size_t m_colorIndex = 0;

  Theme m_themeOptions;

  // -- config ------------------------------------------------------------

  string m_title;
  string m_font;
  string m_fontColor      = "white";
  string m_markerColor    = "white";
  string m_noDataMessage  = "No Data";

  double m_topMargin = 20.0, m_bottomMargin = 20.0, m_leftMargin = 20.0, m_rightMargin = 20.0;
  double m_titleMargin = 20.0, m_legendMargin = 20.0;

  double m_titleFontSize  = 36.0;
  double m_legendFontSize = 20.0;
  double m_markerFontSize = 21.0;
  double m_legendBoxSize  = 20.0;

  bool m_hideLineMarkers = false, m_hideLegend = false, m_hideTitle = false, m_hideLineNumbers = false;
  bool m_centerLabelsOverPoint = true;
  bool m_hasLeftLabels = false;
  bool m_sort = true;

  string m_xAxisLabel, m_yAxisLabel;
  double m_yAxisIncrement = NoValue;

  double m_minimumValue = NoValue, m_maximumValue = NoValue;
  int    m_markerCount = -1; // -1 == unset

  // -- computed graph geometry (available to subclasses) --------------------

  double m_graphLeft = 0, m_graphRight = 0, m_graphTop = 0, m_graphBottom = 0;
  double m_graphWidth = 0, m_graphHeight = 0;
  double m_graphRightMargin = 0, m_graphBottomMargin = 0;
  double m_markerCapsHeight = 0, m_titleCapsHeight = 0, m_legendCapsHeight = 0;
  double m_increment = 1.0, m_incrementScaled = 1.0;

  // -- drawing lifecycle (mirrors Gruff::Base) -----------------------------

  virtual void draw();
  void setupDrawing();
  virtual void normalize(bool force = false);
  virtual void calculateSpread();
  void setupGraphMeasurements();
  void drawAxisLabels();
  virtual void drawLineMarkers();
  void drawLegend();
  void drawTitle();
  void drawLabel(double xOffset, int index);
  void drawNoData();

  void renderBackground();
  void resetThemes();

  double scaleFontSize(double value);
  double significant(double inc);
  void   sortNormData();
  static double sumValues(const std::vector<double> &values);
  string formatLabel(double value);

  double calculateCapsHeight(double fontSize);
  double calculateWidth(double fontSize, const string &text);

  string incrementColor();

  virtual bool largerThanMax(double dataPoint);
  virtual bool lessThanMin(double dataPoint);

  static double clipValueIfGreaterThan(double value, double maxValue);

  // -- low level drawing primitives (raw 800x{rows} coordinate space) -----

  void setFillColor(const string &color);
  void setStrokeColor(const string &color);
  void setStrokeOpacity(double opacity);
  void setFillOpacity(double opacity);
  void setStrokeWidth(double width);
  void setStrokeAntialias(bool antialias);

  void drawRectangleShape(double x1, double y1, double x2, double y2);
  void drawLineShape(double x1, double y1, double x2, double y2);
  void drawCircleShape(double ox, double oy, double px, double py);
  void drawEllipseShape(double cx, double cy, double rx, double ry, double startDeg, double endDeg);
  void drawPolygonShape(const std::vector<double> &points);

  void measureText(const string &text, double rawPointSize, bool bold, double &width, double &height);

  void annotateText(double width, double height, double x, double y,
                     const string &text, Gravity gravity, const string &fillColor,
                     double rawPointSize, bool bold = false, double angle = 0.0);

  private:

  void init(int width, int height);
};

} // namespace chart
} // namespace graphics
} // namespace arken

#endif
