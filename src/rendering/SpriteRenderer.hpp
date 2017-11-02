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

#include <memory>
#include <unordered_map>
#include <string>
#include <optional>

#ifndef _GL3W_
#define _GL3W_
#include "GL/gl3w.h"
#include <GLFW/glfw3.h>
#endif

#include "src/ecs/Entity.hpp"
#include "src/ecs/Components.hpp"
#include "src/rendering/Texture.hpp"
#include "src/rendering/Texture2DArray.hpp"

namespace cdwarfs
{
  class EntityManager;
}

namespace cdwarfs::render
{

class TileRenderer;
class OrthographicCamera;
class ShaderManager;

class SpriteRenderer
{
  using SpriteKey = std::string;

public:
  SpriteRenderer() = delete;
  SpriteRenderer(const std::shared_ptr<EntityManager>& entManager,
                 const std::shared_ptr<TileRenderer>& tileRenderer,
                 const std::shared_ptr<ShaderManager>& shaderManager);
  SpriteRenderer(SpriteRenderer&) = delete;
  SpriteRenderer(SpriteRenderer&&) = delete;

  void init(const std::shared_ptr<Texture2D>& targetTexture, const std::shared_ptr<OrthographicCamera>& camera);

  void render(double dt);

  void spriteMove(EntityID::UUID entID, const SpriteKey& spriteKey, int nextRow, int nextCol);
  void playAnimation(EntityID::UUID entID, const SpriteKey& spriteKey);

private:
  void advanceAnimations(double dt);
  void moveSprites(double dt);

  struct Sprite
  {
    std::string filePath;
    Texture2D texture;
  };

  struct AnimatedSprite
  {
    std::string filePath;
    unsigned int numKeyFrames{ 0 };
    unsigned int currKeyFrame{ 0 };
    double duration{ 500 }, currTime{ 0 }; //TODO: round duration should be changeable later on. For now 500ms.
    bool playing{ false };
    Texture2DArray textureArray;
  };

  template<typename T>
  class SpriteSet
  {
  public:
    std::optional<float> nextScreenX, nextScreenY;
    float screenX{ 0.f }, screenY{ 0.f };
    float moveStepX{ 0.0 }, moveStepY{ 0.0 };

    SpriteSet() : m_set(), m_currentSpriteIt(m_set.end()) {}

    T& operator[](const SpriteKey& key)
    {
      auto it = m_set.find(key);
      if (it != m_set.end()) return it->second;

      if (m_currentSpriteIt == m_set.end()){
        auto& newEl = m_set[key];
        m_currentSpriteIt = m_set.begin();
        return newEl;
      }
      else {
        const auto& spriteKey = m_currentSpriteIt->first;
        auto& newEl = m_set[key];
        m_currentSpriteIt = m_set.find(spriteKey);
        return newEl;
      }
    }

    typename std::unordered_map<SpriteKey, T>::iterator begin() { return m_set.begin(); }
    typename std::unordered_map<SpriteKey, T>::iterator end() { return m_set.end(); }
    typename std::unordered_map<SpriteKey, T>::iterator find(const SpriteKey& key) { return m_set.find(key); }
    typename std::unordered_map<SpriteKey, T>::iterator currentSpriteIt() const { return m_currentSpriteIt; }

    T* currentSprite() const
    {
      if (m_currentSpriteIt == m_set.end()) return nullptr;
      return &m_currentSpriteIt->second;
    }

    bool setCurrentSprite(const SpriteKey& key)
    {
      auto it = m_set.find(key);
      if (it != m_set.end()) {
        m_currentSpriteIt = it;
        return true;
      }
      return false;
    }

  private:
    std::unordered_map<SpriteKey, T> m_set;
    typename std::unordered_map<SpriteKey, T>::iterator m_currentSpriteIt;
  };

  std::unordered_map<EntityID::UUID, SpriteSet<AnimatedSprite>> m_animSprites;
  std::unordered_map<EntityID::UUID, SpriteSet<Sprite>>         m_sprites;

  std::shared_ptr<EntityManager> m_entManager;
  std::shared_ptr<TileRenderer>  m_tileRenderer;
  std::shared_ptr<ShaderManager> m_shaderManager;

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
