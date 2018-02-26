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
#include "ComponentSystem.hpp"
#include "src/TerrainMap.hpp"
#include "src/cmpSystems/TerrainObjectSystem.hpp"

using namespace cdwarfs::compSys;

ChangeTerrainType_Sys::ChangeTerrainType_Sys(
    const std::shared_ptr<EntityManager>& entManager,
    const std::shared_ptr<TerrainMap>& terrainMap,
    const std::shared_ptr<TerrainObjectSystem>& terrainObjSys
) :
    BaseVisitor(entManager),
    m_terrainMap(terrainMap),
    m_terrainObjSys(terrainObjSys)
{}

BaseVisitor::ReturnedCommands ChangeTerrainType_Sys::operator()(const cmd::ChangeTerrainType& cmd)
{
    ReturnedCommands ret;
    //TODO: Implement further checking if change is valid!
    m_terrainMap->set(cmd.row, cmd.col, cmd.newType);
    cmd::ChangeTileType newCmd;
    newCmd.row = cmd.row;
    newCmd.col = cmd.col;
    newCmd.newType = cmd.newType;
    ret.push_back(newCmd);

    return ret;
}
