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

#pragma once

#include <glm/glm.hpp>

#include <memory>

namespace cdwarfs::render
{

class GLFWInput;

class OrthographicCamera
{
public:
  OrthographicCamera();

  void init(int width, int height, const std::shared_ptr<GLFWInput>& input);
  const float* mvpPtr();
  void update(double dt);

private:
  int m_width{ -1 };
  int m_height{ -1 };
  float m_aspectratio{ -1.f };

  glm::mat4 m_orthoProj{ 1.f };
  glm::vec3 m_scale{ 1.f, 1.f, 0.f };
  glm::vec3 m_translate{ 0.f, 0.f, 0.f };
  glm::mat4 m_MVP{ 1.f };

  std::shared_ptr<GLFWInput> m_input;
};

}
