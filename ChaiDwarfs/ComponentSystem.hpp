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
#include "TerrainObjectSystem.hpp"
#include "TileRenderer.hpp"
#include "SpriteRenderer.hpp"

namespace cdwarfs::compSys {

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
    virtual ReturnedCommands operator()(const cmd::Cmd_MoveNone&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::Cmd_ChangeTerrainType&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::Cmd_ChangeTileType&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::Cmd_MoveSpriteLeft&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::Cmd_MoveSpriteRight&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::Cmd_MoveSpriteUp&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::Cmd_MoveSpriteDown&) { return ReturnedCommands(); }

  protected:
    std::shared_ptr<EntityManager> m_entManager;
  };

  class TouchValue_Sys : public BaseVisitor {
  public:
    TouchValue_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

    virtual ReturnedCommands operator()(const cmd::Cmd_Touch& cmd) override {
      //std::cout << "TouchValue_Sys\n";
      ReturnedCommands ret;
      auto touchValue = m_entManager->getComponent<comp::TouchValue>(cmd.touched);
      if (touchValue) {
        auto points = m_entManager->getComponent<comp::Points>(cmd.touching);
        if (points) {
          //std::cout << " exec\n";
          cmd::Cmd_Points cmdPoints;
          cmdPoints.dest = cmd.touching;
          cmdPoints.points = touchValue->value;
          ret.push_back(cmdPoints);
        }
      }
      return ret;
    }
  };

  class TouchHeal_Sys : public BaseVisitor {
  public:
    TouchHeal_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

    virtual ReturnedCommands operator()(const cmd::Cmd_Touch& cmd) override {
      //std::cout << "TouchHeal_Sys\n";
      ReturnedCommands ret;
      auto touchHeal = m_entManager->getComponent<comp::TouchHeal>(cmd.touched);
      if (touchHeal) {
        auto hp = m_entManager->getComponent<comp::HP>(cmd.touching);
        if (hp) {
          //std::cout << " exec\n";
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

    virtual ReturnedCommands operator()(const cmd::Cmd_Touch& cmd) override {
      //std::cout << "TouchDamage_Sys\n";
      ReturnedCommands ret;
      auto touchDamage = m_entManager->getComponent<comp::TouchDamage>(cmd.touched);
      if (touchDamage) {
        //std::cout << " exec\n";
        auto hp = m_entManager->getComponent<comp::HP>(cmd.touching);
        if (hp) {
          cmd::Cmd_Damage cmdDmg;
          cmdDmg.dest = cmd.touching;
          cmdDmg.damage = touchDamage->damage;
          ret.push_back(cmdDmg);
        }
      }
      return ret;
    }
  };

  class TouchDestroy_Sys : public BaseVisitor {
  public:
    TouchDestroy_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

    virtual ReturnedCommands operator()(const cmd::Cmd_Touch& cmd) override {
      //std::cout << "TouchDestroy_Sys\n";
      if (m_entManager->getComponent<comp::TouchDestroy>(cmd.touched)) {
        m_entManager->addComponent<comp::FlaggedDestroyed>(cmd.touched);
        //std::cout << " exec\n";
      }
      return ReturnedCommands();
    }
  };

  class Damage_Sys : public BaseVisitor {
  public:
    Damage_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

    virtual ReturnedCommands operator()(const cmd::Cmd_Damage& cmd) override {
      //std::cout << "Damage_Sys\n";
      auto hp = m_entManager->getComponent<comp::HP>(cmd.dest);
      if (hp) { hp->hp -= cmd.damage; /*std::cout << " exec\n";*/ }
      return ReturnedCommands();
    }
  };

  class Points_Sys : public BaseVisitor {
  public:
    Points_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

    virtual ReturnedCommands operator()(const cmd::Cmd_Points& cmd) override {
      //std::cout << "Points_Sys\n";
      auto points = m_entManager->getComponent<comp::Points>(cmd.dest);
      if (points) { points->points += cmd.points; /*std::cout << " exec\n";*/ }
      return ReturnedCommands();
    }
  };

  class Move_Sys : public BaseVisitor {
  public:
    Move_Sys(
      const std::shared_ptr<EntityManager>& entManager,
      const std::shared_ptr<TerrainMap>& terrainMap,
      const std::shared_ptr<TerrainObjectSystem>& terrainObjSys
    ) :
      BaseVisitor(entManager),
      m_terrainMap(terrainMap),
      m_terrainObjSys(terrainObjSys)
    {}

    virtual ReturnedCommands operator()(const cmd::Cmd_MoveUp& cmd) override {
      ReturnedCommands ret;
      auto newPos = executeMove(ret, -1, 0, cmd.dest);
      executeSpriteMove<cmd::Cmd_MoveSpriteUp>(ret, cmd.dest, newPos);
      return ret;
    }

    virtual ReturnedCommands operator()(const cmd::Cmd_MoveDown& cmd) override {
      ReturnedCommands ret;
      auto newPos = executeMove(ret, 1, 0, cmd.dest);
      executeSpriteMove<cmd::Cmd_MoveSpriteDown>(ret, cmd.dest, newPos);
      return ret;
    }

    virtual ReturnedCommands operator()(const cmd::Cmd_MoveLeft& cmd) override {
      ReturnedCommands ret;
      auto newPos = executeMove(ret, 0, -1, cmd.dest);
      executeSpriteMove<cmd::Cmd_MoveSpriteLeft>(ret, cmd.dest, newPos);
      return ret;
    }

    virtual ReturnedCommands operator()(const cmd::Cmd_MoveRight& cmd) override {
      ReturnedCommands ret;
      auto newPos = executeMove(ret, 0, 1, cmd.dest);
      executeSpriteMove<cmd::Cmd_MoveSpriteRight>(ret, cmd.dest, newPos);
      return ret;
    }

    virtual ReturnedCommands operator()(const cmd::Cmd_MoveNone& cmd) override {
      ReturnedCommands ret;
      executeMove(ret, 0, 0, cmd.dest);
      return ret;
    }

  private:
    template<typename T>
    void executeSpriteMove(ReturnedCommands& ret, EntityID::UUID entID, std::optional<comp::Position*> newPos) {
      if (newPos.has_value()) {
        if (m_entManager->hasComponent<comp::AnimatedSprites>(entID) || m_entManager->hasComponent<comp::Sprites>(entID)) {
          T moveCmd;
          moveCmd.dest = entID;
          moveCmd.row = newPos.value()->row;
          moveCmd.col = newPos.value()->col;
          ret.push_back(moveCmd);
        }
      }
    }

    std::optional<comp::Position*> executeMove(ReturnedCommands& ret, int rowDiff, int colDiff, EntityID::UUID entID) {
      //std::cout << "Move_Sys\n";
      auto pos = m_entManager->getComponent<comp::Position>(entID);
      if (pos) {
        if (m_terrainMap->at(pos->row + rowDiff, pos->col + colDiff) == TerrainType::PASSABLE) {
          ///// TODO: Remove!
          //cmd::Cmd_ChangeTerrainType changeCmd;
          //changeCmd.row = pos->row;
          //changeCmd.col = pos->col+1;
          //changeCmd.newType = TerrainType::STONE;
          //ret.push_back(changeCmd);
          /////

          //std::cout << " exec\n";
          auto objects = m_terrainObjSys->at(pos->row + rowDiff, pos->col + colDiff);
          for (auto& ent : objects) {
            cmd::Cmd_Touch touchCmd;
            touchCmd.touched = ent.first;
            touchCmd.touching = entID;
            ret.push_back(touchCmd);
          }
          pos->row += rowDiff;
          pos->col += colDiff;
          return pos;
        }
        else {
          //std::cout << " exec\n";
          cmd::Cmd_MoveNone standCmd;
          standCmd.dest = entID;
          ret.push_back(standCmd);
        }
      }
      return {};
    }

    std::shared_ptr<TerrainMap> m_terrainMap;
    std::shared_ptr<TerrainObjectSystem> m_terrainObjSys;
  };

  class MoveSprite_Sys : public BaseVisitor {
  public:
    MoveSprite_Sys(
      const std::shared_ptr<EntityManager>& entManager,
      const std::shared_ptr<render::SpriteRenderer>& spriteRenderer
    ) :
      BaseVisitor(entManager),
      m_spriteRenderer(spriteRenderer)
    {}

    virtual ReturnedCommands operator()(const cmd::Cmd_MoveSpriteLeft& cmd) override {
      m_spriteRenderer->spriteMove(cmd.dest, "moveLeft", cmd.row, cmd.col);
      return ReturnedCommands();
    }

    virtual ReturnedCommands operator()(const cmd::Cmd_MoveSpriteRight& cmd) override {
      m_spriteRenderer->spriteMove(cmd.dest, "moveRight", cmd.row, cmd.col);
      return ReturnedCommands();
    }

    virtual ReturnedCommands operator()(const cmd::Cmd_MoveSpriteUp& cmd) override {
      m_spriteRenderer->spriteMove(cmd.dest, "moveUp", cmd.row, cmd.col);
      return ReturnedCommands();
    }

    virtual ReturnedCommands operator()(const cmd::Cmd_MoveSpriteDown& cmd) override {
      m_spriteRenderer->spriteMove(cmd.dest, "moveDown", cmd.row, cmd.col);
      return ReturnedCommands();
    }

  private:
    std::shared_ptr<render::SpriteRenderer> m_spriteRenderer;
  };

  class ChangeTerrainType_Sys : public BaseVisitor{
  public:
    ChangeTerrainType_Sys(
      const std::shared_ptr<EntityManager>& entManager,
      const std::shared_ptr<TerrainMap>& terrainMap,
      const std::shared_ptr<TerrainObjectSystem>& terrainObjSys
    ) :
      BaseVisitor(entManager),
      m_terrainMap(terrainMap),
      m_terrainObjSys(terrainObjSys)
    {}

    virtual ReturnedCommands operator()(const cmd::Cmd_ChangeTerrainType& cmd) override { 
      ReturnedCommands ret;
      //TODO: Implement further checking if change is valid!
      m_terrainMap->set(cmd.row, cmd.col, cmd.newType);
      cmd::Cmd_ChangeTileType newCmd;
      newCmd.row = cmd.row;
      newCmd.col = cmd.col;
      newCmd.newType = cmd.newType;
      ret.push_back(newCmd);

      return ret;
    }

  private:
    std::shared_ptr<TerrainMap> m_terrainMap;
    std::shared_ptr<TerrainObjectSystem> m_terrainObjSys;
  };

  class ChangeTileType_Rendering_Sys : public BaseVisitor {
  public:
    ChangeTileType_Rendering_Sys(
      const std::shared_ptr<EntityManager>& entManager,
      const std::shared_ptr<render::TileRenderer>& tileRenderer
    ) :
      BaseVisitor(entManager),
      m_tileRenderer(tileRenderer)
    {}

    virtual ReturnedCommands operator()(const cmd::Cmd_ChangeTileType& cmd) override { 
      m_tileRenderer->setTileType(cmd.row, cmd.col, cmd.newType);
      return ReturnedCommands(); 
    }

  private:
    std::shared_ptr<render::TileRenderer> m_tileRenderer;
  };

}

#endif // !_COMPONENTSYSTEM_HPP_

