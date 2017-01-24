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
#include "WindowGLFW.hpp"


using namespace CDwarfs;
using namespace CDwarfs::render;

RenderSystem::RenderSystem(const std::shared_ptr<TerrainObjectSystem>& terrainObjSys, const std::shared_ptr<TerrainMap>& terrainMap,
  const std::shared_ptr<EntityManager>& entManager, const std::shared_ptr<DwarfSystem>& dwarfSys) :
  m_terrainObjSys(terrainObjSys), m_terrainMap(terrainMap), m_entManager(entManager), m_dwarfSys(dwarfSys),
  m_shaderManager(std::make_shared<ShaderManager>()), m_tileRend(nullptr), m_window(nullptr) {}

RenderSystem::~RenderSystem() {}

void RenderSystem::init(int wnd_Width, int wnd_Height, const std::string& windowName) {

  m_window = std::make_shared<WindowGLFW>(wnd_Width, wnd_Height, windowName, false);
  m_window->init();

  if (gl3wInit()) throw std::runtime_error("Could not initialize gl3w!");
  if (!gl3wIsSupported(4, 3)) throw std::runtime_error("OpenGL 4.3 not supported!");

  std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << '\n';
  std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';


  GLfloat screenquadPos[] = { -1.f, -1.f, 0.f, 0.f,   1.f, -1.f, 1.f, 0.f,   -1.f, 1.f, 0.f, 1.f,
                              -1.f,  1.f, 0.f, 1.f,   1.f, -1.f, 1.f, 0.f,    1.f, 1.f, 1.f, 1.f };


  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  
  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(screenquadPos), screenquadPos, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*4, (void*)(sizeof(GLfloat)*2));
  glBindVertexArray(0);

  m_shaderManager->compileShader(R"_(
    #version 430
    layout(location=0) in vec2 pos;
    layout(location=1) in vec2 uv;

    out vec2 ex_uv;

    void main(void){
      gl_Position = vec4(pos, 0, 1);
      ex_uv = uv;
    }
    )_", "vertexShader", GL_VERTEX_SHADER);

  m_shaderManager->compileShader(R"_(
    #version 430
    smooth in vec2 ex_uv;
    out vec4 color;

    uniform sampler2D tex;

    void main(void){
      color = texture2D(tex, ex_uv);
    }
    )_", "fragmentShader", GL_FRAGMENT_SHADER);

  m_screenquadProg = m_shaderManager->createProgram("screenquadProg");
  m_shaderManager->attachShader("vertexShader", "screenquadProg");
  m_shaderManager->attachShader("fragmentShader", "screenquadProg");
  m_shaderManager->linkProgram("screenquadProg");
  m_shaderManager->deleteShader("vertexShader");
  m_shaderManager->deleteShader("fragmentShader");

}

void RenderSystem::render() {
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(m_vao);

  glDrawArrays(GL_TRIANGLES, 0, 6);

  m_shaderManager->useProgram(m_screenquadProg);

  glBindVertexArray(0);

  m_window->swapBuffers();
}