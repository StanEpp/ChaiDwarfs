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
#include "src/ecs/Components.hpp"
#include "TerrainObjectSystem.hpp"

using namespace cdwarfs::compSys;
using namespace cdwarfs;

Move_Sys::Move_Sys(
    const std::shared_ptr<EntityManager>& entManager,
    const std::shared_ptr<TerrainMap>& terrainMap,
    const std::shared_ptr<TerrainObjectSystem>& terrainObjSys
) :
    BaseVisitor(entManager),
    m_terrainMap(terrainMap),
    m_terrainObjSys(terrainObjSys)
{}

BaseVisitor::ReturnedCommands Move_Sys::operator()(const cmd::MoveUp& cmd)
{
    ReturnedCommands ret;
    auto newPos = executeMove(ret, -1, 0, cmd.dest);
    executeSpriteMove<cmd::MoveSpriteUp>(ret, cmd.dest, newPos);
    return ret;
}

BaseVisitor::ReturnedCommands Move_Sys::operator()(const cmd::MoveDown& cmd)
{
    ReturnedCommands ret;
    auto newPos = executeMove(ret, 1, 0, cmd.dest);
    executeSpriteMove<cmd::MoveSpriteDown>(ret, cmd.dest, newPos);
    return ret;
}

BaseVisitor::ReturnedCommands Move_Sys::operator()(const cmd::MoveLeft& cmd)
{
    ReturnedCommands ret;
    auto newPos = executeMove(ret, 0, -1, cmd.dest);
    executeSpriteMove<cmd::MoveSpriteLeft>(ret, cmd.dest, newPos);
    return ret;
}

BaseVisitor::ReturnedCommands Move_Sys::operator()(const cmd::MoveRight& cmd)
{
    ReturnedCommands ret;
    auto newPos = executeMove(ret, 0, 1, cmd.dest);
    executeSpriteMove<cmd::MoveSpriteRight>(ret, cmd.dest, newPos);
    return ret;
}

BaseVisitor::ReturnedCommands Move_Sys::operator()(const cmd::MoveNone& cmd)
{
    ReturnedCommands ret;
    executeMove(ret, 0, 0, cmd.dest);
    return ret;
}

std::optional<comp::Position*> Move_Sys::executeMove(ReturnedCommands& ret,
                                                     int rowDiff, int colDiff,
                                                     EntityID::UUID entID)
{
    auto pos = m_entManager->getComponent<comp::Position>(entID);
    if (pos) {
        if (m_terrainMap->at(pos->row + rowDiff, pos->col + colDiff) == TerrainType::PASSABLE) {
            auto objects = m_terrainObjSys->at(pos->row + rowDiff, pos->col + colDiff);
            for (auto& ent : objects) {
                cmd::Touch touchCmd;
                touchCmd.touched = ent.first;
                touchCmd.touching = entID;
                ret.push_back(touchCmd);
            }
            pos->row += rowDiff;
            pos->col += colDiff;
            return pos;
        } else {
            cmd::MoveNone standCmd;
            standCmd.dest = entID;
            ret.push_back(standCmd);
        }
    }
    return {};
}
