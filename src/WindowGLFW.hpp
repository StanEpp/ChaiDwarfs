#pragma once

#ifndef _GL3W_
#define _GL3W_
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#endif

#include <stdexcept>


namespace cdwarfs::render
{

class WindowGLFW
{
public:
  WindowGLFW() {}
  WindowGLFW(int width, int height, const std::string& windowName, bool fullscreen, bool vSync = false) :
    m_width(width),
    m_height(height),
    m_windowName(windowName),
    m_fullscreen(fullscreen),
    m_vSync(vSync),
    m_initialized(false),
    m_window(nullptr)
  {}

  WindowGLFW(WindowGLFW&) = delete;
  WindowGLFW(WindowGLFW&&) = delete;
  WindowGLFW& operator=(WindowGLFW&) = delete;
  WindowGLFW& operator=(WindowGLFW&&) = delete;

  ~WindowGLFW()
  {
    if (!m_initialized) return;
    glfwMakeContextCurrent(nullptr);
    if (m_window) glfwDestroyWindow(m_window);
    glfwTerminate();
  }

  inline int getWidth() const
  {
    return m_width;
  }

  inline int getHeight() const
  {
    return m_height;
  }

  GLFWwindow* getGLFWwindow() const
  {
    return m_window;
  }

  inline void swapBuffers()
  {
    glfwSwapBuffers(m_window);
  }

  inline void setWindowTitle(const std::string& title)
  {
    glfwSetWindowTitle(m_window, title.c_str());
  }

  inline void setDefaultWindowTitle()
  {
    glfwSetWindowTitle(m_window, m_windowName.c_str());
  }

  void setVSync(bool enable)
  {
    m_vSync = enable;
    glfwSwapInterval(m_vSync ? 1 : 0);
  }

  inline bool isVSyncOn()
  {
    return m_vSync;
  }

  void init()
  {
    if (!glfwInit()) {
      throw std::runtime_error("Could not initialize GLFW!");
    }

    m_initialized = true;

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), m_fullscreen ? glfwGetPrimaryMonitor() : 0, 0);

    if (!m_window) {
      throw std::runtime_error("Could not open GLFW Window!");
    }

    setDefaultWindowTitle();

    //glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(m_window);

    setVSync(m_vSync);
  }

private:
  int m_width{ 1024 };
  int m_height{ 768 };
  std::string m_windowName{ "GLFW Window" };
  bool m_fullscreen{ false };
  bool m_vSync{ false };
  bool m_initialized{ false };
  GLFWwindow* m_window{ nullptr };
};

}
