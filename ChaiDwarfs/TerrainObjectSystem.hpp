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
#ifndef _TERRAINOBJECTSYSTEM_HPP_
#define _TERRAINOBJECTSYSTEM_HPP_

#include <vector>
#include <algorithm>
#include <iostream>
#include "Entity.hpp"
#include "Components.hpp"
#include "ComponentSystem.hpp"

namespace CDwarfs {

  class TerrainObjectSystem {
  public:
    TerrainObjectSystem() = delete;
    TerrainObjectSystem(const std::shared_ptr<EntityManager> entManager) : m_entManager(entManager) {}
    TerrainObjectSystem(TerrainObjectSystem&) = delete;
    TerrainObjectSystem(TerrainObjectSystem&&) = delete;
    TerrainObjectSystem& operator=(TerrainObjectSystem&) = delete;
    TerrainObjectSystem& operator=(TerrainObjectSystem&&) = delete;

    ~TerrainObjectSystem() { }

    // TODO: Optimize via quadtree
    std::vector<std::pair<EntityID::UUID, std::string>> at(int x, int y) const {
      auto posEqual = [x, y](const comp::Position* pos) {
        if (pos->col == x && pos->row == y) { return true; }
        else { return false; }
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

    void loadObjects(const std::string& filepath) {
      chaiscript::ChaiScript chai;

      chai.add(chaiscript::var(std::ref(*this)), "terrainMap");
      chai.add(chaiscript::fun(&TerrainObjectSystem::add), "addObject");

      chai.add(chaiscript::var(std::ref(*(m_entManager.get()))), "factory");
      chai.add(chaiscript::fun(&EntityManager::createObject), "createObject");

      chai.add(m_entManager->getChaiModuleForComponents());
      chai.add(m_entManager->getChaiModuleForEntityManager());
      chai.eval_file(filepath);
    }

    bool erase(EntityID::UUID ID) {
      auto ent = std::find(m_objects.cbegin(), m_objects.cend(), ID);
      if (ent == m_objects.cend()) return false;
      m_objects.erase(ent);
      return true;
    }

  private:

    EntityID::UUID add(const std::string& name, int row, int col) {
      auto ID = m_entManager->createObject(name);
      auto pos = m_entManager->getComponent<comp::Position>(ID);
      if (!pos) throw std::runtime_error("Object " + name + " has no position component. Can't be placed on the terrain.");
      pos->row = row; 
      pos->col = col;
      m_objects.push_back(ID);
      return ID;
    }

    std::vector<EntityID::UUID>  m_objects;
    std::shared_ptr<EntityManager> m_entManager;
  };

}

#endif // !_TERRAINOBJECTSYSTEM_HPP_
