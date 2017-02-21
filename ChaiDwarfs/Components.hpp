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

#include <memory>
#include <string>
#include <vector>

#include "BaseComponent.hpp"
#include "DwarfAI.hpp"

namespace CDwarfs {
  namespace comp {

    struct FlaggedDestroyed : public Component<FlaggedDestroyed> {
      FlaggedDestroyed();
    };

    struct TouchDestroy : public Component<TouchDestroy> {
      TouchDestroy();
    };

    struct TouchValue : public Component<TouchValue> {
      int value;
      TouchValue();
      TouchValue(int l_value);
    };

    struct TouchHeal : public Component<TouchHeal> {
      int heal;
      TouchHeal();
      TouchHeal(int l_heal);
    };

    struct TouchDamage : public Component<TouchDamage> {
      int damage;
      TouchDamage();
      TouchDamage(int l_damage);
    };

    struct Name : public Component<Name> {
      std::string name;
      Name();
      Name(const std::string& l_name);
    };

    struct Position : public Component<Position> {
      int row, col;
      Position();
      Position(int l_row, int l_col);
    };

    struct HP : public Component<HP> {
      int hp;
      int maxHP;
      HP();
      HP(int hp, int maxHP);
    };

    struct Speed : public Component<Speed> {
      int speed;
      Speed();
      Speed(int speed);
    };

    struct Points : public Component<Points> {
      int points;
      Points();
      Points(int points);
    };

    struct View : public Component<View> {
      int dist;
      View();
      View(int dist);
    };

    struct AIComponent : public Component<AIComponent>{
      std::unique_ptr<DwarfAI> ai;
      AIComponent() = delete;
      AIComponent(std::unique_ptr<DwarfAI>&& ai);
    };

    struct ScriptAI : public AIComponent {
      std::string filePath;
      ScriptAI();
      ScriptAI(const std::string& filePath);
    };

    struct Sprites : public Component<Sprites> {
      std::vector<std::pair<std::string, std::string>> sprites;
      Sprites() = default;
      Sprites(const std::vector<std::pair<std::string, std::string>>& filePaths);
    };

    struct AnimatedSprites : public Component<AnimatedSprites> {
      std::vector<std::pair<std::string, std::string>> sprites;
      // These data get filled by the SpriteRenderer upon loading the images
      // playing, duration, currTime, numKeyframes
      std::vector<std::tuple<bool, float, float, int>> animationState;

      AnimatedSprites() = default;
      AnimatedSprites(const std::vector<std::pair<std::string, std::string>>& filePaths);
    };

  }
}

#endif // !_COMPONENTS_HPP_
