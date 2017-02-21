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
*/

#include "RenderSystem.hpp"
#include <iostream>
#include "ShaderManager.hpp"
#include "TileRenderer.hpp"
#include "ScreenquadRenderer.hpp"
#include "Texture.hpp"
#include "WindowGLFW.hpp"
#include "GLFWInput.hpp"
#include "OrthographicCamera.hpp"

using namespace CDwarfs;
using namespace CDwarfs::render;

RenderSystem::RenderSystem(const std::shared_ptr<TerrainObjectSystem>& terrainObjSys, const std::shared_ptr<TerrainMap>& terrainMap,
  const std::shared_ptr<EntityManager>& entManager, const std::shared_ptr<DwarfSystem>& dwarfSys, const std::shared_ptr<GLFWInput>& input) :
  m_terrainObjSys(terrainObjSys), m_terrainMap(terrainMap), m_entManager(entManager), m_dwarfSys(dwarfSys),
  m_shaderManager(std::make_shared<ShaderManager>()), m_tileRend(std::make_shared<TileRenderer>(m_shaderManager, terrainMap)), m_screenquad(std::make_shared<ScreenquadRenderer>(m_shaderManager)),
  m_camera(std::make_shared<OrthographicCamera>()), m_window(nullptr), m_input(input) {}

RenderSystem::~RenderSystem() {}

std::shared_ptr<WindowGLFW> RenderSystem::getWindow() {
  return m_window;
}

void RenderSystem::init(int wnd_Width, int wnd_Height, const std::string& windowName) {

  m_window = std::make_shared<WindowGLFW>(wnd_Width, wnd_Height, windowName, false);
  m_window->init();

  if (gl3wInit()) throw std::runtime_error("Could not initialize gl3w!");
  if (!gl3wIsSupported(4, 3)) throw std::runtime_error("OpenGL 4.3 not supported!");

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

  m_camera->init(wnd_Width, wnd_Height, m_input);

  m_outputTexture = std::make_shared<Texture2D>(wnd_Width, wnd_Height);

  m_screenquad->init(wnd_Width, wnd_Height, m_outputTexture);
  m_tileRend->init(m_outputTexture, m_camera);
}

const std::shared_ptr<TileRenderer> RenderSystem::getTileRenderer() {
  return m_tileRend;
}

void RenderSystem::render(double dt) {
  m_camera->update(1.0);

  m_tileRend->render();

  m_screenquad->render();

  m_window->swapBuffers();
}