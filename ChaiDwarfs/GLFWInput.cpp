#include "GLFWInput.hpp"

#ifndef _GL3W_
#define _GL3W_
#include <GL\gl3w.h>
#include <GLFW\glfw3.h>
#endif

#include "WindowGLFW.hpp"
#include <functional>

using namespace CDwarfs::render;

GLFWInput::GLFWInput() : m_xPos(0), m_yPos(0), m_xDiff(0), m_yDiff(0), m_mouseButtonPressed{ false, false, false }, m_window(nullptr) {}

GLFWInput::~GLFWInput() {
  if(m_window) glfwSetWindowUserPointer(m_window, nullptr);
}

void GLFWInput::bindInputToWindow(const WindowGLFW& window) {
  m_window = window.getGLFWwindow();

  glfwSetWindowUserPointer(m_window, this);
  glfwSetScrollCallback(m_window, [](GLFWwindow* wnd, double xOff, double yOff) { 
    static_cast<GLFWInput*>(glfwGetWindowUserPointer(wnd))->mouseWheelCallback(wnd, xOff, yOff); 
  });

}

void GLFWInput::updateInput() {
  double x, y;
  glfwPollEvents();
  glfwGetCursorPos(m_window, &x, &y);
  m_xDiff = x - m_xPos;
  m_yDiff = y - m_yPos;
  m_xPos = x;
  m_yPos = y;
  m_scrollDiff = m_scrollLastDiff;
  m_scrollLastDiff = 0.0;
}

int GLFWInput::isKeyPressed(unsigned int GLFW_Key) const {
  return (glfwGetKey(m_window, GLFW_Key) == GLFW_PRESS);
}

bool GLFWInput::isMouseButtonPressed(unsigned int GLFW_MouseButton) const {
  return (glfwGetMouseButton(m_window, GLFW_MouseButton) == GLFW_PRESS);
}

bool GLFWInput::isKeyPressedOnce(unsigned int GLFW_Key) {
  if (isKeyPressed(GLFW_Key)) {
    if (m_keyPressed.find(GLFW_Key) == m_keyPressed.end()) {
      m_keyPressed.insert(GLFW_Key);
      return true;
    }
  }
  else {
    m_keyPressed.erase(GLFW_Key);
  }
  return false;
}

bool GLFWInput::isMouseButtonPressedOnce(unsigned int GLFW_MouseButton) {
  if (isMouseButtonPressed(GLFW_MouseButton)) {
    if (!m_mouseButtonPressed[GLFW_MouseButton]) {
      m_mouseButtonPressed[GLFW_MouseButton] = true;
      return true;
    }
  }
  else {
    m_mouseButtonPressed[GLFW_MouseButton] = false;
  }
  return false;
}

void GLFWInput::setMousePos(double xpos, double ypos) {
  glfwSetCursorPos(m_window, xpos, ypos);
}

void GLFWInput::mouseWheelCallback(GLFWwindow* wnd, double xOff, double yOff) {
  m_scrollLastDiff = yOff;
}