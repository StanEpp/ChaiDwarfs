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

#include <iostream>

#include "src/cmpSystems/TerrainObjectSystem.hpp"
#include "src/ecs/EntityManager.hpp"
#include "src/CommandSystem.hpp"

using namespace cdwarfs;

TerrainObjectSystem::TerrainObjectSystem(const std::shared_ptr<EntityManager>& entManager) :
    m_entManager(entManager)
{}

TerrainObjectSystem::~TerrainObjectSystem(){}

// TODO: Optimize via quadtree or create an array like in the terrainmap?
std::vector<std::pair<EntityID::UUID, std::string>> TerrainObjectSystem::at(int row, int col) const
{
    auto posEqual = [row, col](const comp::Position* pos) {
        if (pos->col == col && pos->row == row) return true;
        return false;
    };

    std::vector<std::pair<EntityID::UUID, std::string>> ret;

    for (auto objID : m_objects) {
        if (auto objptr = m_entManager->getComponent<comp::Position>(objID)) {
            std::string name = "Unknown";
            if (auto nameComp = m_entManager->getComponent<comp::Name>(objID)) name = nameComp->name;
            if (posEqual(objptr)) ret.push_back(std::make_pair(objID, name));
        }
    }

    return ret;
}

void TerrainObjectSystem::loadObjects(const std::string& filepath)
{
    chaiscript::ChaiScript chai;

    chai.add(chaiscript::var(std::ref(*this)), "terrainMap");
    chai.add(chaiscript::fun(&TerrainObjectSystem::add), "placeObject");

    chai.add(chaiscript::var(std::ref(*(m_entManager.get()))), "factory");
    chai.add(chaiscript::fun(&EntityManager::createObject), "createObject");

    chai.add(m_entManager->getChaiModuleForComponents());
    chai.add(m_entManager->getChaiModuleForEntityManager());
    chai.eval_file(filepath);
}

bool TerrainObjectSystem::erase(EntityID::UUID ID)
{
    auto ent = std::find(m_objects.cbegin(), m_objects.cend(), ID);
    if (ent == m_objects.cend()) return false;
    m_objects.erase(ent);
    return false;
}

// TODO: Improve runtime. Currently quadratic!
void TerrainObjectSystem::objectCollisions(std::shared_ptr<CommandSystem>& cmdSys)
{
    for (auto objID : m_objects) {
        auto pos = m_entManager->getComponent<comp::Position>(objID);
        if (!pos) continue;
        auto otherObj = at(pos->row, pos->col);

        for (auto otherID : otherObj) {
            if (objID == otherID.first) continue;
            cmd::Touch cmd;
            cmd.touched = otherID.first;
            cmd.touching = objID;
            cmdSys->pushCommand(cmd);
        }
    }
}

EntityID::UUID TerrainObjectSystem::add(const std::string& name, int row, int col)
{
    auto ID = m_entManager->createObject(name);
    auto pos = m_entManager->getComponent<comp::Position>(ID);
    if (!pos) throw std::runtime_error("Object " + name + " has no position component. Can't be placed on the terrain.");
    pos->row = row;
    pos->col = col;
    m_objects.push_back(ID);
    m_entManager->listenToEntityDestruction(ID, [this](EntityID::UUID killedID) {
        this->erase(killedID);
    });
    return ID;
}
