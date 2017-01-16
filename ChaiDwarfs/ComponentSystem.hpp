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
#ifndef _COMPONENTSYSTEM_HPP_
#define _COMPONENTSYSTEM_HPP_

#include <iostream>
#include <vector>
#include "Commands.hpp"
#include "EntityManager.hpp"
#include "Components.hpp"
#include "TerrainMap.hpp"

namespace CDwarfs {
  namespace compSys {

    class BaseVisitor {
    protected:
      using ReturnedCommands = std::vector<cmd::Command>;

    public:
      BaseVisitor() = delete;
      BaseVisitor(const std::shared_ptr<EntityManager>& entManager) : m_entManager(entManager) {}

      virtual ~BaseVisitor() {}

      virtual ReturnedCommands operator()(const cmd::Cmd_Touch&) { return ReturnedCommands(); }
      virtual ReturnedCommands operator()(const cmd::Cmd_Heal&) { return ReturnedCommands(); }
      virtual ReturnedCommands operator()(const cmd::Cmd_Damage&) { return ReturnedCommands(); }
      virtual ReturnedCommands operator()(const cmd::Cmd_Points&) { return ReturnedCommands(); }
      virtual ReturnedCommands operator()(const cmd::Cmd_MoveUp&) { return ReturnedCommands(); }
      virtual ReturnedCommands operator()(const cmd::Cmd_MoveDown&) { return ReturnedCommands(); }
      virtual ReturnedCommands operator()(const cmd::Cmd_MoveLeft&) { return ReturnedCommands(); }
      virtual ReturnedCommands operator()(const cmd::Cmd_MoveRight&) { return ReturnedCommands(); }

    protected:
      std::shared_ptr<EntityManager> m_entManager;
    };


    class TouchHeal_Sys : public BaseVisitor {
    public:
      TouchHeal_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

      virtual ReturnedCommands operator()(const cmd::Cmd_Touch& cmd) {
        std::cout << "TouchHeal_Sys" << std::endl;
        ReturnedCommands ret;
        auto touchHeal = m_entManager->getComponent<comp::TouchHeal>(cmd.touched);
        if (touchHeal) {
          auto hp = m_entManager->getComponent<comp::HP>(cmd.touching);
          if (hp) {
            cmd::Cmd_Heal cmdHeal;
            cmdHeal.dest = cmd.touching;
            cmdHeal.heal = touchHeal->heal;
            ret.push_back(cmdHeal);
          }
        }
        return ret;
      }
    };

    class TouchDamage_Sys : public BaseVisitor {
    public:
      TouchDamage_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

      virtual ReturnedCommands operator()(const cmd::Cmd_Touch& cmd) {
        std::cout << "TouchDamage_Sys" << std::endl;
        ReturnedCommands ret;
        auto touchDamage = m_entManager->getComponent<comp::TouchDamage>(cmd.touched);
        if (touchDamage) {
          auto hp = m_entManager->getComponent<comp::HP>(cmd.touching);
          if (hp) {
            cmd::Cmd_Damage cmdDmg;
            cmdDmg.dest = cmd.touching;
            cmdDmg.damage = touchDamage->damage;
            ret.push_back(cmdDmg);
          }
        }
        ret.push_back(cmd::Cmd_Damage());
        return ret;
      }
    };

    class Damage_Sys : public BaseVisitor {
    public:
      Damage_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

      virtual ReturnedCommands operator()(const cmd::Cmd_Damage& cmd) {
        std::cout << "Damage_Sys" << std::endl;
        auto hp = m_entManager->getComponent<comp::HP>(cmd.dest);
        if (hp) hp->hp -= cmd.damage;
        return ReturnedCommands();
      }
    };

    class Move_Sys : public BaseVisitor {
    public:
      Move_Sys(const std::shared_ptr<EntityManager>& entManager, const std::shared_ptr<TerrainMap> terrainMap) : 
        BaseVisitor(entManager), m_terrainMap(terrainMap) {}

      virtual ReturnedCommands operator()(const cmd::Cmd_MoveUp& cmd) {
        return executeMove(-1, 0, cmd.dest);
      }

      virtual ReturnedCommands operator()(const cmd::Cmd_MoveDown& cmd) {
        return executeMove(1, 0, cmd.dest);
      }

      virtual ReturnedCommands operator()(const cmd::Cmd_MoveLeft& cmd) {
        return executeMove(0, -1, cmd.dest);
      }

      virtual ReturnedCommands operator()(const cmd::Cmd_MoveRight& cmd) {
        return executeMove(0, 1, cmd.dest);
      }

    private:
      ReturnedCommands executeMove(int rowDiff, int colDiff, EntityID::UUID entID) {
        std::cout << "Move_Sys" << std::endl;
        auto pos = m_entManager->getComponent<comp::Position>(entID);
        if (pos) {
          if (m_terrainMap->at(pos->row + rowDiff, pos->col + colDiff) == TerrainType::PASSABLE) {
            pos->row += rowDiff;
            pos->col += colDiff;
          }
        }
        return ReturnedCommands();
      }

      std::shared_ptr<TerrainMap> m_terrainMap;
    };

  }
}

#endif // !_COMPONENTSYSTEM_HPP_

