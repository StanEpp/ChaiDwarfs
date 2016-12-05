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

#include "ObjectComponents.hpp"

#include <chaiscript\chaiscript.hpp>

namespace CDwarfs {
  namespace objDef {

    template<class TObjComp>
    void registerComponent(chaiscript::ModulePtr& module);

    template<>
    void registerComponent<TouchValue>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<TouchValue>(), "TouchValue");
      module->add(chaiscript::fun(&TouchValue::value), "value");
    }

    template<>
    void registerComponent<TouchDamage>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<TouchDamage>(), "TouchDamage");
      module->add(chaiscript::fun(&TouchDamage::damage), "damage");
    }

    template<>
    void registerComponent<Name>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<Name>(), "Name");
      module->add(chaiscript::fun(&Name::name), "name");
    }

    template<>
    void registerComponent<Position>(chaiscript::ModulePtr& module) {
      module->add(chaiscript::user_type<Position>(), "Position");
      module->add(chaiscript::fun(&Position::row), "row");
      module->add(chaiscript::fun(&Position::col), "col");
    }

  }
}

#endif // !_OBJECTCOMPONENTREGISTRATIONS_HPP_
