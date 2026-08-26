// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "text.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace arken { namespace notifygl {

namespace {

const uint32_t kFirstCodepoint = 0x20;
const uint32_t kLastCodepoint  = 0xFF;
const int      kGlyphCount     = static_cast<int>(kLastCodepoint - kFirstCodepoint + 1);
const int      kAtlasColumns   = 16;
const int      kAtlasPadding   = 1; // pixels de margem em cada lado da célula

} // namespace

uint32_t decodeUtf8(const std::string & text, size_t & i) {
  unsigned char c = static_cast<unsigned char>(text[i]);

  int      extraBytes = 0;
  uint32_t codepoint   = 0;

  if ((c & 0x80) == 0x00) {
    codepoint = c;
    extraBytes = 0;
  } else if ((c & 0xE0) == 0xC0) {
    codepoint = c & 0x1F;
    extraBytes = 1;
  } else if ((c & 0xF0) == 0xE0) {
    codepoint = c & 0x0F;
    extraBytes = 2;
  } else if ((c & 0xF8) == 0xF0) {
    codepoint = c & 0x07;
    extraBytes = 3;
  } else {
    // Byte de continuação isolado ou sequência inválida: consome 1 byte.
    i += 1;
    return c;
  }

  size_t next = i + 1;
  for (int n = 0; n < extraBytes; ++n) {
    if (next >= text.size() || (static_cast<unsigned char>(text[next]) & 0xC0) != 0x80) {
      i += 1;
      return c;
    }
    codepoint = (codepoint << 6) | (static_cast<unsigned char>(text[next]) & 0x3F);
    next += 1;
  }

  i = next;
  return codepoint;
}

Font::~Font() {
  if (m_texture != 0) {
    glDeleteTextures(1, &m_texture);
  }
}

bool Font::load(const std::string & fontPath, int pixelSize) {
  if (fontPath.empty()) {
    std::cerr << "arken::notify (opengl): no font path provided" << std::endl;
    return false;
  }

  FT_Library ft;
  if (FT_Init_FreeType(&ft)) {
    std::cerr << "arken::notify (opengl): FT_Init_FreeType failed" << std::endl;
    return false;
  }

  FT_Face face;
  if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
    std::cerr << "arken::notify (opengl): FT_New_Face failed for " << fontPath << std::endl;
    FT_Done_FreeType(ft);
    return false;
  }

  FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(pixelSize));

  m_lineHeight = face->size->metrics.height / 64.0f;
  m_ascent     = face->size->metrics.ascender / 64.0f;

  // 1a passada: descobre o tamanho máximo de glifo para dimensionar a grade.
  int maxGlyphWidth  = 1;
  int maxGlyphHeight = 1;
  for (uint32_t cp = kFirstCodepoint; cp <= kLastCodepoint; ++cp) {
    if (FT_Load_Char(face, cp, FT_LOAD_RENDER)) {
      continue;
    }
    maxGlyphWidth  = std::max(maxGlyphWidth,  static_cast<int>(face->glyph->bitmap.width));
    maxGlyphHeight = std::max(maxGlyphHeight, static_cast<int>(face->glyph->bitmap.rows));
  }

  int cellWidth  = maxGlyphWidth  + kAtlasPadding * 2;
  int cellHeight = maxGlyphHeight + kAtlasPadding * 2;
  int rows       = (kGlyphCount + kAtlasColumns - 1) / kAtlasColumns;

  m_atlasWidth  = cellWidth  * kAtlasColumns;
  m_atlasHeight = cellHeight * rows;

  std::vector<unsigned char> buffer(static_cast<size_t>(m_atlasWidth) * m_atlasHeight, 0);

  // 2a passada: renderiza cada glifo na célula correspondente e registra métricas.
  for (int idx = 0; idx < kGlyphCount; ++idx) {
    uint32_t cp = kFirstCodepoint + static_cast<uint32_t>(idx);
    if (FT_Load_Char(face, cp, FT_LOAD_RENDER)) {
      continue;
    }

    FT_GlyphSlot slot = face->glyph;
    int col = idx % kAtlasColumns;
    int row = idx / kAtlasColumns;
    int originX = col * cellWidth  + kAtlasPadding;
    int originY = row * cellHeight + kAtlasPadding;

    const FT_Bitmap & bitmap = slot->bitmap;
    for (unsigned int y = 0; y < bitmap.rows; ++y) {
      const unsigned char * srcRow = bitmap.buffer + y * static_cast<unsigned int>(std::abs(bitmap.pitch));
      unsigned char * dstRow = buffer.data() +
        static_cast<size_t>(originY + static_cast<int>(y)) * m_atlasWidth + originX;
      std::copy(srcRow, srcRow + bitmap.width, dstRow);
    }

    Glyph glyph;
    glyph.advance  = slot->advance.x / 64.0f;
    glyph.width    = static_cast<float>(bitmap.width);
    glyph.height   = static_cast<float>(bitmap.rows);
    glyph.bearingX = static_cast<float>(slot->bitmap_left);
    glyph.bearingY = static_cast<float>(slot->bitmap_top);
    glyph.u0 = static_cast<float>(originX) / static_cast<float>(m_atlasWidth);
    glyph.v0 = static_cast<float>(originY) / static_cast<float>(m_atlasHeight);
    glyph.u1 = static_cast<float>(originX + static_cast<int>(bitmap.width))  / static_cast<float>(m_atlasWidth);
    glyph.v1 = static_cast<float>(originY + static_cast<int>(bitmap.rows))   / static_cast<float>(m_atlasHeight);

    m_glyphs[cp] = glyph;
  }

  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_ALPHA, m_atlasWidth, m_atlasHeight, 0,
    GL_ALPHA, GL_UNSIGNED_BYTE, buffer.data()
  );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);

  m_ready = true;
  return true;
}

