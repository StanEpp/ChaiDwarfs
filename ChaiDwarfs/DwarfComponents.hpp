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
#ifndef _DWARFCOMPONENTS_HPP_
#define _DWARFCOMPONENTS_HPP_

#include "Component.hpp"

namespace CDwarfs {

  struct DwarfHP : public DwarfComponent<DwarfHP> {
    int HP;
    int maxHP;
    DwarfHP() : HP(0), maxHP(100) {}
    DwarfHP(const int HP, const int maxHP) : HP(HP), maxHP(maxHP){}
  };

  struct DwarfPosition : public DwarfComponent<DwarfPosition> {
    int row, col;
    DwarfPosition() : row(0), col(0) {}
    DwarfPosition(const int l_row, const int l_col) : row(l_row), col(l_col) {}
  };

  struct DwarfSpeed : public DwarfComponent<DwarfPosition> {
    int speed;
    DwarfSpeed() : speed(0) {}
    DwarfSpeed(const int speed) : speed(speed) {}
  };

  struct DwarfPoints : public DwarfComponent<DwarfPoints> {
    int points;
    DwarfPoints() : points(0) {}
    DwarfPoints(const int points) : points(points) {}
  };

  struct DwarfView : public DwarfComponent<DwarfView> {
    int dist;
    DwarfView() : dist(10) {}
    DwarfView(const int dist) : dist(dist) {}
  };
}

#endif // !_DWARFCOMPONENTS_HPP_
