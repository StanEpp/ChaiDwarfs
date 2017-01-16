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
#ifndef _DWARFROUNDACTIONS_HPP_
#define _DWARFROUNDACTIONS_HPP_

namespace CDwarfs {

  class DwarfRoundActions {
  public:
    enum class MoveDirection {
      LEFT, RIGHT, UP, DOWN, NONE
    };

    DwarfRoundActions() : movement(MoveDirection::NONE) {}

    void moveUp()    { movement = MoveDirection::UP; }
    void moveDown()  { movement = MoveDirection::DOWN; }
    void moveRight() { movement = MoveDirection::RIGHT; }
    void moveLeft()  { movement = MoveDirection::LEFT; }
    void moveStop()  { movement = MoveDirection::NONE; }

    void clear() {
      movement = MoveDirection::NONE;
    }

    MoveDirection movement;

  };
}

#endif // !_DWARFROUNDACTIONS_HPP_
