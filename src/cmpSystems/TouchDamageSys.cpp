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

TouchDamage_Sys::TouchDamage_Sys(const std::shared_ptr<EntityManager>& entManager) :
    BaseVisitor(entManager)
{}

BaseVisitor::ReturnedCommands TouchDamage_Sys::operator()(const cmd::Touch& cmd)
{
    ReturnedCommands ret;
    auto touchDamage = m_entManager->getComponent<comp::TouchDamage>(cmd.touched);
    if (touchDamage) {
        auto hp = m_entManager->getComponent<comp::HP>(cmd.touching);
        if (hp) {
            cmd::Damage cmdDmg;
            cmdDmg.dest = cmd.touching;
            cmdDmg.damage = touchDamage->damage;
            ret.push_back(cmdDmg);
        }
    }
    return ret;
}

