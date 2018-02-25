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
*
*  Description:
*  Every new commponent needs to specialise the registerComponent template
*  and provide an exposure to its type and members to the chaiscript module.
*/
#pragma once

#include <string>
#include <chaiscript/chaiscript.hpp>

#include "ecs/Components.hpp"
#include "ecs/ComponentsAI.hpp"

namespace cdwarfs::objDef
{

template<class TObjComp>
void inline registerComponent(chaiscript::ModulePtr& module);

template<>
void inline registerComponent<cdwarfs::comp::TouchValue>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::TouchValue>(), "TouchValue");
    module->add(chaiscript::fun(&cdwarfs::comp::TouchValue::value), "value");
}

template<>
void inline registerComponent<cdwarfs::comp::TouchHeal>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::TouchHeal>(), "TouchHeal");
    module->add(chaiscript::fun(&cdwarfs::comp::TouchHeal::heal), "heal");
}

template<>
void inline registerComponent<cdwarfs::comp::TouchDamage>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::TouchDamage>(), "TouchDamage");
    module->add(chaiscript::fun(&cdwarfs::comp::TouchDamage::damage), "damage");
}

template<>
void inline registerComponent<cdwarfs::comp::Name>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::Name>(), "Name");
    module->add(chaiscript::fun(&cdwarfs::comp::Name::name), "name");
}

template<>
void inline registerComponent<cdwarfs::comp::Position>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::Position>(), "Position");
    module->add(chaiscript::fun(&cdwarfs::comp::Position::row), "row");
    module->add(chaiscript::fun(&cdwarfs::comp::Position::col), "col");
}

template<>
void inline registerComponent<cdwarfs::comp::HP>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::HP>(), "HP");
    module->add(chaiscript::fun(&cdwarfs::comp::HP::hp), "hp");
}

template<>
void inline registerComponent<cdwarfs::comp::View>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::View>(), "View");
    module->add(chaiscript::fun(&cdwarfs::comp::View::dist), "dist");
}

template<>
void inline registerComponent<cdwarfs::comp::Speed>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::Speed>(), "Speed");
    module->add(chaiscript::fun(&cdwarfs::comp::Speed::speed), "speed");
}

template<>
void inline registerComponent<cdwarfs::comp::Points>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::Points>(), "Points");
    module->add(chaiscript::fun(&cdwarfs::comp::Points::points), "points");
}

template<>
void inline registerComponent<cdwarfs::comp::TouchDestroy>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::TouchDestroy>(), "DestroyOnTouch");
}

template<>
void inline registerComponent<cdwarfs::comp::DwarfAI>(chaiscript::ModulePtr&) {}

template<>
void inline registerComponent<cdwarfs::comp::Sprites>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::Sprites>(), "Sprites");
    module->add(chaiscript::fun(&cdwarfs::comp::Sprites::sprites), "sprites");
}

template<>
void inline registerComponent<cdwarfs::comp::AnimatedSprites>(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::user_type<cdwarfs::comp::AnimatedSprites>(), "AnimatedSprites");
    module->add(chaiscript::fun(&cdwarfs::comp::AnimatedSprites::sprites), "sprites");
}

void inline registerConversions(chaiscript::ModulePtr& module)
{
    module->add(chaiscript::type_conversion<std::vector<chaiscript::Boxed_Value>, std::vector<std::pair<std::string, std::string>>>(
        [](const std::vector<chaiscript::Boxed_Value>& vs) {
            std::vector<std::pair<std::string, std::string>> retVec;
            std::transform(vs.begin(), vs.end(), std::back_inserter(retVec),
                [](const chaiscript::Boxed_Value &bv) {
                    auto boxedPair = chaiscript::boxed_cast<std::pair<chaiscript::Boxed_Value, chaiscript::Boxed_Value>>(bv);
                    auto firstStr = chaiscript::boxed_cast<std::string>(boxedPair.first);
                    auto secondStr = chaiscript::boxed_cast<std::string>(boxedPair.second);
                    return std::make_pair(std::move(firstStr), std::move(secondStr));
                }
            );
            return retVec;
        }
    ));
}

}
