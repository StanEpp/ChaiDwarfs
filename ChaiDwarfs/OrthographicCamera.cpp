
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

#include "OrthographicCamera.hpp"

#ifndef _GL3W_
#define _GL3W_
#include <GL\gl3w.h>
#include <GLFW\glfw3.h>
#endif

#include "GLFWInput.hpp"

#pragma warning(disable : 4201)
#include <glm\gtc\matrix_transform.hpp>
#pragma warning(default : 4201)

using namespace cdwarfs::render;

OrthographicCamera::OrthographicCamera() {}

void OrthographicCamera::init(int width, int height, const std::shared_ptr<GLFWInput>& input) {
  m_input = input;

  m_width = width;
  m_height = height;
  m_aspectratio = static_cast<float>(width) / static_cast<float>(height);

  m_orthoProj = glm::ortho(-m_aspectratio, m_aspectratio, -1.f, 1.f, 0.f, 1.f);
  m_MVP = glm::translate(glm::scale(m_orthoProj, m_scale), m_translate);
}

const float* OrthographicCamera::mvpPtr() {
  return &m_MVP[0][0];
}

void OrthographicCamera::update(double dt) {
  float dx, dy;
  dx = dy = 0.f;
  double speed = 0.01; //TODO: Adjust speed, when dt gets a meaning

  if (m_input->isKeyPressed(GLFW_KEY_W)) {
    dy = static_cast<float>(-1.0 * speed * dt);
  }

  if (m_input->isKeyPressed(GLFW_KEY_S)) {
    dy = static_cast<float>(speed * dt);
  }

  if (m_input->isKeyPressed(GLFW_KEY_D)) {
    dx = static_cast<float>(-1.0 * speed * dt);
  }

  if (m_input->isKeyPressed(GLFW_KEY_A)) {
    dx = static_cast<float>(speed * dt);
  }

  m_translate += glm::vec3( dx, dy, 0 );
  m_scale += glm::vec3( m_input->getScrollDiff() * speed * 2, m_input->getScrollDiff() * speed * 2, 0 );

  m_MVP = glm::translate(glm::translate(glm::scale(glm::translate(m_orthoProj, { 0, 0, 0 }), m_scale), { 0,0, 0 }), m_translate);
}