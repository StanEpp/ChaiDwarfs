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
#ifndef _COMMANDS_HPP_
#define _COMMANDS_HPP_

#include <variant>
#include "Entity.hpp"

namespace CDwarfs {
  namespace cmd {
    struct Cmd_Touch {
      EntityID::UUID  touched;
      EntityID::UUID  touching;
    };

    struct Cmd_Heal {
      EntityID::UUID dest;
      int heal;
    };

    struct Cmd_Damage {
      EntityID::UUID dest;
      int damage;
    };

    struct Cmd_Points {
      EntityID::UUID dest;
      int points;
    };

    struct Cmd_MoveUp {
      EntityID::UUID dest;
    };

    struct Cmd_MoveDown {
      EntityID::UUID dest;
    };

    struct Cmd_MoveLeft {
      EntityID::UUID dest;
    };

    struct Cmd_MoveRight {
      EntityID::UUID dest;
    };

    struct Cmd_MoveNone {
      EntityID::UUID dest;
    };

    using Command = std::variant<
      Cmd_Touch, Cmd_Heal, Cmd_Damage, Cmd_Points,
      Cmd_MoveUp, Cmd_MoveDown, Cmd_MoveLeft, Cmd_MoveRight, Cmd_MoveNone>;
  }
}

#endif // !_COMMANDS_HPP_
