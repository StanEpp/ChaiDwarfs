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

#include <iostream>
#include <vector>
#include "src/Commands.hpp"
#include "src/ecs/EntityManager.hpp"
#include "src/ecs/Components.hpp"
#include "src/ecs/ComponentsAI.hpp"
#include "src/TerrainMap.hpp"
#include "src/cmpSystems/TerrainObjectSystem.hpp"
#include "src/rendering/TileRenderer.hpp"
#include "src/rendering/SpriteRenderer.hpp"
#include "src/interfaces/TerrainObjectInterface.hpp"
#include "src/interfaces/TerrainInterface.hpp"

namespace cdwarfs::compSys
{

class BaseVisitor
{
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
    virtual ReturnedCommands operator()(const cmd::Cmd_ExecuteAI&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::Cmd_ExecuteEveryAI&) { return ReturnedCommands(); }

protected:
    std::shared_ptr<EntityManager> m_entManager;
};

class TouchValue_Sys : public BaseVisitor
{
public:
    TouchValue_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

     ReturnedCommands operator()(const cmd::Cmd_Touch& cmd) override
     {
        ReturnedCommands ret;
        auto touchValue = m_entManager->getComponent<comp::TouchValue>(cmd.touched);
        if (touchValue) {
            auto points = m_entManager->getComponent<comp::Points>(cmd.touching);
            if (points) {
               cmd::Cmd_Points cmdPoints;
               cmdPoints.dest = cmd.touching;
               cmdPoints.points = touchValue->value;
               ret.push_back(cmdPoints);
            }
        }
        return ret;
    }
};

class TouchHeal_Sys : public BaseVisitor
{
public:
  TouchHeal_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

