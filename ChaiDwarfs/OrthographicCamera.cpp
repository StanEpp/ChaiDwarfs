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

using namespace CDwarfs::render;

OrthographicCamera::OrthographicCamera() : 
  m_width(-1), m_height(-1), m_aspectratio(-1.f),
  m_orthoProj(1.f), m_scale{ 1.f, 1.f, 0.f }, m_translate{ 0.f, 0.f, 0.f }, m_MVP(1.f),
  m_input(nullptr) {}

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