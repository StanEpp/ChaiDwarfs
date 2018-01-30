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

#include <experimental/filesystem>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "src/ecs/EntityManager.hpp"
#include "src/rendering/SpriteRenderer.hpp"
#include "src/rendering/Texture2D.hpp"
#include "src/rendering/Texture2DArray.hpp"
#include "src/rendering/TextureFactory.hpp"
#include "src/rendering/TileRenderer.hpp"
#include "src/rendering/ShaderManager.hpp"
#include "src/rendering/OrthographicCamera.hpp"

using namespace cdwarfs;
using namespace cdwarfs::render;

SpriteRenderer::SpriteRenderer(
  const std::shared_ptr<EntityManager>& entManager,
  const std::shared_ptr<TileRenderer>& tileRenderer,
  const std::shared_ptr<ShaderManager>& shaderManager
  ) :
  m_entManager(entManager),
  m_tileRenderer(tileRenderer),
  m_shaderManager(shaderManager)
  {}

void SpriteRenderer::init(const std::shared_ptr<Texture2D>& targetTexture,
                          const std::shared_ptr<OrthographicCamera>& camera,
                          const std::shared_ptr<TextureFactory>& textureFactory)
{
  m_camera = camera;

  auto entities = m_entManager->getAllEntitiesWithComponent<comp::Sprites>();

  // Generate all the Sprite structs for internal use in SpriteRenderer
  for (const auto& ent : entities) {
    auto pos = m_entManager->getComponent<comp::Position>(ent);
    if (!pos) continue; //TODO: Maybe log the missing position for this sprites?
    auto sprites = m_entManager->getComponent<comp::Sprites>(ent);
    for (const auto& sprite : sprites->sprites) {
      if (std::experimental::filesystem::exists(sprite.second)) { //TODO: Maybe log that file couldn't be found?
        m_sprites[ent][sprite.first] = Sprite{ sprite.second };
      }
    }
    if (m_sprites.find(ent) != m_sprites.end()) {
      auto screenPos = m_tileRenderer->posToScreenCoord(pos->row, pos->col);
      m_sprites[ent].screenX = screenPos.x;
      m_sprites[ent].screenY = screenPos.y;
    }
  }

  entities = m_entManager->getAllEntitiesWithComponent<comp::AnimatedSprites>();
  // Generate all the AnimatedSprite structs for internal use in SpriteRenderer
  for (const auto& ent : entities) {
    auto pos = m_entManager->getComponent<comp::Position>(ent);
    if (!pos) continue; //TODO: Maybe log the missing position for this sprites?
    auto sprites = m_entManager->getComponent<comp::AnimatedSprites>(ent);
    for (const auto& sprite : sprites->sprites) {
      // Check how many keyframes there are
      auto posIt = std::find(sprite.second.rbegin(), sprite.second.rend(), '.');
      auto prefix = std::string(sprite.second.begin(), posIt.base() - 1);
      auto suffix = std::string(posIt.base() - 1, sprite.second.end());
      unsigned int numKeyframes = 0;
      for (int i = 0; i < 99; ++i) {
        auto filePath = prefix;
        if (i < 10) filePath += "0";
        filePath += std::to_string(i) + suffix;
        if (!std::experimental::filesystem::exists(filePath)) break;
        numKeyframes++;
      }
      if (numKeyframes == 0) continue;
      // Some Keyframes have been found => Create AnimatedSprite struct
      auto screenPos = m_tileRenderer->posToScreenCoord(pos->row, pos->col);
      m_animSprites[ent][sprite.first] = AnimatedSprite{ sprite.second, numKeyframes };
    }
    if (m_animSprites.find(ent) != m_animSprites.end()) {
      auto screenPos = m_tileRenderer->posToScreenCoord(pos->row, pos->col);
      m_animSprites[ent].screenX = screenPos.x;
      m_animSprites[ent].screenY = screenPos.y;
    }
  }

  // Load the images and create textures for rendering
  for (auto& spriteSet : m_sprites) {
    for (auto& sprite : spriteSet.second) {
      sprite.second.texture = textureFactory->loadTexture2D(sprite.second.filePath);
    }
  }

  for (auto& spriteSet : m_animSprites) {
    for (auto& sprite : spriteSet.second) {
      std::vector<std::string> filePaths;
      filePaths.reserve(sprite.second.numKeyFrames);
      for (size_t i = 0; i < sprite.second.numKeyFrames; ++i) {
        auto posIt = std::find(sprite.second.filePath.rbegin(), sprite.second.filePath.rend(), '.');
        auto prefix = std::string(sprite.second.filePath.begin(), posIt.base() - 1);
        auto suffix = std::string(posIt.base() - 1, sprite.second.filePath.end());
        auto filePath = prefix;
        if (i < 10) filePath += '0';
        filePath += std::to_string(i) + suffix;
        filePaths.push_back(std::move(filePath));
      }
      sprite.second.textureArray = textureFactory->loadTexture2DArray(filePaths, GL_RGBA);
    }
  }

  /**********

  v3/4(x,y)---v6(x,y)
  |          |
  |          |
  |          |
  v1(x,y)--- v2/5(x,y)
  (0,0)

  **********/
  auto q = m_tileRenderer->quadSize();
  std::vector<float> quad = {
    0.f, 0.f, 0.f, 0.f, //v1
    q, 0.f, 1.f, 0.f,   //v2
    0.f, q, 0.f, 1.f,   //v3
    0.f, q, 0.f, 1.f,   //v4
    q, 0.f, 1.f, 0.f,   //v5
    q, q, 1.f, 1.f      //v6
  };

  m_shaderManager->compileShader(R"_(
    #version 430
    layout(location=0) in vec2 pos;
    layout(location=1) in vec2 uv;

    uniform mat4 proj;
    uniform mat4 vp;
    smooth out vec2 ex_uv;

    void main(void){
      gl_Position = proj * vp * vec4(pos, 0, 1);
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
    )_", "fragmentShaderSprite", GL_FRAGMENT_SHADER);

  m_shaderManager->compileShader(R"_(
    #version 430
    smooth in vec2 ex_uv;
    out vec4 color;

    uniform sampler2DArray spriteAtlas;
    uniform uint keyFrame;

    void main(void){
      color = texture(spriteAtlas, vec3(ex_uv, keyFrame));
    }
    )_", "fragmentShaderAnimatedSprite", GL_FRAGMENT_SHADER);

  m_glsl_spriteProg = m_shaderManager->createProgram("spriteProg");
  m_shaderManager->attachShader("vertexShader", "spriteProg");
  m_shaderManager->attachShader("fragmentShaderSprite", "spriteProg");
  m_shaderManager->linkProgram("spriteProg");

  m_glsl_animSpriteProg = m_shaderManager->createProgram("animSpriteProg");
  m_shaderManager->attachShader("vertexShader", "animSpriteProg");
  m_shaderManager->attachShader("fragmentShaderAnimatedSprite", "animSpriteProg");
  m_shaderManager->linkProgram("animSpriteProg");

  m_shaderManager->deleteShader("vertexShader");
  m_shaderManager->deleteShader("fragmentShaderSprite");
  m_shaderManager->deleteShader("fragmentShaderAnimatedSprite");

  // Create Framebuffer Object
  glGenFramebuffers(1, &m_gl_fboID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_gl_fboID);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, targetTexture->texID(), 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  GLenum activeDrawBuffers = GL_COLOR_ATTACHMENT0;
  glNamedFramebufferDrawBuffers(m_gl_fboID, 1, &activeDrawBuffers);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glGenBuffers(1, &m_gl_vboVertexID);
  glBindBuffer(GL_ARRAY_BUFFER, m_gl_vboVertexID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quad.size(), quad.data(), GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glGenVertexArrays(1, &m_gl_vaoID);
  glBindVertexArray(m_gl_vaoID);
  glBindBuffer(GL_ARRAY_BUFFER, m_gl_vboVertexID);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
  glBindVertexArray(0);

  m_glsl_projMatLoc = m_shaderManager->getUniformLocation(m_glsl_spriteProg, "proj");
  m_glsl_vpMatLoc = m_shaderManager->getUniformLocation(m_glsl_spriteProg, "vp");
  m_glsl_Anim_projMatLoc = m_shaderManager->getUniformLocation(m_glsl_animSpriteProg, "proj");
  m_glsl_Anim_vpMatLoc = m_shaderManager->getUniformLocation(m_glsl_animSpriteProg, "vp");
  m_glsl_Anim_keyFrameLoc = m_shaderManager->getUniformLocation(m_glsl_animSpriteProg, "keyFrame");

}

void SpriteRenderer::spriteMove(EntityID::UUID entID, const SpriteKey& spriteKey, int nextRow, int nextCol)
{
  auto spriteSetIt = m_sprites.find(entID);
  if (spriteSetIt != m_sprites.end()) {
    auto& spriteSet = spriteSetIt->second;

    auto screenPos = m_tileRenderer->posToScreenCoord(nextRow, nextCol);
    spriteSet.nextScreenX = screenPos.x;
    spriteSet.nextScreenY = screenPos.y;
    spriteSet.moveStepX = (screenPos.x - spriteSet.screenX) / 500.f; //TODO: round duration should be changeable later on. For now 500ms.
    spriteSet.moveStepY = (screenPos.y - spriteSet.screenY) / 500.f;

    auto spriteIt = spriteSet.find(spriteKey);
    if (spriteIt != spriteSet.end()) {
      spriteSet.setCurrentSprite(spriteKey);
    }
  }

  auto animSpriteSetIt = m_animSprites.find(entID);
  if (animSpriteSetIt != m_animSprites.end()) {
    auto& spriteSet = animSpriteSetIt->second;

    auto screenPos = m_tileRenderer->posToScreenCoord(nextRow, nextCol);
    spriteSet.nextScreenX = screenPos.x;
    spriteSet.nextScreenY = screenPos.y;
    spriteSet.moveStepX = (screenPos.x - spriteSet.screenX) / 500.f; //TODO: round duration should be changeable later on. For now 500ms.
    spriteSet.moveStepY = (screenPos.y - spriteSet.screenY) / 500.f;

    auto animSpriteIt = spriteSet.find(spriteKey);
    if (animSpriteIt != spriteSet.end()) {
      auto& sprite = animSpriteIt->second;
      sprite.playing = true;
      sprite.currTime = 0.0;
      if (animSpriteIt != spriteSet.currentSpriteIt()) {
        spriteSet.currentSprite()->currKeyFrame = 0;
        spriteSet.currentSprite()->currTime = 0;
        spriteSet.currentSprite()->playing = false;
        spriteSet.setCurrentSprite(spriteKey);
      }
    }
  }
}

void SpriteRenderer::playAnimation(EntityID::UUID entID, const SpriteKey& spriteKey)
{
  auto spriteSetIt = m_animSprites.find(entID);
  if (spriteSetIt != m_animSprites.end()) {
    auto& spriteSet = spriteSetIt->second;
    auto spriteIt = spriteSet.find(spriteKey);
    if (spriteIt != spriteSet.end()) {
      spriteIt->second.playing = true;
      if (spriteIt != spriteSet.currentSpriteIt()) {
        auto animSprite = spriteSet.currentSprite();
        animSprite->currTime = 0.0;
        animSprite->currKeyFrame = 0;
        animSprite->playing = false;
      }
      spriteSet.setCurrentSprite(spriteKey);
    }
  }
}

void SpriteRenderer::advanceAnimations(double dt)
{
  for (auto& spriteListIt : m_animSprites) {
    auto sprite = spriteListIt.second.currentSprite();
    if (!sprite || !sprite->playing) continue;
    sprite->currTime += dt;
    if (sprite->currTime >= sprite->duration) {
      sprite->currKeyFrame = 0;
      sprite->currTime = 0.0;
      sprite->playing = false;
    }
    else {
      sprite->currKeyFrame = static_cast<unsigned int>(std::floor(static_cast<double>(sprite->numKeyFrames) * sprite->currTime / sprite->duration));
    }
  }
}

void SpriteRenderer::moveSprites(double dt)
{
  auto moveCurrentSprite = [&](auto& spriteSet) {
    if (spriteSet.nextScreenX.has_value()) {
      if (spriteSet.moveStepX < 0) {
        if (spriteSet.screenX <= spriteSet.nextScreenX.value()) {
          spriteSet.nextScreenX = {};
          spriteSet.moveStepX = 0.f;
        }
      }
      else {
        if (spriteSet.screenX >= spriteSet.nextScreenX.value()) {
          spriteSet.nextScreenX = {};
          spriteSet.moveStepX = 0.f;
        }
      }
      spriteSet.screenX += static_cast<float>(static_cast<double>(spriteSet.moveStepX) * dt);
    }
    if (spriteSet.nextScreenY.has_value()) {
      if (spriteSet.moveStepY < 0) {
        if (spriteSet.screenY <= spriteSet.nextScreenY) {
          spriteSet.nextScreenY = {};
          spriteSet.moveStepY = 0.f;
        }
      }
      else {
        if (spriteSet.screenY >= spriteSet.nextScreenY) {
          spriteSet.nextScreenY = {};
          spriteSet.moveStepY = 0.f;
        }
      }
      spriteSet.screenY += static_cast<float>(static_cast<double>(spriteSet.moveStepY) * dt);
    }
  };

  for (auto& spriteSet : m_sprites) {
    moveCurrentSprite(spriteSet.second);
  }

  for (auto& spriteSet : m_animSprites) {
    moveCurrentSprite(spriteSet.second);
  }
}

void SpriteRenderer::render(double dt)
{
  advanceAnimations(dt);
  moveSprites(dt);

  glBindVertexArray(m_gl_vaoID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_gl_fboID);

  glEnable(GL_BLEND);
  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

  m_shaderManager->useProgram(m_glsl_spriteProg);
  m_shaderManager->loadMatrix4(m_glsl_projMatLoc, m_camera->mvpPtr());

  for (const auto& spriteSet : m_sprites) {
    const auto sprite = spriteSet.second.currentSprite();
    if (sprite) {
      auto vpMat = glm::translate(glm::mat4(1.f), glm::vec3(spriteSet.second.screenX, spriteSet.second.screenY, 0.f));
      m_shaderManager->loadMatrix4(m_glsl_vpMatLoc, glm::value_ptr(vpMat));
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, sprite->texture->texID());
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }


  m_shaderManager->useProgram(m_glsl_animSpriteProg);
  m_shaderManager->loadMatrix4(m_glsl_Anim_projMatLoc, m_camera->mvpPtr());

  for (const auto& spriteSet : m_animSprites) {
    const auto sprite = spriteSet.second.currentSprite();
    if (sprite) {
      m_shaderManager->loadMatrix4(m_glsl_Anim_vpMatLoc, glm::value_ptr(glm::translate(glm::mat4(1.f), glm::vec3(spriteSet.second.screenX, spriteSet.second.screenY, 0.f))));
      m_shaderManager->loadUniform(m_glsl_Anim_keyFrameLoc, sprite->currKeyFrame);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D_ARRAY, sprite->textureArray->texID());
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
  m_shaderManager->resetProgram();

  glDisable(GL_BLEND);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindVertexArray(0);
}