   ReturnedCommands operator()(const cmd::Cmd_Touch& cmd) override
   {
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

class TouchDamage_Sys : public BaseVisitor
{
public:
  TouchDamage_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

   ReturnedCommands operator()(const cmd::Cmd_Touch& cmd) override
   {
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
     return ret;
  }
};

class TouchDestroy_Sys : public BaseVisitor
{
public:
    TouchDestroy_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

     ReturnedCommands operator()(const cmd::Cmd_Touch& cmd) override
     {
        if (m_entManager->getComponent<comp::TouchDestroy>(cmd.touched)) {
            m_entManager->addComponent<comp::FlaggedDestroyed>(cmd.touched);
        }
        return ReturnedCommands();
    }
};

class Damage_Sys : public BaseVisitor
{
public:
    Damage_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

     ReturnedCommands operator()(const cmd::Cmd_Damage& cmd) override
     {
         auto hp = m_entManager->getComponent<comp::HP>(cmd.dest);
         if (hp) {
             hp->hp -= cmd.damage;
         }
         return ReturnedCommands();
    }
};

class Points_Sys : public BaseVisitor
{
public:
    Points_Sys(const std::shared_ptr<EntityManager>& entManager) : BaseVisitor(entManager) {}

     ReturnedCommands operator()(const cmd::Cmd_Points& cmd) override
     {
        auto points = m_entManager->getComponent<comp::Points>(cmd.dest);
        if (points) {
            points->points += cmd.points;
        }
        return ReturnedCommands();
    }
};

class Move_Sys : public BaseVisitor
{
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

     ReturnedCommands operator()(const cmd::Cmd_MoveUp& cmd) override
     {
        ReturnedCommands ret;
        auto newPos = executeMove(ret, -1, 0, cmd.dest);
        executeSpriteMove<cmd::Cmd_MoveSpriteUp>(ret, cmd.dest, newPos);
        return ret;
    }

     ReturnedCommands operator()(const cmd::Cmd_MoveDown& cmd) override
     {
        ReturnedCommands ret;
        auto newPos = executeMove(ret, 1, 0, cmd.dest);
        executeSpriteMove<cmd::Cmd_MoveSpriteDown>(ret, cmd.dest, newPos);
        return ret;
    }

     ReturnedCommands operator()(const cmd::Cmd_MoveLeft& cmd) override
     {
        ReturnedCommands ret;
        auto newPos = executeMove(ret, 0, -1, cmd.dest);
        executeSpriteMove<cmd::Cmd_MoveSpriteLeft>(ret, cmd.dest, newPos);
        return ret;
    }

     ReturnedCommands operator()(const cmd::Cmd_MoveRight& cmd) override
     {
        ReturnedCommands ret;
        auto newPos = executeMove(ret, 0, 1, cmd.dest);
        executeSpriteMove<cmd::Cmd_MoveSpriteRight>(ret, cmd.dest, newPos);
        return ret;
    }

     ReturnedCommands operator()(const cmd::Cmd_MoveNone& cmd) override
     {
        ReturnedCommands ret;
        executeMove(ret, 0, 0, cmd.dest);
        return ret;
    }

private:
    template<typename T>
    void executeSpriteMove(ReturnedCommands& ret, EntityID::UUID entID, std::optional<comp::Position*> newPos)
    {
        if (newPos.has_value()) {
            if (m_entManager->hasComponent<comp::AnimatedSprites>(entID) ||
                m_entManager->hasComponent<comp::Sprites>(entID))
            {
                T moveCmd;
                moveCmd.dest = entID;
                moveCmd.row = newPos.value()->row;
                moveCmd.col = newPos.value()->col;
                ret.push_back(moveCmd);
            }
        }
    }

    std::optional<comp::Position*> executeMove(ReturnedCommands& ret, int rowDiff, int colDiff, EntityID::UUID entID)
    {
        auto pos = m_entManager->getComponent<comp::Position>(entID);
        if (pos) {
            if (m_terrainMap->at(pos->row + rowDiff, pos->col + colDiff) == TerrainType::PASSABLE) {
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
            } else {
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

class MoveSprite_Sys : public BaseVisitor
{
public:
    MoveSprite_Sys(
      const std::shared_ptr<EntityManager>& entManager,
      const std::shared_ptr<render::SpriteRenderer>& spriteRenderer
    ) :
      BaseVisitor(entManager),
      m_spriteRenderer(spriteRenderer)
    {}

     ReturnedCommands operator()(const cmd::Cmd_MoveSpriteLeft& cmd) override
     {
        m_spriteRenderer->spriteMove(cmd.dest, "moveLeft", cmd.row, cmd.col);
        return ReturnedCommands();
    }

     ReturnedCommands operator()(const cmd::Cmd_MoveSpriteRight& cmd) override
     {
        m_spriteRenderer->spriteMove(cmd.dest, "moveRight", cmd.row, cmd.col);
        return ReturnedCommands();
    }

     ReturnedCommands operator()(const cmd::Cmd_MoveSpriteUp& cmd) override
     {
        m_spriteRenderer->spriteMove(cmd.dest, "moveUp", cmd.row, cmd.col);
        return ReturnedCommands();
    }

     ReturnedCommands operator()(const cmd::Cmd_MoveSpriteDown& cmd) override
     {
        m_spriteRenderer->spriteMove(cmd.dest, "moveDown", cmd.row, cmd.col);
        return ReturnedCommands();
    }

private:
    std::shared_ptr<render::SpriteRenderer> m_spriteRenderer;
};

class ChangeTerrainType_Sys : public BaseVisitor
{
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

    ReturnedCommands operator()(const cmd::Cmd_ChangeTerrainType& cmd) override
    {
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

    ReturnedCommands operator()(const cmd::Cmd_ChangeTileType& cmd) override
    {
        m_tileRenderer->setTileType(cmd.row, cmd.col, cmd.newType);
        return ReturnedCommands();
    }

private:
    std::shared_ptr<render::TileRenderer> m_tileRenderer;
};

class DwarfAI_Sys : public BaseVisitor
{
private:
    enum class MoveAction {
        LEFT, RIGHT, UP, DOWN, NONE
    };

public:
    DwarfAI_Sys(
        const std::shared_ptr<EntityManager>& entManager,
        const std::shared_ptr<TerrainMap>& terrainMap,
        const std::shared_ptr<TerrainObjectSystem>& terrainObjSys
    ) :
      BaseVisitor(entManager),
      m_terrainMap(terrainMap),
      m_terrainObjSys(terrainObjSys)
    {}

    ReturnedCommands operator()(const cmd::Cmd_ExecuteAI &cmd) override
    {
        auto ai = m_entManager->getComponent<comp::DwarfAI>(cmd.dest);
        if(ai) {
            if(!ai->initialized) initScript(cmd.dest, ai);
            return execute(cmd.dest, ai);
        }
    }

    ReturnedCommands operator()(const cmd::Cmd_ExecuteEveryAI &) override
    {
        ReturnedCommands ret;
        auto entities = m_entManager->getAllEntitiesWithComponent<comp::DwarfAI>();
        for (const auto ent : entities) {
            auto ai = m_entManager->getComponent<comp::DwarfAI>(ent);
            if(!ai->initialized) initScript(ent, ai);
            auto cmds = execute(ent, ai);
            ret.insert(ret.end(), cmds.begin(), cmds.end());
        }
        return ret;
    }

private:
    void initScript(EntityID::UUID id, comp::DwarfAI *ai)
    {
        ai->terrain = std::make_unique<cdwarfs::TerrainInterface>(m_terrainMap);
        ai->terrainObj = std::make_unique<cdwarfs::TerrainObjectInterface>(m_terrainObjSys);

        ai->chai.add(chaiscript::fun([this, id, ai](int x, int y) {
            auto pos = m_entManager->getComponent<comp::Position>(id);
            auto view = m_entManager->getComponent<comp::View>(id);
            if (pos && view) {
                return ai->terrain->checkTerrain(pos->row, pos->col, view->dist, y, x);
            } else {
                return TerrainType::DARK;
            }
        }),
        "checkTerrain");

        ai->chai.add(chaiscript::fun([this, id, ai](int x, int y) {
            auto pos = m_entManager->getComponent<comp::Position>(id);
            auto view = m_entManager->getComponent<comp::View>(id);
            if (pos && view) {
                return ai->terrainObj->checkForObject(pos->row, pos->col, view->dist, y, x);
            } else {
                return std::string("Unknown");
            }
        }),
        "checkForObject");

        ai->chai.add(chaiscript::fun([this, id]() {
            auto hp = m_entManager->getComponent<comp::HP>(id);
            if (hp)
                return hp->hp;
            return -1;
        }), "getHP");

        ai->chai.add(chaiscript::fun([this, id]() {
            auto speed = m_entManager->getComponent<comp::Speed>(id);
            if (speed)
                return speed->speed;
            return -1;
        }), "getSpeed");

        ai->chai.add(chaiscript::fun([this, id]() {
            auto pos = m_entManager->getComponent<comp::Position>(id);
            if (pos)
                return pos->col;
            return -1;
        }), "getX");

        ai->chai.add(chaiscript::fun([this, id]() {
            auto pos = m_entManager->getComponent<comp::Position>(id);
            if (pos)
                return pos->row;
            return -1;
        }), "getY");

        ai->chai.add(chaiscript::fun([this, id]() {
            auto view = m_entManager->getComponent<comp::View>(id);
            if (view)
                return view->dist;
            return -1;
        }), "getViewDistance");

        ai->chai.add(chaiscript::fun([this, id]() {
            auto points = m_entManager->getComponent<comp::Points>(id);
            if (points)
                return points->points;
            return -1;
        }), "getPoints");

        ai->chai.add(chaiscript::fun([this]() { m_moveAction = MoveAction::DOWN; }), "moveDown");
        ai->chai.add(chaiscript::fun([this]() { m_moveAction = MoveAction::UP; }), "moveUp");
        ai->chai.add(chaiscript::fun([this]() { m_moveAction = MoveAction::LEFT; }), "moveLeft");
        ai->chai.add(chaiscript::fun([this]() { m_moveAction = MoveAction::RIGHT; }), "moveRight");
        ai->chai.add(chaiscript::fun([this]() { m_moveAction = MoveAction::NONE; }), "moveStop");

        try{
            ai->chai.eval_file(ai->filePath);
            ai->entryPoint = ai->chai.eval<std::function<void()>>("main");
        } catch (const chaiscript::exception::eval_error &ee) {
            std::cout << ee.what() << '\n';
            if (ee.call_stack.size() > 0) {
                std::cout << "during evaluation at (" << ee.call_stack[0].start().line << ", " << ee.call_stack[0].start().column << ")\n";
            }
        }

        ai->initialized = true;
    }

    ReturnedCommands execute(EntityID::UUID id, comp::DwarfAI *ai)
    {
        ReturnedCommands ret;

        try {
            std::cout << "Executing script " << ai->filePath << " of entity ID " << id <<" ...\n";
            ai->entryPoint();
            std::cout << "Successfully executed script!" << "\n\n";
        } catch (const std::exception &e) {
            std::cout << "Error while executing script " << ai->filePath << ":\n";
            std::cout << e.what() << "\n\n";
            return ret;
        }

        ret.reserve(1);
        switch (m_moveAction) {
        case MoveAction::LEFT: {
            cmd::Cmd_MoveLeft cmdLeft;
            cmdLeft.dest = id;
            ret.push_back(cmdLeft);
        } break;
        case MoveAction::RIGHT: {
            cmd::Cmd_MoveRight cmdRight;
            cmdRight.dest = id;
            ret.push_back(cmdRight);
        } break;
        case MoveAction::UP: {
            cmd::Cmd_MoveUp cmdUp;
            cmdUp.dest = id;
            ret.push_back(cmdUp);
        } break;
        case MoveAction::DOWN: {
            cmd::Cmd_MoveDown cmdDown;
            cmdDown.dest = id;
            ret.push_back(cmdDown);
        } break;
        case MoveAction::NONE: {
            cmd::Cmd_MoveNone cmdNone;
            cmdNone.dest = id;
            ret.push_back(cmdNone);
        } break;
        }

        m_moveAction = MoveAction::NONE;

        return ret;
    }

    std::shared_ptr<TerrainMap> m_terrainMap;
    std::shared_ptr<TerrainObjectSystem> m_terrainObjSys;
    MoveAction m_moveAction{ MoveAction::NONE };
};

}
