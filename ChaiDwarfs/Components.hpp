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
#ifndef _COMPONENTS_HPP_
#define _COMPONENTS_HPP_

#include "BaseComponent.hpp"

namespace CDwarfs {
  namespace comp {

    struct TouchValue : public Component<TouchValue> {
      int value;
      TouchValue() : value(0) {}
      TouchValue(int l_value) : value(l_value) {}
    };

    struct TouchHeal : public Component<TouchHeal> {
      int heal;
      TouchHeal() : heal(0) {}
      TouchHeal(int l_heal) : heal(l_heal) {}
    };

    struct TouchDamage : public Component<TouchDamage> {
      int damage;
      TouchDamage() : damage(0) {}
      TouchDamage(int l_damage) : damage(l_damage) {}
    };

    struct Name : public Component<Name> {
      std::string name;
      Name() : name() {}
      Name(const std::string& l_name) : name(l_name) {}
    };

    struct Position : public Component<Position> {
      int row, col;
      Position() : row(0), col(0) {}
      Position(int l_row, int l_col) : row(l_row), col(l_col) {}
    };

    struct HP : public Component<HP> {
      int hp;
      int maxHP;
      HP() : hp(0), maxHP(100) {}
      HP(int hp, int maxHP) : hp(hp), maxHP(maxHP) {}
    };

    struct Speed : public Component<Speed> {
      int speed;
      Speed() : speed(0) {}
      Speed(int speed) : speed(speed) {}
    };

    struct Points : public Component<Points> {
      int points;
      Points() : points(0) {}
      Points(int points) : points(points) {}
    };

    struct View : public Component<View> {
      int dist;
      View() : dist(10) {}
      View(int dist) : dist(dist) {}
    };
  }
}

#endif // !_COMPONENTS_HPP_
