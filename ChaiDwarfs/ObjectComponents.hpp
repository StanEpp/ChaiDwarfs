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
*/
#ifndef _OBJECTCOMPONENTS_HPP_
#define _OBJECTCOMPONENTS_HPP_

#include "Component.hpp"

namespace CDwarfs {

  struct TouchValue : public ObjectComponent<TouchValue> {
    int value;
    TouchValue() : value(0) {}
    TouchValue(const int l_value) : value(l_value) {}
  };

  struct TouchDamage : public ObjectComponent<TouchDamage> {
    int damage;
    TouchDamage() : damage(0) {}
    TouchDamage(const int l_damage) : damage(l_damage) {}
  };

  struct Name : public ObjectComponent<Name> {
    std::string name;
    Name() : name() {}
    Name(const std::string& l_name) : name(l_name) {}
  };

  struct Position : public ObjectComponent<Position> {
    int row, col;
    Position() : row(0), col(0) {}
    Position(const int l_row, const int l_col) : row(l_row), col(l_col) {}
  };
}

#endif // !_OBJECTCOMPONENTS_HPP_
