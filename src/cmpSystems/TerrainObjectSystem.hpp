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
#include <string>
#include <vector>

#include "src/ecs/Entity.hpp"

namespace cdwarfs
{

class CommandSystem;
class EntityManager;

class TerrainObjectSystem
{
public:
    TerrainObjectSystem() = delete;
    TerrainObjectSystem(const std::shared_ptr<EntityManager>& entManager);
    TerrainObjectSystem(TerrainObjectSystem&) = delete;
    TerrainObjectSystem(TerrainObjectSystem&&) = delete;
    TerrainObjectSystem& operator=(TerrainObjectSystem&) = delete;
    TerrainObjectSystem& operator=(TerrainObjectSystem&&) = delete;

    ~TerrainObjectSystem();

    std::vector<std::pair<EntityID::UUID, std::string>> at(int row, int col) const;

    void loadObjects(const std::string& filepath);

    bool erase(EntityID::UUID ID);

    void objectCollisions(std::shared_ptr<CommandSystem>& cmdSys);

private:

    EntityID::UUID add(const std::string& name, int row, int col);

    std::vector<EntityID::UUID>    m_objects;
    std::shared_ptr<EntityManager> m_entManager;
};

}