float Font::measureWidth(const std::string & utf8Text) const {
  if (!m_ready) {
    return 0.0f;
  }

  float width = 0.0f;
  size_t i = 0;
  while (i < utf8Text.size()) {
    uint32_t cp = decodeUtf8(utf8Text, i);
    auto it = m_glyphs.find(cp);
    if (it != m_glyphs.end()) {
      width += it->second.advance;
    }
  }
  return width;
}

std::vector<std::string> Font::wrap(const std::string & utf8Text, float maxWidth) const {
  std::vector<std::string> lines;
  if (!m_ready) {
    lines.push_back(utf8Text);
    return lines;
  }

  // Tokeniza em palavras separadas por espaço, tratando '\n' como token de
  // quebra forçada isolado.
  std::vector<std::string> tokens;
  std::string word;
  for (size_t i = 0; i < utf8Text.size();) {
    size_t start = i;
    uint32_t cp = decodeUtf8(utf8Text, i);
    if (cp == '\n') {
      if (!word.empty()) { tokens.push_back(word); word.clear(); }
      tokens.push_back("\n");
    } else if (cp == ' ') {
      if (!word.empty()) { tokens.push_back(word); word.clear(); }
    } else {
      word.append(utf8Text, start, i - start);
    }
  }
  if (!word.empty()) {
    tokens.push_back(word);
  }

  std::string currentLine;
  float       currentWidth = 0.0f;
  float       spaceWidth   = measureWidth(" ");

  for (const std::string & token : tokens) {
    if (token == "\n") {
      lines.push_back(currentLine);
      currentLine.clear();
      currentWidth = 0.0f;
      continue;
    }

    float tokenWidth = measureWidth(token);
    float extra = currentLine.empty() ? tokenWidth : (spaceWidth + tokenWidth);

    if (!currentLine.empty() && currentWidth + extra > maxWidth) {
      lines.push_back(currentLine);
      currentLine  = token;
      currentWidth = tokenWidth;
    } else {
      if (!currentLine.empty()) {
        currentLine += " ";
        currentWidth += spaceWidth;
      }
      currentLine += token;
      currentWidth += tokenWidth;
    }
  }
  lines.push_back(currentLine);

  return lines;
}

void Font::draw(const std::string & utf8Text, float x, float y, const float color[4]) const {
  if (!m_ready) {
    return;
  }

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, m_texture);
  glColor4fv(color);

  float penX = x;
  float penY = y + m_ascent;

  glBegin(GL_QUADS);
  size_t i = 0;
  while (i < utf8Text.size()) {
    uint32_t cp = decodeUtf8(utf8Text, i);
    if (cp == '\n') {
      penX = x;
      penY += m_lineHeight;
      continue;
    }

    auto it = m_glyphs.find(cp);
    if (it == m_glyphs.end()) {
      continue;
    }
    const Glyph & glyph = it->second;

    float left   = penX + glyph.bearingX;
    float top    = penY - glyph.bearingY;
    float right  = left + glyph.width;
    float bottom = top  + glyph.height;

    glTexCoord2f(glyph.u0, glyph.v0); glVertex2f(left,  top);
    glTexCoord2f(glyph.u1, glyph.v0); glVertex2f(right, top);
    glTexCoord2f(glyph.u1, glyph.v1); glVertex2f(right, bottom);
    glTexCoord2f(glyph.u0, glyph.v1); glVertex2f(left,  bottom);

    penX += glyph.advance;
  }
  glEnd();

  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
}

}} // namespace arken::notifygl
