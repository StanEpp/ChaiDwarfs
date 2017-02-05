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

#ifndef _TILERENDERER_HPP_
#define _TILERENDERER_HPP_

#ifndef _GL3W_
#define _GL3W_
#include <GL\gl3w.h>
#include <GLFW\glfw3.h>
#endif

#include <memory>
#include <vector>
#include "TerrainMap.hpp"

#pragma warning(disable : 4201)
#include <glm\glm.hpp>
#pragma warning(default : 4201)

namespace CDwarfs {

  class TerrainMap;
  
  namespace render {

    class Texture2D;
    class Texture2DArray;
    class ShaderManager;
    class OrthographicCamera;

    class TileRenderer {
    public:
      TileRenderer() = delete;
      TileRenderer(const std::shared_ptr<ShaderManager>& shaderManager, const std::shared_ptr<TerrainMap>& terrainMap);
      ~TileRenderer();

      void init(const std::shared_ptr<Texture2D>& targetTexture, const std::shared_ptr<OrthographicCamera>& camera);
      void render();

      void setTileType(int row, int col, TerrainType newType);

      glm::vec2 posToScreenCoord(int row, int col);

    private:
      void initTiles();

      std::shared_ptr<TerrainMap>         m_terrainMap;
      std::shared_ptr<Texture2DArray>     m_textureAtlas;
      std::shared_ptr<ShaderManager>      m_shaderManager;
      std::shared_ptr<OrthographicCamera> m_camera;

      std::vector<std::underlying_type_t<TerrainType>>  m_tiles;

      GLuint  m_gl_fboID;
      GLuint  m_gl_vboVertexID, m_gl_vboTerrain1ID, m_gl_vboTerrain2ID;
      GLuint  m_gl_vao1ID, m_gl_vao2ID;
      GLuint  m_tileRenderingProg;
      GLsizei m_numVertices;

      GLint m_glsl_projMatLoc;

      float m_quadSize;
      bool  m_useVAO1;
    };

  }
}

#endif // !_TILERENDERER_HPP_
