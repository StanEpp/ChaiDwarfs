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
#ifndef _DWARFSYSTEM_HPP_
#define _DWARFSYSTEM_HPP_

#include <vector>

#include "EntityManager.hpp"
#include "CommandSystem.hpp"
#include "Components.hpp"
#include "DwarfRoundActions.hpp"

namespace CDwarfs {

  class DwarfSystem {
  public:
    DwarfSystem(const std::shared_ptr<EntityManager>& entityManager, const std::shared_ptr<CommandSystem>& cmdSystem) : 
      m_entityManager(entityManager), m_cmdSystem(cmdSystem) {}
    DwarfSystem() = delete;
    DwarfSystem(DwarfSystem&) = delete; DwarfSystem& operator=(DwarfSystem&) = delete;
    DwarfSystem(DwarfSystem&&) = delete; DwarfSystem& operator=(DwarfSystem&&) = delete;
    ~DwarfSystem() = default;

    EntityID::UUID add(
      const std::string& dwarfType,
      const std::shared_ptr<TerrainMap> &terrainMap, 
      const std::shared_ptr<TerrainObjectSystem> &terrainObjSys) 
    {
      auto ID = m_entityManager->createObject(dwarfType);
      m_DwarfIDs.push_back(ID);
      
      auto ai = m_entityManager->getComponent<comp::AIComponent>(ID);
      
      if (ai) {
        ai->ai->setTerrainInterface(terrainMap);
        ai->ai->setTerrainObjectInterface(terrainObjSys);
        ai->ai->setEntityManager(m_entityManager);
        ai->ai->setDwarfID(ID);
        ai->ai->init();
      }

      return ID;
    }

    void erase(EntityID::UUID ID) {
      if (ID >= m_DwarfIDs.size()) return;
      m_DwarfIDs.erase(m_DwarfIDs.cbegin() + ID);
      m_entityManager->killEntity(ID);
    }

    void updateDwarfs() {
      for (auto eID : m_DwarfIDs) {
        auto ai = m_entityManager->getComponent<comp::AIComponent>(eID);
        if (ai) {
          auto actions = ai->ai->generateCommand();
          
          switch (actions.movement) {
            case DwarfRoundActions::MoveDirection::LEFT: {
              cmd::Cmd_MoveLeft cmdLeft;
              cmdLeft.dest = eID;
              m_cmdSystem->pushCommand(cmdLeft);
            } break;
            case DwarfRoundActions::MoveDirection::RIGHT: {
              cmd::Cmd_MoveRight cmdRight;
              cmdRight.dest = eID;
              m_cmdSystem->pushCommand(cmdRight);
            } break;
            case DwarfRoundActions::MoveDirection::UP: {
              cmd::Cmd_MoveUp cmdUp;
              cmdUp.dest = eID;
              m_cmdSystem->pushCommand(cmdUp);
            } break;
            case DwarfRoundActions::MoveDirection::DOWN: {
              cmd::Cmd_MoveDown cmdDown;
              cmdDown.dest = eID;
              m_cmdSystem->pushCommand(cmdDown);
            } break;
          }

        }
      }
    }

  private:
    std::vector<EntityID::UUID>    m_DwarfIDs;
    std::shared_ptr<EntityManager> m_entityManager;
    std::shared_ptr<CommandSystem> m_cmdSystem;
  };

}

#endif
