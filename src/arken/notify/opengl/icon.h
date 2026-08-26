// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef ARKEN_NOTIFY_OPENGL_ICON_H
#define ARKEN_NOTIFY_OPENGL_ICON_H

#include <string>

namespace arken { namespace notifygl {

enum class IconKind { Ok, Warning, Error };

// "ok" -> Ok (verde), "warning" -> Warning (amarelo),
// "error"/"failure" -> Error (vermelho), qualquer outro valor -> Ok
// (mesmo default de arken::notify::send, icon = "ok").
IconKind resolveIconKind(const std::string & icon);

// Desenha um círculo preenchido colorido por status, centrado em (cx, cy),
// com o raio dado, mais um símbolo branco simples por cima (✓ / ! / X).
void drawIcon(IconKind kind, float cx, float cy, float radius);

}} // namespace arken::notifygl

#endif // ARKEN_NOTIFY_OPENGL_ICON_H
