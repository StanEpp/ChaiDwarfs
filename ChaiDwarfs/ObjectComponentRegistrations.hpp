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
#ifndef _OBJECTCOMPONENTREGISTRATIONS_HPP_
#define _OBJECTCOMPONENTREGISTRATIONS_HPP_

#include <string>
#include <chaiscript\chaiscript.hpp>

#include "Components.hpp"

namespace cdwarfs {
  namespace objDef {

    using namespace cdwarfs::comp;

    template<class TObjComp>
    void inline registerComponent(chaiscript::ModulePtr& module);

    template<>
    void inline registerComponent<TouchValue>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<TouchValue>(), "TouchValue");
      module->add(chaiscript::fun(&TouchValue::value), "value");
    }

    template<>
    void inline registerComponent<TouchHeal>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<TouchHeal>(), "TouchHeal");
      module->add(chaiscript::fun(&TouchHeal::heal), "heal");
    }

    template<>
    void inline registerComponent<TouchDamage>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<TouchDamage>(), "TouchDamage");
      module->add(chaiscript::fun(&TouchDamage::damage), "damage");
    }

    template<>
    void inline registerComponent<Name>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<Name>(), "Name");
      module->add(chaiscript::fun(&Name::name), "name");
    }

    template<>
    void inline registerComponent<Position>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<Position>(), "Position");
      module->add(chaiscript::fun(&Position::row), "row");
      module->add(chaiscript::fun(&Position::col), "col");
    }

    template<>
    void inline registerComponent<HP>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<HP>(), "HP");
      module->add(chaiscript::fun(&HP::hp), "hp");
    }

    template<>
    void inline registerComponent<View>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<View>(), "View");
      module->add(chaiscript::fun(&View::dist), "dist");
    }

    template<>
    void inline registerComponent<Speed>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<Speed>(), "Speed");
      module->add(chaiscript::fun(&Speed::speed), "speed");
    }

    template<>
    void inline registerComponent<Points>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<Points>(), "Points");
      module->add(chaiscript::fun(&Points::points), "points");
    }

    template<>
    void inline registerComponent<TouchDestroy>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<TouchDestroy>(), "DestroyOnTouch");
    }

    template<>
    void inline registerComponent<ScriptAI>(chaiscript::ModulePtr&) {}

    template<>
    void inline registerComponent<Sprites>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<Sprites>(), "Sprites");
      module->add(chaiscript::fun(&Sprites::sprites), "sprites");
    }

    template<>
    void inline registerComponent<AnimatedSprites>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<AnimatedSprites>(), "AnimatedSprites");
      module->add(chaiscript::fun(&AnimatedSprites::sprites), "sprites");
      module->add(chaiscript::fun(&AnimatedSprites::duration), "duration");
    }

    void inline registerConversions(chaiscript::ModulePtr& module) {
      module->add(chaiscript::type_conversion<std::vector<chaiscript::Boxed_Value>, std::vector<std::pair<std::string, std::string>>>(
        [](const std::vector<chaiscript::Boxed_Value>& vs) {
        std::vector<std::pair<std::string, std::string>> retVec;
        std::transform(vs.begin(), vs.end(), std::back_inserter(retVec), [](const chaiscript::Boxed_Value &bv) {
          auto boxedPair = chaiscript::boxed_cast<std::pair<chaiscript::Boxed_Value, chaiscript::Boxed_Value>>(bv);
          auto firstStr = chaiscript::boxed_cast<std::string>(boxedPair.first);
          auto secondStr = chaiscript::boxed_cast<std::string>(boxedPair.second);
          return std::make_pair(std::move(firstStr), std::move(secondStr));
        });
        return retVec;
      }));
    }

  }
}

#endif // !_OBJECTCOMPONENTREGISTRATIONS_HPP_
