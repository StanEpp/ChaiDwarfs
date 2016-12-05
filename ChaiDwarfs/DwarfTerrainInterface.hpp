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

#ifndef _DWARFTERRAININTERFACE_HPP_
#define _DWARFTERRAININTERFACE_HPP_

#include <memory>

namespace CDwarfs {
  class Dwarf;
  class Point;
  class TerrainMap;
  enum class TerrainType : unsigned int;

  class DwarfTerrainInterface final{
  public:
    DwarfTerrainInterface() = delete;
    DwarfTerrainInterface(const std::shared_ptr<TerrainMap>& terrainMap);
    ~DwarfTerrainInterface(){}

    const TerrainType checkTerrain(const Dwarf&, int x, int y) const;
  private:
    std::weak_ptr<TerrainMap> m_terrain;
  };


}

#endif // !_DWARFTERRAININTERFACE_HPP_
