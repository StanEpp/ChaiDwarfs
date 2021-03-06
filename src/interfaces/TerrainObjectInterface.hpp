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

namespace cdwarfs
{

class TerrainObjectSystem;

class TerrainObjectInterface final
{
public:
    TerrainObjectInterface() = delete;
    TerrainObjectInterface(const std::shared_ptr<TerrainObjectSystem>&);
    ~TerrainObjectInterface() {}

    const std::string checkForObject(int currRow, int currCol, int viewDist, int diffRow, int diffCol) const;
private:
    std::weak_ptr<TerrainObjectSystem> m_terrainObjSys;
};

}
