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

#include "TileRenderer.hpp"
#include "TerrainMap.hpp"
#include "Texture.hpp"
#include "Texture2DArray.hpp"
#include "ShaderManager.hpp"
#include "OrthographicCamera.hpp"

#pragma warning(disable : 4201)
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#pragma warning(default : 4201)

using namespace cdwarfs;
using namespace cdwarfs::render;

TileRenderer::TileRenderer(
  const std::shared_ptr<ShaderManager>& shaderManager,
  const std::shared_ptr<TerrainMap>& terrainMap
  ) : 
  m_terrainMap(terrainMap),
  m_shaderManager(shaderManager)
  {}

TileRenderer::~TileRenderer(){}

void TileRenderer::initTiles() {
  m_tiles.resize(m_terrainMap->rows() * m_terrainMap->columns() * 6);
  for (int row = 0; row < m_terrainMap->rows(); ++row) {
    for (int col = 0; col < m_terrainMap->columns(); ++col) {
      setTileType(row, col, m_terrainMap->at(row, col));
    }
  }
}

void TileRenderer::setTileType(int row, int col, TerrainType newType) {
  //TODO: Check which type of the tile needs to be inserted. Check surrounding tiles to do that.
  using TerrType = std::underlying_type_t<TerrainType>;
  auto cols = m_terrainMap->columns();
  m_tiles[row * cols * 6 + col * 6 + 0] = static_cast<TerrType>(newType);
  m_tiles[row * cols * 6 + col * 6 + 1] = static_cast<TerrType>(newType);
  m_tiles[row * cols * 6 + col * 6 + 2] = static_cast<TerrType>(newType);
  m_tiles[row * cols * 6 + col * 6 + 3] = static_cast<TerrType>(newType);
  m_tiles[row * cols * 6 + col * 6 + 4] = static_cast<TerrType>(newType);
  m_tiles[row * cols * 6 + col * 6 + 5] = static_cast<TerrType>(newType);
}

