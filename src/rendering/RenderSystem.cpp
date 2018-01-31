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

#include <iostream>

#include "RenderSystem.hpp"
#include "ShaderManager.hpp"
#include "TileRenderer.hpp"
#include "ScreenquadRenderer.hpp"
#include "SpriteRenderer.hpp"
#include "Texture2D.hpp"
#include "OrthographicCamera.hpp"
#include "TextureFactory.hpp"
#include "src/WindowGLFW.hpp"
#include "src/GLFWInput.hpp"

using namespace cdwarfs;
using namespace cdwarfs::render;

RenderSystem::RenderSystem(const std::shared_ptr<TerrainObjectSystem>& terrainObjSys,
                           const std::shared_ptr<TerrainMap>& terrainMap,
                           const std::shared_ptr<EntityManager>& entManager,
                           const std::shared_ptr<DwarfSystem>& dwarfSys,
                           const std::shared_ptr<GLFWInput>& input) :
  m_window(nullptr),
  m_textureFactory(nullptr),
  m_terrainObjSys(terrainObjSys),
  m_terrainMap(terrainMap),
  m_entManager(entManager),
  m_dwarfSys(dwarfSys),
  m_shaderManager(std::make_shared<ShaderManager>()),
  m_tileRend(std::make_shared<TileRenderer>(m_shaderManager, terrainMap)),
  m_screenquad(std::make_shared<ScreenquadRenderer>(m_shaderManager)),
  m_spriteRend(std::make_shared<SpriteRenderer>(entManager, m_tileRend, m_shaderManager)),
  m_camera(std::make_shared<OrthographicCamera>()),
  m_input(input)
  {}

RenderSystem::~RenderSystem() {}

void RenderSystem::init(int wnd_Width, int wnd_Height, const std::string& windowName)
{
  m_window = std::make_shared<WindowGLFW>(wnd_Width, wnd_Height, windowName, false);
  m_window->init();

  if (gl3wInit()) throw std::runtime_error("Could not initialize gl3w!");
  if (!gl3wIsSupported(4, 3)) throw std::runtime_error("OpenGL 4.3 not supported!");

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

  m_textureFactory = std::make_shared<TextureFactory>(m_window);

  m_camera->init(wnd_Width, wnd_Height, m_input);

  m_outputTexture = m_textureFactory->createTexture2D(wnd_Width, wnd_Height, 3);

  m_screenquad->init(wnd_Width, wnd_Height, m_outputTexture, m_textureFactory);
  m_tileRend->init(m_outputTexture, m_camera, m_textureFactory);

  m_spriteRend->init(m_outputTexture, m_camera, m_textureFactory);
}

std::shared_ptr<WindowGLFW> RenderSystem::getWindow()
{
  return m_window;
}

const std::shared_ptr<TileRenderer> RenderSystem::getTileRenderer()
{
  return m_tileRend;
}

const std::shared_ptr<SpriteRenderer> RenderSystem::getSpriteRenderer()
{
  return m_spriteRend;
}

void RenderSystem::render(double dt)
{
  m_camera->update(dt);

  m_tileRend->render();

  m_spriteRend->render(dt);

  m_screenquad->render();

  m_window->swapBuffers();
}
