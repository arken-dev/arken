// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.
//
// Implementação de plataforma para macOS. Reforça always-on-top via
// NSWindow level/collectionBehavior e localiza uma fonte do sistema em
// caminhos conhecidos. Compilado como Objective-C++ (ver cmake/arken-notify.cmake).
// Não é possível compilar/testar este arquivo no ambiente Linux atual;
// segue estritamente a documentação da Cocoa API.
//
// NOTA: chamadas ao GLFW/AppKit idealmente rodam na main thread da
// aplicação. Este backend as executa a partir da thread dedicada de
// Dialog::loop() para manter a arquitetura uniforme com Linux/Windows;
// setters simples de propriedade do NSWindow (setLevel/setCollectionBehavior)
// tendem a tolerar isso na prática, mas é um risco conhecido e documentado.

#define GLFW_EXPOSE_NATIVE_COCOA
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#import <Cocoa/Cocoa.h>

#include <sys/stat.h>

#include "platform.h"

namespace arken { namespace notifygl {

static bool fileExists(const std::string & path) {
  struct stat st;
  return stat(path.c_str(), &st) == 0;
}

void platformAlwaysOnTop(GLFWwindow * window) {
  NSWindow * nsWindow = glfwGetCocoaWindow(window);
  if (nsWindow == nil) {
    return;
  }
  [nsWindow setLevel:NSStatusWindowLevel];
  [nsWindow setCollectionBehavior:
      NSWindowCollectionBehaviorCanJoinAllSpaces |
      NSWindowCollectionBehaviorStationary];
}

void platformDisableFocus(GLFWwindow * window) {
  NSWindow * nsWindow = glfwGetCocoaWindow(window);
  if (nsWindow == nil) {
    return;
  }
  // AppKit não expõe um setter para impedir uma NSWindow comum de virar
  // key window ao ser clicada (isso exigiria subclassificar NSWindow e
  // sobrescrever -canBecomeKeyWindow, o que a janela criada pelo GLFW não
  // permite sem recompilar o próprio GLFW). GLFW_FOCUS_ON_SHOW já evita que
  // ela peça foco ao aparecer (usa orderFront: em vez de
  // makeKeyAndOrderFront:); accessoryBehavior reduz sua presença como
  // janela "de verdade" no Spaces/Mission Control. Limitação conhecida e
  // não testável neste ambiente (sem macOS disponível).
  [nsWindow setCollectionBehavior:
      [nsWindow collectionBehavior] | NSWindowCollectionBehaviorTransient];
}

std::string platformFindFont() {
  static const char * candidates[] = {
    "/System/Library/Fonts/Supplemental/Arial.ttf",
    "/System/Library/Fonts/SFNSText.ttf",
    "/Library/Fonts/Arial.ttf",
  };

  for (const char * path : candidates) {
    if (fileExists(path)) {
      return path;
    }
  }

  return "";
}

}} // namespace arken::notifygl