void TileRenderer::init(const std::shared_ptr<Texture2D>& targetTexture, const std::shared_ptr<OrthographicCamera>& camera) {
  m_camera = camera;

  auto cols = m_terrainMap->columns();
  auto rows = m_terrainMap->rows();
  auto quadsSizeCol = 2.f / static_cast<float>(cols);
  auto quadsSizeRow = 2.f / static_cast<float>(rows);
  m_quadSize = (quadsSizeCol < quadsSizeRow) ? quadsSizeCol : quadsSizeRow;
  
  std::vector<float> vertices(cols * rows * 6 * 2, 0.f);

  /**********
    
    v3/4(x,y)---v6(x,y)
      |          |
      |          |
      |          |
    v1(x,y)--- v2/5(x,y)
    (0,1 - quadsize)
    
  **********/
  for (int row = 0; row < m_terrainMap->rows(); ++row) {
    for (int col = 0; col < m_terrainMap->columns(); ++col) {
      float rowf = static_cast<float>(row);
      float colf = static_cast<float>(col);

      vertices[row * cols * 6 * 2 + col * 6 * 2 + 0] = colf * m_quadSize - 1.f;         //v1.x
      vertices[row * cols * 6 * 2 + col * 6 * 2 + 1] = 1.f - (rowf + 1.f) * m_quadSize; //v1.y

      vertices[row * cols * 6 * 2 + col * 6 * 2 + 2] = (colf + 1.f) * m_quadSize - 1.f; //v2.x
      vertices[row * cols * 6 * 2 + col * 6 * 2 + 3] = 1.f - (rowf + 1.f) * m_quadSize; //v2.y

      vertices[row * cols * 6 * 2 + col * 6 * 2 + 4] = colf * m_quadSize - 1.f; //v3.x
      vertices[row * cols * 6 * 2 + col * 6 * 2 + 5] = 1.f - rowf * m_quadSize; //v3.y

      vertices[row * cols * 6 * 2 + col * 6 * 2 + 6] = colf * m_quadSize - 1.f; //v4.x
      vertices[row * cols * 6 * 2 + col * 6 * 2 + 7] = 1.f - rowf * m_quadSize; //v4.y

      vertices[row * cols * 6 * 2 + col * 6 * 2 + 8] = (colf + 1.f) * m_quadSize - 1.f; //v5.x
      vertices[row * cols * 6 * 2 + col * 6 * 2 + 9] = 1.f - (rowf + 1.f) * m_quadSize; //v5.y

      vertices[row * cols * 6 * 2 + col * 6 * 2 + 10] = (colf + 1.f) * m_quadSize - 1.f;  //v6.x
      vertices[row * cols * 6 * 2 + col * 6 * 2 + 11] = 1.f - rowf * m_quadSize;          //v6.y
    }
  }
  //std::for_each(vertices.begin(), vertices.end(), [](auto& x) { x = x - 1.f; });
  m_numVertices = static_cast<GLsizei>(vertices.size()/2);

  initTiles();
  
  m_shaderManager->loadShader("shader\\tileRendering_vs.glsl", "tileVertexShader", GL_VERTEX_SHADER);
  m_shaderManager->loadShader("shader\\tileRendering_fs.glsl", "tileFragmentShader", GL_FRAGMENT_SHADER);

  m_tileRenderingProg = m_shaderManager->createProgram("TileRenderingProgram");
  m_shaderManager->attachShader("tileVertexShader", "TileRenderingProgram");
  m_shaderManager->attachShader("tileFragmentShader", "TileRenderingProgram");
  m_shaderManager->linkProgram(m_tileRenderingProg);
  m_shaderManager->deleteShader("tileVertexShader");
  m_shaderManager->deleteShader("tileFragmentShader");

  glGenFramebuffers(1, &m_gl_fboID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_gl_fboID);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTexture->texID(), 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GLenum activeDrawBuffers = GL_COLOR_ATTACHMENT0;
  glNamedFramebufferDrawBuffers(m_gl_fboID, 1, &activeDrawBuffers);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glGenBuffers(1, &m_gl_vboVertexID);
  glBindBuffer(GL_ARRAY_BUFFER, m_gl_vboVertexID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &m_gl_vboTerrain1ID);
  glBindBuffer(GL_ARRAY_BUFFER, m_gl_vboTerrain1ID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(std::underlying_type_t<TerrainType>) * m_tiles.size(), m_tiles.data(), GL_STREAM_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenBuffers(1, &m_gl_vboTerrain2ID);
  glBindBuffer(GL_ARRAY_BUFFER, m_gl_vboTerrain2ID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(std::underlying_type_t<TerrainType>) * m_tiles.size(), m_tiles.data(), GL_STREAM_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &m_gl_vao1ID);
  glBindVertexArray(m_gl_vao1ID);
  glBindBuffer(GL_ARRAY_BUFFER, m_gl_vboVertexID);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, m_gl_vboTerrain1ID);
  glEnableVertexAttribArray(1);
  glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0, 0);

  glGenVertexArrays(1, &m_gl_vao2ID);
  glBindVertexArray(m_gl_vao2ID);
  glBindBuffer(GL_ARRAY_BUFFER, m_gl_vboVertexID);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, m_gl_vboTerrain2ID);
  glEnableVertexAttribArray(1);
  glVertexAttribIPointer(1, 1, GL_UNSIGNED_INT, 0, 0);
  glBindVertexArray(0);

  m_textureAtlas = std::make_shared<Texture2DArray>(std::vector<std::string>{ "tiles\\soil.png", "tiles\\passable.png", "tiles\\stone.png" }, GL_RGB);

  m_glsl_projMatLoc = m_shaderManager->getUniformLocation(m_tileRenderingProg, "mvp");
}

glm::vec2 TileRenderer::posToScreenCoord(int row, int col) {
  return glm::vec2(static_cast<float>(col) * m_quadSize - 1.f, 1.f - static_cast<float>(row + 1) * m_quadSize);
}

float TileRenderer::quadSize() {
  return m_quadSize;
}

void TileRenderer::render() {
  // TODO: Wrong! Frame is always one tile update behind!
  if (m_useVAO1) {
    glBindBuffer(GL_ARRAY_BUFFER, m_gl_vboTerrain2ID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(std::underlying_type_t<TerrainType>) * m_tiles.size(), m_tiles.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(m_gl_vao1ID);
  }
  else {
    glBindBuffer(GL_ARRAY_BUFFER, m_gl_vboTerrain1ID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(std::underlying_type_t<TerrainType>) * m_tiles.size(), m_tiles.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(m_gl_vao2ID);
  }
  m_useVAO1 = !m_useVAO1;

  glBindFramebuffer(GL_FRAMEBUFFER, m_gl_fboID);

  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClear(GL_COLOR_BUFFER_BIT);

  m_shaderManager->useProgram(m_tileRenderingProg);

  m_shaderManager->loadMatrix4(m_glsl_projMatLoc, m_camera->mvpPtr());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureAtlas->texID());
  
  glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

  m_shaderManager->resetProgram();

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindVertexArray(0);

}