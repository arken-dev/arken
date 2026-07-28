// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Implementação de plataforma para Windows. Reforça always-on-top via
// SetWindowPos(HWND_TOPMOST) e localiza a fonte padrão do sistema em
// %WINDIR%\Fonts. Não é possível compilar/testar este arquivo no ambiente
// Linux atual; segue estritamente a documentação da Win32 API.

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <windows.h>

#include <sys/stat.h>

#include "platform.h"

namespace arken { namespace notifygl {

static bool fileExists(const std::string & path) {
  struct stat st;
  return stat(path.c_str(), &st) == 0;
}

void platformAlwaysOnTop(GLFWwindow * window) {
  HWND hwnd = glfwGetWin32Window(window);
  if (hwnd == nullptr) {
    return;
  }
  SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
}

void platformDisableFocus(GLFWwindow * window) {
  HWND hwnd = glfwGetWin32Window(window);
  if (hwnd == nullptr) {
    return;
  }
  // WS_EX_NOACTIVATE: a janela nunca é ativada (nem por clique, nem por
  // Alt+Tab), então nunca rouba o foco de teclado do programa em uso.
  // Cliques do mouse continuam sendo entregues normalmente.
  LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
  SetWindowLongPtr(hwnd, GWL_EXSTYLE, exStyle | WS_EX_NOACTIVATE);
}

std::string platformFindFont() {
  char windir[MAX_PATH];
  if (GetWindowsDirectoryA(windir, MAX_PATH) != 0) {
    std::string path = std::string(windir) + "\\Fonts\\segoeui.ttf";
    if (fileExists(path)) {
      return path;
    }
  }

  const char * fallback = "C:\\Windows\\Fonts\\arial.ttf";
  return fileExists(fallback) ? fallback : "";
}

}} // namespace arken::notifygl
