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

#include <queue>
#include <stack>

#include "cmpSystems/ComponentSystem.hpp"

namespace cdwarfs::render {
class TileRenderer;
class SpriteRenderer;
}

namespace cdwarfs {

class TerrainMap;
class TerrainObjectSystem;

class CommandSystem {
public:
    CommandSystem() = delete;
    CommandSystem(const std::shared_ptr<EntityManager>& entManager) : m_entManager(entManager) {}

    ~CommandSystem() {}

    void init(const std::shared_ptr<TerrainMap>& terrainMap,
              const std::shared_ptr<TerrainObjectSystem>& terrainObjSys,
              const std::shared_ptr<render::TileRenderer>& tileRenderer,
              const std::shared_ptr<render::SpriteRenderer>& spriteRenderer)
    {
        addNewComponentSystem<compSys::TouchValue_Sys>();
        addNewComponentSystem<compSys::TouchDestroy_Sys>();
        addNewComponentSystem<compSys::TouchHeal_Sys>();
        addNewComponentSystem<compSys::TouchDamage_Sys>();
        addNewComponentSystem<compSys::Damage_Sys>();
        addNewComponentSystem<compSys::Points_Sys>();
        addNewComponentSystem<compSys::Move_Sys>(terrainMap, terrainObjSys);
        addNewComponentSystem<compSys::ChangeTerrainType_Sys>(terrainMap, terrainObjSys);
        addNewComponentSystem<compSys::ChangeTileType_Rendering_Sys>(tileRenderer);
        addNewComponentSystem<compSys::MoveSprite_Sys>(spriteRenderer);
        addNewComponentSystem<compSys::DwarfAI_Sys>(terrainMap, terrainObjSys);
        addNewComponentSystem<compSys::KillEntity_Sys>();
    }

    void pushCommand(cmd::Command cmd)
    {
        m_cmdQueue.push(cmd);
    }

    void executeRound()
    {
        // Execute every AI/script
        m_cmdQueue.push(cmd::ExecuteEveryAI());
        processQueue();

        // All commands have been executed. Now destroy entities
        // which are flagged for destruction.
        m_cmdQueue.push(cmd::KillFlaggedEntities());
        processQueue();
    }

private:

    void processQueue()
    {
        while (!m_cmdQueue.empty()) {
            m_cmdStack.push(m_cmdQueue.front());
            m_cmdQueue.pop();

            while (!m_cmdStack.empty()) {
                auto currCmd = m_cmdStack.top();
                m_cmdStack.pop();

                for (auto& visitor : m_visitors) {
                    auto retCmd = std::visit(*visitor, currCmd);

                    if (retCmd.size() >= 1) {
                        for (auto rIt = retCmd.rbegin(); rIt != retCmd.rend(); rIt++) {
                            m_cmdStack.push((*rIt));
                        }
                    }
                }
            }
        }
    }

    template<class TSys, class... Params>
    inline void addNewComponentSystem(Params&&... args)
    {
        m_visitors.push_back(std::make_shared<TSys>(m_entManager, std::forward<Params>(args)...));
    }

    std::queue<cmd::Command> m_cmdQueue;
    std::stack<cmd::Command> m_cmdStack; // Stack is needed for correct processing of commands.
    std::vector<std::shared_ptr<compSys::BaseVisitor>>  m_visitors;
    std::shared_ptr<EntityManager> m_entManager;
};

}
