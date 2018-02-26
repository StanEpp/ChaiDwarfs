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

using namespace cdwarfs::compSys;

Damage_Sys::Damage_Sys(const std::shared_ptr<EntityManager>& entManager) :
    BaseVisitor(entManager)
{}

BaseVisitor::ReturnedCommands Damage_Sys::operator()(const cmd::Damage& cmd)
{
    auto hp = m_entManager->getComponent<comp::HP>(cmd.dest);
    if (hp) {
        hp->hp -= cmd.damage;
    }
    return ReturnedCommands();
}
