// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef ARKEN_NOTIFY_OPENGL_DIALOG_H
#define ARKEN_NOTIFY_OPENGL_DIALOG_H

#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <vector>

#include "text.h"

struct GLFWwindow;

namespace arken { namespace notifygl {

struct NotifyNode {
  std::string status;  // "ok", "warning", "error", "failure", ...
  std::string title;
  std::string message;

  NotifyNode(std::string status, std::string title, std::string message);
};

// Janela de notificação singleton, sempre por cima de qualquer outra
// aplicação. Nunca é destruída entre notificações: ao clicar ela apenas
// fica invisível (glfwHideWindow), e a mesma janela é reaproveitada e
// reexibida sempre que title/message/status mudam (nova chamada a send()).
class Dialog {
  public:
  static void send(NotifyNode node);

  private:
  Dialog();
  ~Dialog() = default;
  Dialog(const Dialog &)             = delete;
  Dialog & operator=(const Dialog &) = delete;

  static Dialog & instance();

  void loop();
  bool initWindow();
  void applyNode(const NotifyNode & node);
  void reposition();
  void render();

  static void mouseButtonCallback(GLFWwindow * window, int button, int action, int mods);
  static void framebufferSizeCallback(GLFWwindow * window, int width, int height);

  std::thread m_thread;

  std::mutex              m_queueMutex;
  std::queue<NotifyNode>  m_queue;

  GLFWwindow * m_window = nullptr;
  Font         m_titleFont;
  Font         m_bodyFont;

  NotifyNode                m_current{"ok", "", ""};
  std::vector<std::string>  m_wrappedLines;
  int                        m_windowWidth  = 400;
  int                        m_windowHeight = 150;
};

}} // namespace arken::notifygl

#endif // ARKEN_NOTIFY_OPENGL_DIALOG_H
