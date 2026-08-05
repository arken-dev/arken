// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <arken/graphics/chart.h>
#include <arken/graphics/im_compat.h>
#include <arken/mvm.h>
#include <arken/os.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <atomic>
#include <string>
#include <clocale>

namespace arken {
namespace graphics {
namespace chart {

static const double LABEL_MARGIN = 10.0;

// MagickWandGenesis/Terminus initialize and tear down process-wide state, so
// they must be reference counted across every live Base instance instead of
// being called per-object (Terminus running while a sibling Base is still
// alive crashes on the next MagickWand call).
static std::atomic<int> g_wandInstances{ 0 };

static inline const unsigned char *
ustr(const string &s) {
  return reinterpret_cast<const unsigned char *>((const char *) s);
}

// Builds sem fontconfig (p.ex. o ImageMagick 7 do Homebrew no macOS, ou
// containers minimos sem nenhuma fonte instalada) nao registram fonte
// alguma: MagickQueryFonts devolve zero e o IM nao tem como resolver a
// fonte padrao "". Como MagickDrawImage aborta a stream MVG no primeiro
// primitivo que falha, um unico texto sem fonte descarta todo o desenho do
// grafico -- barras inclusive -- e sobra so o fundo. Nesse caso caimos na
// Roboto empacotada em assets/fonts, que acompanha o proprio arken e
// portanto nao depende de nada instalado no sistema.
static string
bundledFontPath()
{
  // Um caminho invalido aqui reproduziria o bug que esta funcao existe para
  // evitar (grafico so com o fundo), entao a env so vence se apontar mesmo
  // para um arquivo; caso contrario cai na fonte empacotada.
  const char *env = std::getenv("ARKEN_CHART_FONT");
  if (env && *env && os::isfile(env)) return string(env);

  string path(mvm::path());
  path.append("/assets/fonts/Roboto-Regular.ttf");

  return os::isfile(path) ? path : string("");
}

// A varredura toca o disco e o resultado nao muda durante o processo, entao
// roda uma vez so. MagickQueryFonts exige MagickWandGenesis ja chamado.
static string
defaultFont()
{
  static const string cached = [] {
    size_t total = 0;
    char **fonts = MagickQueryFonts("*", &total);
    if (fonts) MagickRelinquishMemory(fonts);
    return (total > 0) ? string("") : bundledFontPath();
  }();

  return cached;
}

//------------------------------------------------------------------------------
// construction / destruction
//------------------------------------------------------------------------------

Base::Base(int width, int height)
{
  init(width, height);
}

void
Base::init(int width, int height)
{
  if (g_wandInstances.fetch_add(1) == 0) MagickWandGenesis();

  // MagickWandGenesis() applies the process locale, and MagickCore
  // serializes MVG drawing primitives (coordinates, point sizes) with the
  // current LC_NUMERIC. In locales that use ',' as the decimal separator
  // (e.g. pt_BR), every annotateText()/shape call would corrupt the MVG
  // primitive stream and MagickDrawImage would silently drop all drawing.
  std::setlocale(LC_NUMERIC, "C");

  m_font = defaultFont();

  m_columns = (double) width;
  m_rows    = (height > 0) ? (double) height : width * 0.75;

  m_rawColumns = 800.0;
  m_rawRows    = 800.0 * (m_rows / m_columns);
  m_scaleRatio = m_columns / m_rawColumns;

  resetThemes();
  themeKeynote();
}

Base::~Base()
{
  if (m_draw) m_draw = DestroyDrawingWand(m_draw);
  if (m_wand) m_wand = DestroyMagickWand(m_wand);
  if (g_wandInstances.fetch_sub(1) == 1) MagickWandTerminus();
}

//------------------------------------------------------------------------------
// data
//------------------------------------------------------------------------------

void
Base::data(string name, std::vector<double> values, string color)
{
  Dataset row;
  row.name  = name;
  row.color = color.empty() ? incrementColor() : color;
  row.values = values;

  m_columnCount = std::max(m_columnCount, (int) values.size());

  for (double point : values) {
    if (std::isnan(point)) continue;

    if (std::isnan(m_maximumValue) && std::isnan(m_minimumValue)) {
      m_maximumValue = m_minimumValue = point;
    }

    m_maximumValue = largerThanMax(point) ? point : m_maximumValue;
    if (m_maximumValue >= 0) m_hasData = true;

    m_minimumValue = lessThanMin(point) ? point : m_minimumValue;
    if (m_minimumValue < 0) m_hasData = true;
  }

  m_data.push_back(row);
  m_normDataValid = false;
}

void
Base::data(string name, double value, string color)
{
  data(name, std::vector<double>{ value }, color);
}

//------------------------------------------------------------------------------
// colors / theme
//------------------------------------------------------------------------------

void
Base::addColor(string color)
{
  m_colors.push_back(color);
}

void
Base::setColors(std::vector<string> colors)
{
  m_colors = colors;
  m_colorIndex = 0;
}

std::vector<string>
Base::colors()
{
  return m_colors;
}

void
Base::setTheme(Theme theme)
{
  resetThemes();

  m_themeOptions = theme;
  m_colors       = theme.colors;
  m_markerColor  = theme.markerColor.empty() ? string("white") : theme.markerColor;
  m_fontColor    = theme.fontColor.empty() ? m_markerColor : theme.fontColor;

  renderBackground();
}

void
Base::themeKeynote()
{
  Theme t;
  t.colors = { "#FDD84E", "#6886B4", "#72AE6E", "#D1695E", "#8A6EAF", "#EFAA43", "white" };
  t.markerColor = "white";
  t.fontColor   = "white";
  t.backgroundColors = { "black", "#4a465a" };
  setTheme(t);
}

void
Base::theme37signals()
{
  Theme t;
  t.colors = { "#FFF804", "#336699", "#339933", "#ff0000", "#cc99cc", "#cf5910", "black" };
  t.markerColor = "black";
  t.fontColor   = "black";
  t.backgroundColors = { "#d1edf5", "white" };
  setTheme(t);
}

void
Base::themeRailsKeynote()
{
  Theme t;
  t.colors = { "#00ff00", "#333333", "#ff5d00", "#f61100", "white", "#999999", "black" };
  t.markerColor = "white";
  t.fontColor   = "white";
  t.backgroundColors = { "#0083a3", "#0083a3" };
  setTheme(t);
}

void
Base::themeOdeo()
{
  Theme t;
  t.colors = { "#202020", "white", "#3a5b87", "#a21764", "#8ab438", "#999999", "black" };
  t.markerColor = "white";
  t.fontColor   = "white";
  t.backgroundColors = { "#ff47a4", "#ff1f81" };
  setTheme(t);
}

void
Base::themePastel()
{
  Theme t;
  t.colors = { "#a9dada", "#aedaa9", "#daaea9", "#dadaa9", "#a9a9da", "#daaeda", "#dadada" };
  t.markerColor = "#aea9a9";
  t.fontColor   = "black";
  t.backgroundColors = { "white" };
  setTheme(t);
}

void
Base::themeGreyscale()
{
  Theme t;
  t.colors = { "#282828", "#383838", "#686868", "#989898", "#c8c8c8", "#e8e8e8" };
  t.markerColor = "#aea9a9";
  t.fontColor   = "black";
  t.backgroundColors = { "white" };
  setTheme(t);
}

//------------------------------------------------------------------------------
// labels
//------------------------------------------------------------------------------

void
Base::addLabel(int index, string label)
{
  m_labels[index] = label;
}

void
Base::setLabels(std::map<int, string> labels)
{
  m_labels = labels;
}

//------------------------------------------------------------------------------
// attr_accessor equivalents
//------------------------------------------------------------------------------

void   Base::setTopMargin(double v)    { m_topMargin = v; }
double Base::topMargin()               { return m_topMargin; }
void   Base::setBottomMargin(double v) { m_bottomMargin = v; }
double Base::bottomMargin()            { return m_bottomMargin; }
void   Base::setLeftMargin(double v)   { m_leftMargin = v; }
double Base::leftMargin()              { return m_leftMargin; }
void   Base::setRightMargin(double v)  { m_rightMargin = v; }
double Base::rightMargin()             { return m_rightMargin; }

void
Base::setMargins(double v)
{
  m_topMargin = m_bottomMargin = m_leftMargin = m_rightMargin = v;
}

void   Base::setTitleMargin(double v)  { m_titleMargin = v; }
double Base::titleMargin()             { return m_titleMargin; }
void   Base::setLegendMargin(double v) { m_legendMargin = v; }
double Base::legendMargin()            { return m_legendMargin; }

void   Base::setCenterLabelsOverPoint(bool v) { m_centerLabelsOverPoint = v; }
bool   Base::centerLabelsOverPoint()          { return m_centerLabelsOverPoint; }
void   Base::setHasLeftLabels(bool v)         { m_hasLeftLabels = v; }
bool   Base::hasLeftLabels()                  { return m_hasLeftLabels; }

void   Base::setXAxisLabel(string v) { m_xAxisLabel = v; }
string Base::xAxisLabel()            { return m_xAxisLabel; }
void   Base::setYAxisLabel(string v) { m_yAxisLabel = v; }
string Base::yAxisLabel()            { return m_yAxisLabel; }
void   Base::setYAxisIncrement(double v) { m_yAxisIncrement = v; }
double Base::yAxisIncrement()            { return m_yAxisIncrement; }

void   Base::setTitle(string v) { m_title = v; }
string Base::title()            { return m_title; }

void   Base::setFont(string fontPath) { m_font = fontPath; }
string Base::font()                   { return m_font; }
void   Base::setFontColor(string v)   { m_fontColor = v; }
string Base::fontColor()              { return m_fontColor; }

void   Base::setHideLineMarkers(bool v) { m_hideLineMarkers = v; }
bool   Base::hideLineMarkers()          { return m_hideLineMarkers; }
void   Base::setHideLegend(bool v)      { m_hideLegend = v; }
bool   Base::hideLegend()               { return m_hideLegend; }
void   Base::setHideTitle(bool v)       { m_hideTitle = v; }
bool   Base::hideTitle()                { return m_hideTitle; }
void   Base::setHideLineNumbers(bool v) { m_hideLineNumbers = v; }
bool   Base::hideLineNumbers()          { return m_hideLineNumbers; }

void   Base::setNoDataMessage(string v) { m_noDataMessage = v; }
string Base::noDataMessage()            { return m_noDataMessage; }

void   Base::setTitleFontSize(double v)  { m_titleFontSize = v; }
double Base::titleFontSize()             { return m_titleFontSize; }
void   Base::setLegendFontSize(double v) { m_legendFontSize = v; }
double Base::legendFontSize()            { return m_legendFontSize; }
void   Base::setMarkerFontSize(double v) { m_markerFontSize = v; }
double Base::markerFontSize()            { return m_markerFontSize; }

void   Base::setMarkerColor(string v) { m_markerColor = v; }
string Base::markerColor()            { return m_markerColor; }
void   Base::setMarkerCount(int v)    { m_markerCount = v; }
int    Base::markerCount()            { return m_markerCount; }

void   Base::setMinimumValue(double v) { m_minimumValue = v; }
double Base::minimumValue()            { return m_minimumValue; }
void   Base::setMaximumValue(double v) { m_maximumValue = v; }
double Base::maximumValue()            { return m_maximumValue; }

void   Base::setSort(bool v) { m_sort = v; }
bool   Base::sort()          { return m_sort; }

void   Base::setLegendBoxSize(double v) { m_legendBoxSize = v; }
double Base::legendBoxSize()            { return m_legendBoxSize; }

//------------------------------------------------------------------------------
// output
//------------------------------------------------------------------------------

void
Base::write(string filename)
{
  draw();
  // Some MagickCore internals (config/exception handling) call setlocale()
  // on their own, so LC_NUMERIC has to be pinned again right before the MVG
  // primitive stream is serialized, not just once at construction time.
  std::setlocale(LC_NUMERIC, "C");
  MagickDrawImage(m_wand, m_draw);
  MagickWriteImage(m_wand, filename);
}

//------------------------------------------------------------------------------
// drawing lifecycle
//------------------------------------------------------------------------------

void
Base::draw()
{
  setupDrawing();
}

void
Base::setupDrawing()
{
  if (!m_hasData) {
    drawNoData();
    return;
  }

  normalize();
  setupGraphMeasurements();
  if (m_sort) sortNormData();

  drawLegend();
  drawLineMarkers();
  drawAxisLabels();
  drawTitle();
}

void
Base::normalize(bool force)
{
  if (m_normDataValid && !force) return;

  m_normData.clear();

  if (!m_hasData) {
    m_normDataValid = true;
    return;
  }

  calculateSpread();

  for (auto &row : m_data) {
    Dataset nd;
    nd.name  = row.name;
    nd.color = row.color;
    nd.values.reserve(row.values.size());

    for (double point : row.values) {
      if (std::isnan(point)) {
        nd.values.push_back(NoValue);
      } else {
        nd.values.push_back((point - m_minimumValue) / m_spread);
      }
    }
    m_normData.push_back(nd);
  }

  m_normDataValid = true;
}

void
Base::calculateSpread()
{
  m_spread = m_maximumValue - m_minimumValue;
  if (m_spread <= 0) m_spread = 1;
}

void
Base::setupGraphMeasurements()
{
  m_markerCapsHeight = m_hideLineMarkers ? 0 : calculateCapsHeight(m_markerFontSize);
  m_titleCapsHeight  = m_hideTitle       ? 0 : calculateCapsHeight(m_titleFontSize);
  m_legendCapsHeight = m_hideLegend      ? 0 : calculateCapsHeight(m_legendFontSize);

  if (m_hideLineMarkers) {
    m_graphLeft          = m_leftMargin;
    m_graphRightMargin   = m_rightMargin;
    m_graphBottomMargin  = m_bottomMargin;
  } else {
    double longestLeftLabelWidth = 0;

    if (m_hasLeftLabels) {
      string longest = "";
      for (auto &kv : m_labels) {
        if (kv.second.size() > longest.size()) longest = kv.second;
      }
      longestLeftLabelWidth = calculateWidth(m_markerFontSize, longest) * 1.25;
    } else {
      longestLeftLabelWidth = calculateWidth(m_markerFontSize, formatLabel(m_maximumValue));
    }

    double lineNumberWidth = (m_hideLineNumbers && !m_hasLeftLabels)
      ? 0.0
      : (longestLeftLabelWidth + LABEL_MARGIN * 2);

    m_graphLeft = m_leftMargin
      + lineNumberWidth
      + (m_yAxisLabel.empty() ? 0.0 : m_markerCapsHeight + LABEL_MARGIN * 2);

    int lastLabel = m_labels.empty() ? 0 : m_labels.rbegin()->first;
    string lastLabelText = m_labels.count(lastLabel) ? m_labels[lastLabel] : string("");

    double extraRoomForLongLabel =
      (lastLabel >= (m_columnCount - 1) && m_centerLabelsOverPoint)
      ? calculateWidth(m_markerFontSize, lastLabelText) / 2.0
      : 0;

    m_graphRightMargin  = m_rightMargin + extraRoomForLongLabel;
    m_graphBottomMargin = m_bottomMargin + m_markerCapsHeight + LABEL_MARGIN;
  }

  m_graphRight = m_rawColumns - m_graphRightMargin;
  m_graphWidth = m_rawColumns - m_graphLeft - m_graphRightMargin;

  m_graphTop = m_topMargin
    + (m_hideTitle  ? m_titleMargin  : m_titleCapsHeight  + m_titleMargin)
    + (m_hideLegend ? m_legendMargin : m_legendCapsHeight + m_legendMargin);

  double xAxisLabelHeight = m_xAxisLabel.empty() ? 0.0 : m_markerCapsHeight + LABEL_MARGIN;
  m_graphBottom = m_rawRows - m_graphBottomMargin - xAxisLabelHeight;
  m_graphHeight = m_graphBottom - m_graphTop;
}

void
Base::drawAxisLabels()
{
  if (!m_xAxisLabel.empty()) {
    double y = m_graphBottom + LABEL_MARGIN * 2 + m_markerCapsHeight;
    annotateText(m_rawColumns, 1.0, 0.0, y, m_xAxisLabel, Gravity::North, m_fontColor, m_markerFontSize);
  }

  if (!m_yAxisLabel.empty()) {
    double x = m_leftMargin + m_markerCapsHeight / 2.0;
    annotateText(1.0, m_rawRows, x, 0.0, m_yAxisLabel, Gravity::Center, m_fontColor, m_markerFontSize,
                 false, -90.0);
  }
}

void
Base::drawLineMarkers()
{
  if (m_hideLineMarkers) return;

  if (std::isnan(m_yAxisIncrement)) {
    if (m_markerCount < 0) {
      for (int lines = 3; lines <= 7; lines++) {
        if (std::fmod(m_spread, lines) == 0.0) {
          m_markerCount = lines;
          break;
        }
      }
      if (m_markerCount < 0) m_markerCount = 4;
    }
    m_increment = (m_spread > 0) ? significant(m_spread / m_markerCount) : 1;
  } else {
    m_maximumValue = std::max(std::ceil(m_maximumValue), m_yAxisIncrement);
    m_minimumValue = std::floor(m_minimumValue);
    calculateSpread();
    normalize(true);

    m_markerCount = (int) (m_spread / m_yAxisIncrement);
    m_increment   = m_yAxisIncrement;
  }
  m_incrementScaled = m_graphHeight / (m_spread / m_increment);

  for (int index = 0; index <= m_markerCount; index++) {
    double y = m_graphTop + m_graphHeight - index * m_incrementScaled;

    setStrokeColor(m_markerColor);
    drawLineShape(m_graphLeft, y, m_graphRight, y);

    double markerLabel = index * m_increment + m_minimumValue;

    if (!m_hideLineNumbers) {
      annotateText(m_graphLeft - LABEL_MARGIN, 1.0, 0.0, y, formatLabel(markerLabel),
                   Gravity::East, m_fontColor, m_markerFontSize);
    }
  }
}

void
Base::drawLegend()
{
  if (m_hideLegend) return;

  std::vector<string> legendLabels;
  for (auto &row : m_data) legendLabels.push_back(row.name);

  double legendSquareWidth = m_legendBoxSize;

  std::vector<std::vector<double>> labelWidths(1);

  for (auto &lbl : legendLabels) {
    double w, h;
    measureText(lbl, m_legendFontSize, false, w, h);
    double labelWidth = w + legendSquareWidth * 2.7;
    labelWidths.back().push_back(labelWidth);

    double sum = 0;
    for (double v : labelWidths.back()) sum += v;

    if (sum > (m_rawColumns * 0.9)) {
      double last = labelWidths.back().back();
      labelWidths.back().pop_back();
      labelWidths.push_back({ last });
    }
  }

  auto sumOf = [](const std::vector<double> &v) {
    double s = 0;
    for (double x : v) s += x;
    return s;
  };

  double currentXOffset = (m_rawColumns - sumOf(labelWidths.front())) / 2.0;
  double currentYOffset = m_hideTitle
    ? m_topMargin + m_titleMargin
    : m_topMargin + m_titleMargin + m_titleCapsHeight;

  size_t widthsRow = 0;

  for (size_t index = 0; index < legendLabels.size(); index++) {
    const string &color = m_data[index].color;

    annotateText(m_rawColumns, 1.0,
                 currentXOffset + (legendSquareWidth * 1.7), currentYOffset,
                 legendLabels[index], Gravity::West, m_fontColor, m_legendFontSize);

    setStrokeColor("transparent");
    setFillColor(color);
    drawRectangleShape(currentXOffset, currentYOffset - legendSquareWidth / 2.0,
                        currentXOffset + legendSquareWidth, currentYOffset + legendSquareWidth / 2.0);

    double w, h;
    measureText(legendLabels[index], m_legendFontSize, false, w, h);
    double currentStringOffset = w + (legendSquareWidth * 2.7);

    labelWidths[widthsRow].erase(labelWidths[widthsRow].begin());

    if (labelWidths[widthsRow].empty()) {
      widthsRow++;
      if (widthsRow < labelWidths.size()) {
        currentXOffset = (m_rawColumns - sumOf(labelWidths[widthsRow])) / 2.0;
        double lineHeight = std::max(m_legendCapsHeight, legendSquareWidth) + m_legendMargin;
        currentYOffset += lineHeight;
        m_graphTop    += lineHeight;
        m_graphHeight  = m_graphBottom - m_graphTop;
      }
    } else {
      currentXOffset += currentStringOffset;
    }
  }

  m_colorIndex = 0;
}

void
Base::drawTitle()
{
  if (m_hideTitle || m_title.empty()) return;

  annotateText(m_rawColumns, 1.0, 0, m_topMargin, m_title, Gravity::North, m_fontColor,
               m_titleFontSize, true);
}

void
Base::drawLabel(double xOffset, int index)
{
  if (m_hideLineMarkers) return;

  if (m_labels.count(index) && !m_labelsSeen.count(index)) {
    double yOffset = m_graphBottom + LABEL_MARGIN;

    annotateText(1.0, 1.0, xOffset, yOffset, m_labels[index], Gravity::North, m_fontColor,
                 m_markerFontSize);

    m_labelsSeen[index] = true;
  }
}

void
Base::drawNoData()
{
  annotateText(m_rawColumns, m_rawRows / 2.0, 0, 10, m_noDataMessage, Gravity::Center, m_fontColor, 80.0);
}

//------------------------------------------------------------------------------
// background / themes
//------------------------------------------------------------------------------

void
Base::renderBackground()
{
  if (m_wand) m_wand = DestroyMagickWand(m_wand);

  int width  = (int) m_columns;
  int height = (int) m_rows;

  if (m_themeOptions.backgroundColors.size() >= 2) {
    m_wand = NewMagickWand();
    MagickSetSize(m_wand, width, height);
    char spec[512];
    std::snprintf(spec, sizeof(spec), "gradient:%s-%s",
                  (const char *) m_themeOptions.backgroundColors[0],
                  (const char *) m_themeOptions.backgroundColors[1]);
    MagickReadImage(m_wand, spec);
  } else {
    string color = m_themeOptions.backgroundColors.empty()
      ? string("transparent")
      : m_themeOptions.backgroundColors[0];

    m_wand = NewMagickWand();
    PixelWand *pw = NewPixelWand();
    PixelSetColor(pw, color);
    MagickNewImage(m_wand, width, height, pw);
    MagickSetImageAlphaChannel(m_wand, ActivateAlphaChannel);
    DestroyPixelWand(pw);
  }
}

void
Base::resetThemes()
{
  m_colorIndex = 0;
  m_labelsSeen.clear();
  m_themeOptions = Theme();

  if (m_draw) m_draw = DestroyDrawingWand(m_draw);
  m_draw = NewDrawingWand();
}

//------------------------------------------------------------------------------
// helpers
//------------------------------------------------------------------------------

double
Base::scaleFontSize(double value)
{
  return value * m_scaleRatio;
}

double
Base::significant(double inc)
{
  if (inc == 0) return 1.0;

  double factor = 1.0;
  while (inc < 10) {
    inc *= 10;
    factor /= 10;
  }
  while (inc > 100) {
    inc /= 10;
    factor *= 10;
  }

  return std::floor(inc) * factor;
}

void
Base::sortNormData()
{
  std::sort(m_normData.begin(), m_normData.end(), [](const Dataset &a, const Dataset &b) {
    return sumValues(b.values) < sumValues(a.values);
  });
}

double
Base::sumValues(const std::vector<double> &values)
{
  double total = 0;
  for (double v : values) total += std::isnan(v) ? 0.0 : v;
  return total;
}

string
Base::formatLabel(double value)
{
  char buffer[64];
  string label;

  bool useInteger = (std::fmod(m_spread, (double) m_markerCount) == 0.0) || !std::isnan(m_yAxisIncrement);

  if (useInteger) {
    std::snprintf(buffer, sizeof(buffer), "%lld", (long long) value);
    label = buffer;
  } else if (m_spread > 10.0) {
    std::snprintf(buffer, sizeof(buffer), "%lld", (long long) std::llround(value));
    label = buffer;
  } else if (m_spread >= 3.0) {
    std::snprintf(buffer, sizeof(buffer), "%0.2f", value);
    label = buffer;
  } else {
    std::snprintf(buffer, sizeof(buffer), "%g", value);
    label = buffer;
  }

  std::string raw = (const char *) label;
  std::string::size_type dot = raw.find('.');
  std::string intPart  = (dot == std::string::npos) ? raw : raw.substr(0, dot);
  std::string fracPart = (dot == std::string::npos) ? "" : raw.substr(dot);

  bool negative = !intPart.empty() && intPart[0] == '-';
  std::string digits = negative ? intPart.substr(1) : intPart;

  std::string withSeparators;
  for (size_t i = 0; i < digits.size(); i++) {
    if (i > 0 && (digits.size() - i) % 3 == 0) withSeparators += ',';
    withSeparators += digits[i];
  }

  return string(((negative ? "-" : "") + withSeparators + fracPart).c_str());
}

double
Base::calculateCapsHeight(double fontSize)
{
  double w, h;
  measureText("X", fontSize, false, w, h);
  return h;
}

double
Base::calculateWidth(double fontSize, const string &text)
{
  double w, h;
  measureText(text, fontSize, false, w, h);
  return w;
}

string
Base::incrementColor()
{
  if (m_colors.empty()) return string("");

  m_colorIndex = (m_colorIndex + 1) % m_colors.size();
  size_t idx = (m_colorIndex == 0) ? m_colors.size() - 1 : m_colorIndex - 1;
  return m_colors[idx];
}

bool
Base::largerThanMax(double dataPoint)
{
  return dataPoint > m_maximumValue;
}

bool
Base::lessThanMin(double dataPoint)
{
  return dataPoint < m_minimumValue;
}

double
Base::clipValueIfGreaterThan(double value, double maxValue)
{
  return (value > maxValue) ? maxValue : value;
}

//------------------------------------------------------------------------------
// low level drawing primitives
//------------------------------------------------------------------------------

void
Base::setFillColor(const string &color)
{
  PixelWand *pw = NewPixelWand();
  PixelSetColor(pw, color);
  DrawSetFillColor(m_draw, pw);
  DestroyPixelWand(pw);
}

void
Base::setStrokeColor(const string &color)
{
  PixelWand *pw = NewPixelWand();
  PixelSetColor(pw, color);
  DrawSetStrokeColor(m_draw, pw);
  DestroyPixelWand(pw);
}

void
Base::setStrokeOpacity(double opacity)
{
  DrawSetStrokeOpacity(m_draw, opacity);
}

void
Base::setFillOpacity(double opacity)
{
  DrawSetFillOpacity(m_draw, opacity);
}

void
Base::setStrokeWidth(double width)
{
  DrawSetStrokeWidth(m_draw, width * m_scaleRatio);
}

void
Base::drawRectangleShape(double x1, double y1, double x2, double y2)
{
  double s = m_scaleRatio;
  DrawRectangle(m_draw, x1 * s, y1 * s, x2 * s, y2 * s);
}

void
Base::drawLineShape(double x1, double y1, double x2, double y2)
{
  double s = m_scaleRatio;
  DrawLine(m_draw, x1 * s, y1 * s, x2 * s, y2 * s);
}

void
Base::drawCircleShape(double ox, double oy, double px, double py)
{
  double s = m_scaleRatio;
  DrawCircle(m_draw, ox * s, oy * s, px * s, py * s);
}

void
Base::drawEllipseShape(double cx, double cy, double rx, double ry, double startDeg, double endDeg)
{
  double s = m_scaleRatio;
  DrawEllipse(m_draw, cx * s, cy * s, rx * s, ry * s, startDeg, endDeg);
}

void
Base::drawPolygonShape(const std::vector<double> &points)
{
  double s = m_scaleRatio;
  size_t n = points.size() / 2;
  std::vector<PointInfo> pts(n);
  for (size_t i = 0; i < n; i++) {
    pts[i].x = points[i * 2]     * s;
    pts[i].y = points[i * 2 + 1] * s;
  }
  DrawPolygon(m_draw, n, pts.data());
}

void
Base::measureText(const string &text, double rawPointSize, bool bold, double &width, double &height)
{
  if (!m_font.empty()) DrawSetFont(m_draw, m_font);
  DrawSetFontSize(m_draw, rawPointSize);
  DrawSetFontWeight(m_draw, bold ? 700 : 400);

  double *metrics = MagickQueryFontMetrics(m_wand, m_draw, text);
  width  = metrics ? metrics[4] : 0.0;
  height = metrics ? metrics[5] : 0.0;
  if (metrics) MagickRelinquishMemory(metrics);
}

void
Base::annotateText(double width, double height, double x, double y,
                     const string &text, Gravity gravity, const string &fillColor,
                     double rawPointSize, bool bold, double angle)
{
  double s = m_scaleRatio;
  double pointSize = scaleFontSize(rawPointSize);

  setFillColor(fillColor);
  setStrokeColor("transparent");
  if (!m_font.empty()) DrawSetFont(m_draw, m_font);
  DrawSetFontSize(m_draw, pointSize);
  DrawSetFontWeight(m_draw, bold ? 700 : 400);

  double *metrics = MagickQueryFontMetrics(m_wand, m_draw, text);
  double textWidth  = metrics ? metrics[4] : 0.0;
  double textHeight = metrics ? metrics[5] : 0.0;
  double ascender   = metrics ? metrics[2] : 0.0;
  if (metrics) MagickRelinquishMemory(metrics);

  double boxLeft = x * s, boxTop = y * s, boxWidth = width * s, boxHeight = height * s;

  double drawX;
  switch (gravity) {
    case Gravity::West:  drawX = boxLeft; break;
    case Gravity::East:  drawX = boxLeft + boxWidth - textWidth; break;
    default:              drawX = boxLeft + (boxWidth - textWidth) / 2.0; break;
  }

  double baselineY = (gravity == Gravity::North)
    ? boxTop + ascender
    : boxTop + boxHeight / 2.0 - textHeight / 2.0 + ascender;

  if (angle != 0.0) {
    PushDrawingWand(m_draw);
    DrawTranslate(m_draw, drawX, baselineY);
    DrawRotate(m_draw, angle);
    DrawAnnotation(m_draw, 0, 0, ustr(text));
    PopDrawingWand(m_draw);
  } else {
    DrawAnnotation(m_draw, drawX, baselineY, ustr(text));
  }
}

} // namespace chart
} // namespace graphics
} // namespace arken
