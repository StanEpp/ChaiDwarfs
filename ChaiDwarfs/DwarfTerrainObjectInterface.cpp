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

#include "DwarfTerrainObjectInterface.hpp"
#include "TerrainObjectSystem.hpp"

using namespace CDwarfs;

DwarfTerrainObjectInterface::DwarfTerrainObjectInterface(const std::shared_ptr<TerrainObjectSystem>& terrainObjSys) : 
  m_terrainObjSys(terrainObjSys){}

const std::string& DwarfTerrainObjectInterface::checkForObject(int currRow, int currCol, int viewDist, int diffRow, int diffCol) const {
  if (m_terrainObjSys.expired()) return "Unknown";
  auto sys = m_terrainObjSys.lock();

  if (std::abs(diffRow) > viewDist || std::abs(diffCol) > viewDist ||
      currRow + diffRow < 0 || currCol + diffCol < 0)
  {
    return "OutOfReach";
  }

  auto objects = sys->at(currRow + diffRow, currRow + diffCol);
  if (objects.empty()) return "Nothing";
  return objects[0].second;
}