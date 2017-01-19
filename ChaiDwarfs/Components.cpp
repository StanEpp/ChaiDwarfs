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

#include "Components.hpp"
#include "ChaiScriptAI.hpp"

namespace CDwarfs {
  namespace comp {

    FlaggedDestroyed::FlaggedDestroyed() {}

    TouchDestroy::TouchDestroy() {}

    TouchValue::TouchValue() : value(0) {}
    TouchValue::TouchValue(int l_value) : value(l_value) {}

    TouchHeal::TouchHeal() : heal(0) {}
    TouchHeal::TouchHeal(int l_heal) : heal(l_heal) {}

    TouchDamage::TouchDamage() : damage(0) {}
    TouchDamage::TouchDamage(int l_damage) : damage(l_damage) {}

    Name::Name() : name() {}
    Name::Name(const std::string& l_name) : name(l_name) {}

    Position::Position() : row(0), col(0) {}
    Position::Position(int l_row, int l_col) : row(l_row), col(l_col) {}

    HP::HP() : hp(0), maxHP(100) {}
    HP::HP(int hp, int maxHP) : hp(hp), maxHP(maxHP) {}

    Speed::Speed() : speed(0) {}
    Speed::Speed(int speed) : speed(speed) {}

    Points::Points() : points(0) {}
    Points::Points(int points) : points(points) {}

    View::View() : dist(10) {}
    View::View(int dist) : dist(dist) {}

    AIComponent::AIComponent(std::unique_ptr<DwarfAI>&& ai) : ai(std::move(ai)) {}

    ScriptAI::ScriptAI() : AIComponent(std::make_unique<ChaiscriptAI>("")), filePath("") {}
    ScriptAI::ScriptAI(const std::string& filePath) : AIComponent(std::make_unique<ChaiscriptAI>(filePath)), filePath(filePath) {}

  }
}