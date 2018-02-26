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

TouchHeal_Sys::TouchHeal_Sys(const std::shared_ptr<EntityManager>& entManager) :
    BaseVisitor(entManager)
{}

BaseVisitor::ReturnedCommands TouchHeal_Sys::operator()(const cmd::Touch& cmd)
{
    ReturnedCommands ret;
    auto touchHeal = m_entManager->getComponent<comp::TouchHeal>(cmd.touched);
    if (touchHeal) {
        auto hp = m_entManager->getComponent<comp::HP>(cmd.touching);
        if (hp) {
            cmd::Heal cmdHeal;
            cmdHeal.dest = cmd.touching;
            cmdHeal.heal = touchHeal->heal;
            ret.push_back(cmdHeal);
        }
    }
    return ret;
}
