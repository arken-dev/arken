// Copyright 2016 The Arken Platform Authors.
// All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "dialog.h"

#include <algorithm>
#include <iostream>

#include <GLFW/glfw3.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "icon.h"
#include "platform.h"

namespace arken { namespace notifygl {

namespace {

const int   kWindowWidth       = 400;
const int   kHeaderHeight      = 34;
const int   kBodyHPadding      = 20; // 10px esquerda + 10px direita
const int   kBodyLeftPadding   = 10;
const int   kBodyTopPadding    = 16;
const int   kBodyBottomPadding = 15;
const int   kMarginRight       = 5;
const int   kMarginTop         = 30;
const int   kIconRadius        = 9;
const int   kIconCenterX       = 20;
const int   kIconCenterY       = 17;
const int   kTitleTextX        = 40;
const int   kTitleTextY        = 9;

const float kHeaderColor[4]    = {0.325f, 0.431f, 0.478f, 1.0f}; // #546e7a
const float kBodyColor[4]      = {0.984f, 0.984f, 0.984f, 1.0f}; // #fbfbfb
const float kBorderColor[3]    = {0.0f, 0.0f, 0.0f};
const float kTitleTextColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
const float kBodyTextColor[4]  = {0.082f, 0.082f, 0.082f, 1.0f}; // rgb(21,21,21)

void drawRect(float x0, float y0, float x1, float y1, const float color[4]) {
  glColor4fv(color);
  glBegin(GL_QUADS);
  glVertex2f(x0, y0);
  glVertex2f(x1, y0);
  glVertex2f(x1, y1);
  glVertex2f(x0, y1);
  glEnd();
}

} // namespace

NotifyNode::NotifyNode(std::string status_, std::string title_, std::string message_)
  : status(std::move(status_)), title(std::move(title_)), message(std::move(message_)) {
}

Dialog::Dialog() {
  m_thread = std::thread(&Dialog::loop, this);
  m_thread.detach();
}

Dialog & Dialog::instance() {
  static Dialog dialog;
  return dialog;
}

void Dialog::send(NotifyNode node) {
  Dialog & self = Dialog::instance();
  {
    std::lock_guard<std::mutex> lock(self.m_queueMutex);
    self.m_queue.push(std::move(node));
  }
  if (self.m_window != nullptr) {
    glfwPostEmptyEvent();
  }
}

bool Dialog::initWindow() {
  glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
  glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
  glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  m_window = glfwCreateWindow(kWindowWidth, m_windowHeight, "Arken Notify", nullptr, nullptr);
  if (m_window == nullptr) {
    std::cerr << "arken::notify (opengl): glfwCreateWindow failed" << std::endl;
    return false;
  }

  glfwSetWindowUserPointer(m_window, this);
  glfwMakeContextCurrent(m_window);
  glfwSwapInterval(1);

  glfwSetMouseButtonCallback(m_window, &Dialog::mouseButtonCallback);
  glfwSetFramebufferSizeCallback(m_window, &Dialog::framebufferSizeCallback);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_MULTISAMPLE);

  platformAlwaysOnTop(m_window);
  platformDisableFocus(m_window);

  std::string fontPath = platformFindFont();
  if (!m_titleFont.load(fontPath, 15)) {
    std::cerr << "arken::notify (opengl): failed to load title font" << std::endl;
  }
  if (!m_bodyFont.load(fontPath, 13)) {
    std::cerr << "arken::notify (opengl): failed to load body font" << std::endl;
  }

  framebufferSizeCallback(m_window, kWindowWidth, m_windowHeight);
  reposition();

