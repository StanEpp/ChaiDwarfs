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
#ifndef _SPRITERENDERER_HPP_
#define _SPRITERENDERER_HPP_

#ifndef _GL3W_
#define _GL3W_
#include <GL\gl3w.h>
#include <GLFW\glfw3.h>
#endif

#include "Entity.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <optional>
#include "Components.hpp"
#include "Texture.hpp"
#include "Texture2DArray.hpp"

namespace cdwarfs {
  class EntityManager;

  namespace render {

    class TileRenderer;
    class OrthographicCamera;
    class ShaderManager;

    class SpriteRenderer {
    public:
      SpriteRenderer() = delete;
      SpriteRenderer(const std::shared_ptr<EntityManager>& entManager, const std::shared_ptr<TileRenderer>& tileRenderer, const std::shared_ptr<ShaderManager>& shaderManager);
      SpriteRenderer(SpriteRenderer&) = delete;
      SpriteRenderer(SpriteRenderer&&) = delete;

      void init(const std::shared_ptr<Texture2D>& targetTexture, const std::shared_ptr<OrthographicCamera>& camera);

      void render(double dt);

      void spriteUpdate(EntityID::UUID entID, std::string spriteKey, int nextRow, int nextCol);

    private:
      struct Sprite {
        std::optional<int> nextRow, nextCol;
        std::string filePath;
        float screenX{0.f}, screenY{0.f};
        Texture2D texture;
      };

      struct AnimatedSprite {
        std::optional<int> nextRow, nextCol;
        std::string filePath;
        float screenX{ 0.f }, screenY{ 0.f };
        bool playing{ false };
        double duration{ 500 }, currTime{ 0 };
        unsigned int numKeyFrames{ 0 };
        unsigned int currKeyFrame{ 0 };
        Texture2DArray textureArray;
      };

      using SpriteKey = std::string;
      using SpriteList = std::unordered_map<SpriteKey, Sprite>;
      using AnimatedSpriteList = std::unordered_map<SpriteKey, AnimatedSprite>;

      std::shared_ptr<EntityManager> m_entManager;
      std::shared_ptr<TileRenderer>  m_tileRenderer;
      std::shared_ptr<ShaderManager> m_shaderManager;

      std::unordered_map<EntityID::UUID, AnimatedSpriteList> m_animSprites;
      std::unordered_map<EntityID::UUID, SpriteList>         m_sprites;

      std::shared_ptr<OrthographicCamera> m_camera;
      GLuint  m_gl_fboID{ 0 };
      GLuint  m_gl_vboVertexID{ 0 };
      GLuint  m_gl_vaoID{ 0 };

      GLuint  m_glsl_projMatLoc{ 0 };
      GLuint  m_glsl_vpMatLoc{ 0 };

      GLuint  m_glsl_Anim_projMatLoc{ 0 };
      GLuint  m_glsl_Anim_vpMatLoc{ 0 };
      GLuint  m_glsl_Anim_keyFrameLoc{ 0 };
      GLuint  m_glsl_spriteProg{ 0 }; 
      GLuint  m_glsl_animSpriteProg{ 0 };
    };

  }
}


#endif // !_SPRITERENDERER_HPP_