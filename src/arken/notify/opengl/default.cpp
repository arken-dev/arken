// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Implementação de plataforma para Linux e demais sistemas Unix-like que
// não sejam macOS. Reforça always-on-top via EWMH (_NET_WM_STATE_ABOVE) e
// localiza a fonte padrão do sistema via fontconfig.

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <fontconfig/fontconfig.h>

#include "platform.h"

namespace arken { namespace notifygl {

static void sendNetWmStateAbove(Display * display, Window window) {
  Atom wmState      = XInternAtom(display, "_NET_WM_STATE", False);
  Atom wmStateAbove = XInternAtom(display, "_NET_WM_STATE_ABOVE", False);

  XEvent event = {};
  event.type                 = ClientMessage;
  event.xclient.window       = window;
  event.xclient.message_type = wmState;
  event.xclient.format       = 32;
  event.xclient.data.l[0]    = 1; // _NET_WM_STATE_ADD
  event.xclient.data.l[1]    = static_cast<long>(wmStateAbove);
  event.xclient.data.l[2]    = 0;
  event.xclient.data.l[3]    = 1;

  XSendEvent(
    display,
    DefaultRootWindow(display),
    False,
    SubstructureRedirectMask | SubstructureNotifyMask,
    &event
  );
  XFlush(display);
}

void platformAlwaysOnTop(GLFWwindow * window) {
  Display * display = glfwGetX11Display();
  Window    xwindow  = glfwGetX11Window(window);
  if (display == nullptr || xwindow == 0) {
    return;
  }
  sendNetWmStateAbove(display, xwindow);
}

void platformDisableFocus(GLFWwindow * window) {
  Display * display = glfwGetX11Display();
  Window    xwindow  = glfwGetX11Window(window);
  if (display == nullptr || xwindow == 0) {
    return;
  }

  // WM_HINTS.input = False (ICCCM): o X server recusa dar foco de teclado
  // a esta janela, mesmo que o WM tente. Cliques do mouse continuam
  // funcionando normalmente, pois não dependem de foco.
  XWMHints hints = {};
  hints.flags = InputHint;
  hints.input = False;
  XSetWMHints(display, xwindow, &hints);

  // Sinaliza ao WM que é uma janela utilitária (sem entrada na taskbar,
  // sem intenção de ser ativada), reforço adicional além do WM_HINTS.
  Atom windowType    = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
  Atom typeUtility   = XInternAtom(display, "_NET_WM_WINDOW_TYPE_UTILITY", False);
  XChangeProperty(
    display, xwindow, windowType, XA_ATOM, 32, PropModeReplace,
    reinterpret_cast<unsigned char *>(&typeUtility), 1
  );
}

std::string platformFindFont() {
  if (!FcInit()) {
    return "";
  }

  FcPattern * pattern = FcNameParse(reinterpret_cast<const FcChar8 *>("sans-serif"));
  FcConfigSubstitute(nullptr, pattern, FcMatchPattern);
  FcDefaultSubstitute(pattern);

  FcResult fcResult;
  FcPattern * match = FcFontMatch(nullptr, pattern, &fcResult);

  std::string result;
  if (match != nullptr) {
    FcChar8 * file = nullptr;
    if (FcPatternGetString(match, FC_FILE, 0, &file) == FcResultMatch) {
      result = reinterpret_cast<char *>(file);
    }
    FcPatternDestroy(match);
  }
  FcPatternDestroy(pattern);

  return result;
}

}} // namespace arken::notifygl
