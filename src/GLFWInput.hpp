/*
*  Copyright(c) 2016 - 2017 Stanislaw Eppinger
*  Scripting based game called ChaiDwarfs
*
*  This file is part of ChaiDwarfs.
*
*  ChaiDwarfs is free software : you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.If not, see <http://www.gnu.org/licenses/>
*
*/

#ifndef _GLFWINPUT_
#define _GLFWINPUT_

#include <unordered_set>
#include <array>

struct GLFWwindow;

namespace cdwarfs::render {

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

    double              m_xPos{ 0.0 }, m_yPos{ 0.0 };
    double              m_xDiff{ 0.0 }, m_yDiff{ 0.0 };
    double              m_scrollDiff{ false }, m_scrollLastDiff{ false };
    std::array<bool, 3> m_mouseButtonPressed{ false, false, false };
    std::unordered_set<unsigned int>  m_keyPressed;
    GLFWwindow* m_window{ nullptr };
  };
}

#endif