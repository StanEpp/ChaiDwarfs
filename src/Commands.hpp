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

struct Touch {
    EntityID::UUID  touched{ 0 };
    EntityID::UUID  touching{ 0 };
};

struct Heal {
    EntityID::UUID dest{ 0 };
    int heal{ 0 };
};

struct Damage {
    EntityID::UUID dest{ 0 };
    int damage{ 0 };
};

struct Points {
    EntityID::UUID dest{ 0 };
    int points{ 0 };
};

struct MoveUp {
    EntityID::UUID dest{ 0 };
};

struct MoveDown {
    EntityID::UUID dest{ 0 };
};

struct MoveLeft {
    EntityID::UUID dest{ 0 };
};

struct MoveRight {
    EntityID::UUID dest{ 0 };
};

struct MoveNone {
    EntityID::UUID dest{ 0 };
};

struct MoveSpriteRight {
    EntityID::UUID dest{ 0 };
    int row{ 0 }, col{ 0 };
};

struct MoveSpriteLeft {
    EntityID::UUID dest{ 0 };
    int row{ 0 }, col{ 0 };
};

struct MoveSpriteUp {
    EntityID::UUID dest{ 0 };
    int row{ 0 }, col{ 0 };
};

struct MoveSpriteDown {
    EntityID::UUID dest{ 0 };
    int row{ 0 }, col{ 0 };
};

struct ChangeTerrainType {
    int row{ 0 }, col{ 0 };
    TerrainType newType{ TerrainType::SOIL };
};

struct ChangeTileType {
    int row{ 0 }, col{ 0 };
    TerrainType newType{ TerrainType::SOIL };
};

struct ExecuteAI {
    EntityID::UUID dest{ 0 };
};

struct ExecuteEveryAI { };

struct KillFlaggedEntities { };

using Command =
std::variant<
    Touch,
    Heal,
    Damage,
    Points,
    ChangeTerrainType,
    ChangeTileType,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    MoveNone,
    MoveSpriteRight,
    MoveSpriteLeft,
    MoveSpriteUp,
    MoveSpriteDown,
    ExecuteAI,
    ExecuteEveryAI,
    KillFlaggedEntities
>;

}
