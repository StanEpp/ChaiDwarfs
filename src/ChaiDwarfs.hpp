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

#pragma once

#include <memory>

#include "rendering/RenderSystem.hpp"
#include "cmpSystems/TerrainObjectSystem.hpp"
#include "ecs/EntityManager.hpp"
#include "TerrainMap.hpp"
#include "Timer.hpp"
#include "CommandSystem.hpp"
#include "GLFWInput.hpp"
#include "PathResolver.hpp"

namespace cdwarfs
{

class ChaiDwarfs
{
public:
    ChaiDwarfs() :
        m_entManager(std::make_shared<EntityManager>()),
        m_cmdSystem(std::make_shared<CommandSystem>(m_entManager)),
        m_terrain(std::make_shared<TerrainMap>()),
        m_terrainObjSys(std::make_shared<TerrainObjectSystem>(m_entManager)),
        m_input(std::make_shared<render::GLFWInput>()),
        m_renderer(std::make_shared<render::RenderSystem>(m_terrainObjSys, m_terrain, m_entManager, m_input)),
        m_running(true)
    {}

    void init()
    {
        m_terrain->load(pathRes.script("mapGeneration.chai"));

        m_entManager->init();
        m_entManager->loadObjectDefinitions(pathRes.script("objectDefinitions.chai"));

        m_terrainObjSys->loadObjects(pathRes.script("objectCreation.chai"));

        m_renderer->init(1280, 720, "ChaiDwarfs");

        m_cmdSystem->init(m_terrain, m_terrainObjSys, m_renderer->getTileRenderer(), m_renderer->getSpriteRenderer());

        m_input->bindInputToWindow(*m_renderer->getWindow());
    }

    void run()
    {
      Timer timer;

      auto lastRound = timer.currentTime();
      int fpsCounter = 0;

      while (m_running) {
          m_input->updateInput();

          if (m_input->isKeyPressedOnce(GLFW_KEY_ESCAPE)) {
              m_running = false;
          }

          if (timer.haveMilliSecondsPassed(500, lastRound)) {
              m_cmdSystem->pushCommand(cmd::ExecuteEveryAI{});
              m_terrainObjSys->objectCollisions(m_cmdSystem);
              m_cmdSystem->processQueue();
              killAllDestroyedEntities();
              lastRound = timer.currentTime();
          }

          timer.setDt(timer.lastFrameRendered, timer.currentTime());
          timer.lastFrameRendered = timer.currentTime();
          m_renderer->render(timer.dt);

          ++fpsCounter;

          if (timer.haveMilliSecondsPassed(1000, timer.lastFpsUpdate)) {
              fpsCounter = 0;
              timer.lastFpsUpdate = timer.currentTime();
          }
      }
    }

private:
    std::shared_ptr<EntityManager>        m_entManager;
    std::shared_ptr<CommandSystem>        m_cmdSystem;
    std::shared_ptr<TerrainMap>           m_terrain;
    std::shared_ptr<TerrainObjectSystem>  m_terrainObjSys;

    std::shared_ptr<render::GLFWInput>    m_input;
    std::shared_ptr<render::RenderSystem> m_renderer;

    bool  m_running;

    void killAllDestroyedEntities()
    {
        auto vec = m_entManager->getAllEntitiesWithComponent<comp::FlaggedDestroyed>();
        for (auto ID : vec) {
            m_entManager->killEntity(ID);
        }
    }
};

}
