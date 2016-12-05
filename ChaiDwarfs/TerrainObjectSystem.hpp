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
#include"TerrainObject.hpp"

namespace CDwarfs {

  class TerrainObjectSystem {
  public:
    TerrainObjectSystem() = default;
    TerrainObjectSystem(TerrainObjectSystem&) = delete;
    TerrainObjectSystem(TerrainObjectSystem&&) = delete;
    TerrainObjectSystem& operator=(TerrainObjectSystem&) = delete;
    TerrainObjectSystem& operator=(TerrainObjectSystem&&) = delete;

    ~TerrainObjectSystem() {
      std::for_each(m_objects.begin(), m_objects.end(), [](auto ptr) {
        if (ptr) delete ptr;
      });
    }

    // TODO: Optimize via quadtree
    std::vector<TerrainObject*> at(int x, int y) const {
      auto posEqual = [x, y](const Point& pos) {
        if (pos.x() == x && pos.y() == y) { return true; }
        else { return false; }
      };

      std::vector<TerrainObject*> ret;

      for (auto ptr : m_objects) {
        if (posEqual(ptr->pos())) ret.push_back(ptr);
      }

      return ret;
    }

    template<class T, class... Args>
    T& add(Args&&... params) {
      auto ID = m_objects.size();
      auto ptr = new T(ID, std::forward<Args>(params)...);
      m_objects.push_back(ptr);
      return *ptr;
    }

    bool erase(ObjectID ID) {
      if (ID >= m_objects.size()) return false;
      if (m_objects[ID]) delete m_objects[ID];
      m_objects.erase(m_objects.begin() + ID);
    }

  private:
    std::vector<TerrainObject*>  m_objects;
  };

}

#endif // !_TERRAINOBJECTSYSTEM_HPP_
