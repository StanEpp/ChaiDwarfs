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
#pragma once

#include <variant>

#include "ecs/Entity.hpp"
#include "TerrainMap.hpp"

namespace cdwarfs::cmd
{

struct Cmd_Touch {
    EntityID::UUID  touched{ 0 };
    EntityID::UUID  touching{ 0 };
};

struct Cmd_Heal {
    EntityID::UUID dest{ 0 };
    int heal{ 0 };
};

struct Cmd_Damage {
    EntityID::UUID dest{ 0 };
    int damage{ 0 };
};

struct Cmd_Points {
    EntityID::UUID dest{ 0 };
    int points{ 0 };
};

struct Cmd_MoveUp {
    EntityID::UUID dest{ 0 };
};

struct Cmd_MoveDown {
    EntityID::UUID dest{ 0 };
};

struct Cmd_MoveLeft {
    EntityID::UUID dest{ 0 };
};

struct Cmd_MoveRight {
    EntityID::UUID dest{ 0 };
};

struct Cmd_MoveNone {
    EntityID::UUID dest{ 0 };
};

struct Cmd_MoveSpriteRight {
    EntityID::UUID dest{ 0 };
    int row{ 0 }, col{ 0 };
};

struct Cmd_MoveSpriteLeft {
    EntityID::UUID dest{ 0 };
    int row{ 0 }, col{ 0 };
};

struct Cmd_MoveSpriteUp {
    EntityID::UUID dest{ 0 };
    int row{ 0 }, col{ 0 };
};

struct Cmd_MoveSpriteDown {
    EntityID::UUID dest{ 0 };
    int row{ 0 }, col{ 0 };
};

struct Cmd_ChangeTerrainType {
    int row{ 0 }, col{ 0 };
    TerrainType newType{ TerrainType::SOIL };
};

struct Cmd_ChangeTileType {
    int row{ 0 }, col{ 0 };
    TerrainType newType{ TerrainType::SOIL };
};

struct Cmd_ExecuteAI {
    EntityID::UUID dest{ 0 };
};

struct Cmd_ExecuteEveryAI {
};

using Command =
std::variant<
    Cmd_Touch,
    Cmd_Heal,
    Cmd_Damage,
    Cmd_Points,
    Cmd_ChangeTerrainType,
    Cmd_ChangeTileType,
    Cmd_MoveUp,
    Cmd_MoveDown,
    Cmd_MoveLeft,
    Cmd_MoveRight,
    Cmd_MoveNone,
    Cmd_MoveSpriteRight,
    Cmd_MoveSpriteLeft,
    Cmd_MoveSpriteUp,
    Cmd_MoveSpriteDown,
    Cmd_ExecuteAI,
    Cmd_ExecuteEveryAI
>;

}
