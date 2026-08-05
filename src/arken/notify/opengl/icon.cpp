// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "icon.h"

#include <cmath>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace arken { namespace notifygl {

namespace {

const float kColorOk[3]      = {0.298f, 0.686f, 0.314f}; // #4CAF50
const float kColorWarning[3] = {1.000f, 0.757f, 0.027f}; // #FFC107
const float kColorError[3]   = {0.957f, 0.263f, 0.212f}; // #F44336
const float kColorSymbol[3]  = {1.0f, 1.0f, 1.0f};
const int   kCircleSegments  = 48;

void drawFilledCircle(float cx, float cy, float radius, const float color[3]) {
  glColor3fv(color);
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(cx, cy);
  for (int i = 0; i <= kCircleSegments; ++i) {
    float angle = 2.0f * 3.14159265f * static_cast<float>(i) / static_cast<float>(kCircleSegments);
    glVertex2f(cx + radius * std::cos(angle), cy + radius * std::sin(angle));
  }
  glEnd();
}

// Desenha um segmento de reta espesso (x0,y0)-(x1,y1), usado para compor os
// símbolos vetoriais sem depender de fonte.
void drawThickLine(float x0, float y0, float x1, float y1, float thickness) {
  float dx = x1 - x0;
  float dy = y1 - y0;
  float length = std::sqrt(dx * dx + dy * dy);
  if (length < 0.0001f) {
    return;
  }

  float nx = -dy / length * (thickness * 0.5f);
  float ny =  dx / length * (thickness * 0.5f);

  glBegin(GL_QUADS);
  glVertex2f(x0 + nx, y0 + ny);
  glVertex2f(x1 + nx, y1 + ny);
  glVertex2f(x1 - nx, y1 - ny);
  glVertex2f(x0 - nx, y0 - ny);
  glEnd();
}

void drawCheckmark(float cx, float cy, float radius) {
  float thickness = radius * 0.28f;
  glColor3fv(kColorSymbol);
  drawThickLine(cx - radius * 0.45f, cy + radius * 0.05f, cx - radius * 0.10f, cy + radius * 0.40f, thickness);
  drawThickLine(cx - radius * 0.10f, cy + radius * 0.40f, cx + radius * 0.50f, cy - radius * 0.35f, thickness);
}

void drawExclamation(float cx, float cy, float radius) {
  float thickness = radius * 0.28f;
  glColor3fv(kColorSymbol);
  drawThickLine(cx, cy - radius * 0.45f, cx, cy + radius * 0.12f, thickness);
  drawFilledCircle(cx, cy + radius * 0.42f, thickness * 0.55f, kColorSymbol);
}

void drawCross(float cx, float cy, float radius) {
  float thickness = radius * 0.28f;
  glColor3fv(kColorSymbol);
  drawThickLine(cx - radius * 0.42f, cy - radius * 0.42f, cx + radius * 0.42f, cy + radius * 0.42f, thickness);
  drawThickLine(cx - radius * 0.42f, cy + radius * 0.42f, cx + radius * 0.42f, cy - radius * 0.42f, thickness);
}

} // namespace

IconKind resolveIconKind(const std::string & icon) {
  if (icon == "warning") {
    return IconKind::Warning;
  }
  if (icon == "error" || icon == "failure") {
    return IconKind::Error;
  }
  return IconKind::Ok;
}

void drawIcon(IconKind kind, float cx, float cy, float radius) {
  switch (kind) {
    case IconKind::Warning:
      drawFilledCircle(cx, cy, radius, kColorWarning);
      drawExclamation(cx, cy, radius);
      break;
    case IconKind::Error:
      drawFilledCircle(cx, cy, radius, kColorError);
      drawCross(cx, cy, radius);
      break;
    case IconKind::Ok:
    default:
      drawFilledCircle(cx, cy, radius, kColorOk);
      drawCheckmark(cx, cy, radius);
      break;
  }
}

}} // namespace arken::notifygl
