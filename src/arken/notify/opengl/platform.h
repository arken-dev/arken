// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef ARKEN_NOTIFY_OPENGL_PLATFORM_H
#define ARKEN_NOTIFY_OPENGL_PLATFORM_H

#include <string>

struct GLFWwindow;

namespace arken { namespace notifygl {

// Reforça always-on-top a nível de sistema operacional, além do hint
// GLFW_FLOATING já aplicado na criação da janela. Chamada tanto na criação
// quanto toda vez que a janela é reexibida, porque alguns window managers
// esquecem o estado "above all" de forma intermitente.
void platformAlwaysOnTop(GLFWwindow * window);

// Retorna o caminho absoluto de uma fonte TrueType/OpenType do sistema
// adequada para UI, ou string vazia se nenhuma foi encontrada.
std::string platformFindFont();

// Marca a janela como incapaz de receber foco de teclado, para que ela
// apareça por cima sem nunca roubar o foco do programa que o
// desenvolvedor está usando. Cliques continuam funcionando normalmente:
// eventos de botão do mouse não dependem de foco de teclado. Chamada uma
// única vez, logo após a criação da janela.
void platformDisableFocus(GLFWwindow * window);

}} // namespace arken::notifygl

#endif // ARKEN_NOTIFY_OPENGL_PLATFORM_H
