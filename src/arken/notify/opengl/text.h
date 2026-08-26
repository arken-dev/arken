// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef ARKEN_NOTIFY_OPENGL_TEXT_H
#define ARKEN_NOTIFY_OPENGL_TEXT_H

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

namespace arken { namespace notifygl {

struct Glyph {
  float advance   = 0.0f;
  float width     = 0.0f;
  float height    = 0.0f;
  float bearingX  = 0.0f;
  float bearingY  = 0.0f;
  float u0 = 0.0f, v0 = 0.0f, u1 = 0.0f, v1 = 0.0f;
};

// Renderiza texto via atlas de glifos gerado pelo FreeType. Cobre a faixa
// de codepoints 0x20-0xFF (ASCII + Latin-1 Supplement), suficiente para
// texto em português com acentuação (ã, ç, é, í, ó, õ, ú, ...).
class Font {
  public:
  Font()  = default;
  ~Font();

  Font(const Font &)             = delete;
  Font & operator=(const Font &) = delete;

  // Carrega a fonte em fontPath no tamanho pixelSize e constrói o atlas de
  // glifos. Retorna false em qualquer falha (caminho vazio, erro do
  // FreeType); a instância permanece "not ready" e draw()/measureWidth()
  // viram no-ops seguros, sem abortar o processo.
  bool load(const std::string & fontPath, int pixelSize);

  bool  ready() const { return m_ready; }
  float lineHeight() const { return m_lineHeight; }

  float measureWidth(const std::string & utf8Text) const;

  // Quebra utf8Text em linhas que cabem em maxWidth pixels, medindo a
  // largura real dos glifos (não contagem de caracteres). Respeita '\n'
  // como quebra forçada. Uma palavra isolada maior que maxWidth fica
  // sozinha na linha, sem partir no meio.
  std::vector<std::string> wrap(const std::string & utf8Text, float maxWidth) const;

  void draw(const std::string & utf8Text, float x, float y, const float color[4]) const;

  private:
  unsigned int                         m_texture    = 0;
  int                                  m_atlasWidth  = 0;
  int                                  m_atlasHeight = 0;
  float                                m_lineHeight  = 0.0f;
  float                                m_ascent      = 0.0f;
  bool                                 m_ready       = false;
  std::unordered_map<uint32_t, Glyph>  m_glyphs;
};

// Decodifica um codepoint UTF-8 a partir de text[i], avançando i para o
// início do próximo codepoint. Sequências inválidas avançam 1 byte e
// retornam o próprio byte como codepoint (degradação segura).
uint32_t decodeUtf8(const std::string & text, size_t & i);

}} // namespace arken::notifygl

#endif // ARKEN_NOTIFY_OPENGL_TEXT_H
