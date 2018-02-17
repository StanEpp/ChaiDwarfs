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

#include <iostream>

#include "src/interfaces/DwarfTerrainInterface.hpp"
#include "src/TerrainMap.hpp"

using namespace cdwarfs;

DwarfTerrainInterface::DwarfTerrainInterface(const std::shared_ptr<TerrainMap>& terrainMap) :
  m_terrain(terrainMap) {}

TerrainType DwarfTerrainInterface::checkTerrain(int currRow, int currCol,
                                                int viewDist,
                                                int diffRow, int diffCol) const
{
    if (m_terrain.expired()) return TerrainType::NO_MAP;

    auto map = m_terrain.lock();

    if (std::abs(diffRow) > viewDist || std::abs(diffCol) > viewDist ||
        currRow + diffRow < 0 || currRow + diffRow >= static_cast<int>(map->columns()) ||
        currCol + diffCol < 0 || currCol + diffCol >= static_cast<int>(map->rows()) )
    {
        return TerrainType::DARK;
    }

    return map->at(currRow + diffRow, currCol + diffCol);
}
