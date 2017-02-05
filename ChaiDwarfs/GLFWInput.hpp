#ifndef _GLFWINPUT_
#define _GLFWINPUT_

#include <unordered_set>
#include <array>

struct GLFWwindow;

namespace CDwarfs {
  namespace render {

    class WindowGLFW;

    class GLFWInput {
    public:
      GLFWInput();
      ~GLFWInput();

      void bindInputToWindow(const WindowGLFW& window);

      void updateInput();

      int isKeyPressed(unsigned int GLFW_Key) const;

      bool isMouseButtonPressed(unsigned int GLFW_MouseButton) const;

      inline double getXPos() const {
        return m_xPos;
      }

      inline double getYPos() const {
        return m_yPos;
      }

      inline double getXPosDiff() const {
        return m_xDiff;
      }

      inline double getYPosDiff() const {
        return m_yDiff;
      }

      inline double getScrollDiff() const {
        return m_scrollDiff;
      }

      bool isKeyPressedOnce(unsigned int GLFW_Key);

      bool isMouseButtonPressedOnce(unsigned int GLFW_MouseButton);

      void setMousePos(double xpos, double ypos);

      void mouseWheelCallback(GLFWwindow* wnd, double xOff, double yOff);

    private:

      double              m_xPos, m_yPos;
      double              m_xDiff, m_yDiff;
      double              m_scrollDiff, m_scrollLastDiff;
      std::array<bool, 3> m_mouseButtonPressed;
      std::unordered_set<unsigned int>  m_keyPressed;
      GLFWwindow* m_window;
    };
  }
}

#endif