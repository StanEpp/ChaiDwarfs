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
#include "PathResolver.hpp"

namespace cdwarfs::comp
{

FlaggedDestroyed::FlaggedDestroyed() {}

TouchDestroy::TouchDestroy() {}

TouchValue::TouchValue() {}
TouchValue::TouchValue(int l_value) : value(l_value) {}

TouchHeal::TouchHeal() {}
TouchHeal::TouchHeal(int l_heal) : heal(l_heal) {}

TouchDamage::TouchDamage() {}
TouchDamage::TouchDamage(int l_damage) : damage(l_damage) {}

Name::Name() {}
Name::Name(const std::string& l_name) : name(l_name) {}

Position::Position() {}
Position::Position(int l_row, int l_col) : row(l_row), col(l_col) {}

HP::HP() {}
HP::HP(int hp, int maxHP) : hp(hp), maxHP(maxHP) {}

Speed::Speed() {}
Speed::Speed(int speed) : speed(speed) {}

Points::Points() {}
Points::Points(int points) : points(points) {}

View::View() {}
View::View(int dist) : dist(dist) {}

Sprites::Sprites(const std::vector<std::pair<std::string, std::string>>& filePaths) {
    sprites.reserve(filePaths.size());
    for (const auto &p : filePaths) {
        sprites.emplace_back(p.first, pathRes.sprite(p.second));
    }
}

AnimatedSprites::AnimatedSprites(const std::vector<std::pair<std::string, std::string>>& filePaths) {
    sprites.reserve(filePaths.size());
    for (const auto &p : filePaths) {
        sprites.emplace_back(p.first, pathRes.sprite(p.second));
    }
}

}
