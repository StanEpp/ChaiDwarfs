#ifndef _WINDOWGLFW_HPP_
#define _WINDOWGLFW_HPP_

#ifndef _GL3W_
#define _GL3W_
#include <GL\gl3w.h>
#include <GLFW\glfw3.h>
#endif

#include <stdexcept>


namespace CDwarfs {
  namespace render {

    class WindowGLFW {
    private:
      int _width, _height;
      std::string _windowName;
      bool  _fullscreen, _vSync, _initialized;
      GLFWwindow * _window;

    public:
      WindowGLFW() : _width(1024), _height(768), _windowName("GLFW Window"), _fullscreen(false), _vSync(false), _initialized(false), _window(nullptr) {}
      WindowGLFW(int width, int height, const std::string& windowName, bool fullscreen, bool vSync = false) :
        _width(width), _height(height), _windowName(windowName), _fullscreen(fullscreen), _vSync(vSync), _initialized(false), _window(nullptr) {}
      WindowGLFW(WindowGLFW&) = delete;
      WindowGLFW(WindowGLFW&&) = delete;
      WindowGLFW& operator=(WindowGLFW&) = delete;
      WindowGLFW& operator=(WindowGLFW&&) = delete;

      ~WindowGLFW() {
        if (!_initialized) return;
        glfwMakeContextCurrent(nullptr);
        if (_window) glfwDestroyWindow(_window);
        glfwTerminate();
      }

      inline int getWidth() const {
        return _width;
      }

      inline int getHeight() const {
        return _height;
      }

      GLFWwindow* getGLFWwindow() const {
        return _window;
      }

      inline void swapBuffers() {
        glfwSwapBuffers(_window);
      }

      inline void setWindowTitle(const std::string& title) {
        glfwSetWindowTitle(_window, title.c_str());
      }

      inline void setDefaultWindowTitle() {
        glfwSetWindowTitle(_window, _windowName.c_str());
      }

      void setVSync(bool enable) {
        _vSync = enable;
        glfwSwapInterval(_vSync ? 0 : 1);
      }

      inline bool isVSyncOn() {
        return _vSync;
      }

      void init() {
        if (!glfwInit()) {
          throw std::runtime_error("Could not initialize GLFW!");
        }

        _initialized = true;

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        _window = glfwCreateWindow(_width, _height, _windowName.c_str(), _fullscreen ? glfwGetPrimaryMonitor() : 0, 0);

        if (!_window) {
          throw std::runtime_error("Could not open GLFW Window!");
        }

        setDefaultWindowTitle();

        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwMakeContextCurrent(_window);

        glfwSwapInterval(_vSync ? 0 : 1);
      }
    };
  }
}

#endif // !_WINDOWGLFW_HPP_
