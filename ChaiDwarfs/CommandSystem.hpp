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
#ifndef _COMMANDSYSTEM_HPP_
#define _COMMANDSYSTEM_HPP_

#include <queue>
#include <stack>
#include "ComponentSystem.hpp"

namespace CDwarfs {

  class TerrainMap;
  class TerrainObjectSystem;
  
  namespace render {
    class TileRenderer;
  }

  class CommandSystem {
  public:
    CommandSystem() = delete;
    CommandSystem(const std::shared_ptr<EntityManager>& entManager) : m_entManager(entManager) {}

    ~CommandSystem() {}

    void init(const std::shared_ptr<TerrainMap>& terrainMap, const std::shared_ptr<TerrainObjectSystem>& terrainObjSys, const std::shared_ptr<render::TileRenderer>& tileRenderer) {
      addNewComponentSystem<compSys::TouchValue_Sys>();
      addNewComponentSystem<compSys::TouchDestroy_Sys>();
      addNewComponentSystem<compSys::TouchHeal_Sys>();
      addNewComponentSystem<compSys::TouchDamage_Sys>();
      addNewComponentSystem<compSys::Damage_Sys>();
      addNewComponentSystem<compSys::Points_Sys>();
      addNewComponentSystem<compSys::Move_Sys>(terrainMap, terrainObjSys);
      addNewComponentSystem<compSys::ChangeTerrainType_Sys>(terrainMap, terrainObjSys);
      addNewComponentSystem<compSys::ChangeTileType_Rendering_Sys>(tileRenderer);
    }

    void pushCommand(cmd::Command cmd) {
      m_cmdQueue.push(cmd);
    }

    void processQueue() {
      std::stack<cmd::Command> cmdStack;

      while (!m_cmdQueue.empty()) {
        cmdStack.push(m_cmdQueue.front());
        m_cmdQueue.pop();

        while (!cmdStack.empty()) {
          auto currCmd = cmdStack.top();
          cmdStack.pop();

          for (auto& visitor : m_visitors) {
            auto retCmd = std::visit(*visitor, currCmd);

            if (retCmd.size() >= 1) {
              for (auto rIt = retCmd.rbegin(); rIt != retCmd.rend(); rIt++) {
                cmdStack.push((*rIt));
              }
            }
          }
        }
      }
    }

  private:

    template<class TSys, class... Params>
    inline void addNewComponentSystem(Params&&... args) {
      m_visitors.push_back(std::make_shared<TSys>(m_entManager, std::forward<Params>(args)...));
    }

    std::queue<cmd::Command>  m_cmdQueue;
    std::vector<std::shared_ptr<compSys::BaseVisitor>>  m_visitors;
    std::shared_ptr<EntityManager> m_entManager;
  };
}

#endif // !_COMMANDSYSTEM_HPP_