  return true;
}

void Dialog::loop() {
  glfwSetErrorCallback([](int code, const char * description) {
    std::cerr << "arken::notify (opengl): glfw error " << code << ": " << description << std::endl;
  });

  if (!glfwInit()) {
    std::cerr << "arken::notify (opengl): glfwInit failed" << std::endl;
    return;
  }

  if (!initWindow()) {
    glfwTerminate();
    return;
  }

  while (!glfwWindowShouldClose(m_window)) {
    bool visible = glfwGetWindowAttrib(m_window, GLFW_VISIBLE) != 0;
    glfwWaitEventsTimeout(visible ? 0.2 : 0.5);

    NotifyNode next("", "", "");
    bool hasNext = false;
    {
      std::lock_guard<std::mutex> lock(m_queueMutex);
      if (!m_queue.empty()) {
        next = std::move(m_queue.front());
        m_queue.pop();
        hasNext = true;
      }
    }
    if (hasNext) {
      applyNode(next);
    }

    if (glfwGetWindowAttrib(m_window, GLFW_VISIBLE) != 0) {
      render();
      glfwSwapBuffers(m_window);
    }
  }

  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Dialog::applyNode(const NotifyNode & node) {
  glfwHideWindow(m_window);
  m_current = node;

  float maxBodyWidth = static_cast<float>(kWindowWidth - kBodyHPadding);
  m_wrappedLines = m_bodyFont.wrap(node.message, maxBodyWidth);

  float bodyHeight = static_cast<float>(m_wrappedLines.size()) * m_bodyFont.lineHeight();
  int desired = kHeaderHeight + kBodyTopPadding + static_cast<int>(bodyHeight) + kBodyBottomPadding;

  int monitorX = 0, monitorY = 0, monitorWidth = 0, monitorHeight = 0;
  glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monitorX, &monitorY, &monitorWidth, &monitorHeight);
  int maxHeight = static_cast<int>(static_cast<float>(monitorHeight) * 0.9f);

  m_windowHeight = std::min(std::max(desired, kHeaderHeight), maxHeight);
  glfwSetWindowSize(m_window, kWindowWidth, m_windowHeight);

  reposition();
  glfwShowWindow(m_window);
  platformAlwaysOnTop(m_window);
}

void Dialog::reposition() {
  int monitorX = 0, monitorY = 0, monitorWidth = 0, monitorHeight = 0;
  glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monitorX, &monitorY, &monitorWidth, &monitorHeight);
  int x = monitorX + monitorWidth - m_windowWidth - kMarginRight;
  int y = monitorY + kMarginTop;
  glfwSetWindowPos(m_window, x, y);
}

void Dialog::render() {
  glClear(GL_COLOR_BUFFER_BIT);

  drawRect(0, 0, static_cast<float>(kWindowWidth), static_cast<float>(kHeaderHeight), kHeaderColor);
  drawRect(0, static_cast<float>(kHeaderHeight), static_cast<float>(kWindowWidth), static_cast<float>(m_windowHeight), kBodyColor);

  glColor3fv(kBorderColor);
  glBegin(GL_LINE_LOOP);
  glVertex2f(0.5f, 0.5f);
  glVertex2f(static_cast<float>(kWindowWidth) - 0.5f, 0.5f);
  glVertex2f(static_cast<float>(kWindowWidth) - 0.5f, static_cast<float>(m_windowHeight) - 0.5f);
  glVertex2f(0.5f, static_cast<float>(m_windowHeight) - 0.5f);
  glEnd();

  drawIcon(resolveIconKind(m_current.status), static_cast<float>(kIconCenterX), static_cast<float>(kIconCenterY), static_cast<float>(kIconRadius));

  m_titleFont.draw(m_current.title, static_cast<float>(kTitleTextX), static_cast<float>(kTitleTextY), kTitleTextColor);

  float lineY = static_cast<float>(kHeaderHeight + kBodyTopPadding);
  for (const std::string & line : m_wrappedLines) {
    m_bodyFont.draw(line, static_cast<float>(kBodyLeftPadding), lineY, kBodyTextColor);
    lineY += m_bodyFont.lineHeight();
  }
}

void Dialog::mouseButtonCallback(GLFWwindow * window, int button, int action, int mods) {
  (void)button;
  (void)mods;
  if (action == GLFW_RELEASE) {
    glfwHideWindow(window);
  }
}

void Dialog::framebufferSizeCallback(GLFWwindow * window, int width, int height) {
  (void)window;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, width, height, 0, -1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

}} // namespace arken::notifygl
