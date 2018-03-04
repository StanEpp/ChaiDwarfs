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

#include <vector>
#include <memory>
#include "src/ecs/EntityManager.hpp"
#include "src/Commands.hpp"

namespace cdwarfs
{
class EntityManager;
class TerrainMap;
class TerrainObjectSystem;

namespace render {
class TileRenderer;
class SpriteRenderer;
}

namespace comp {
struct DwarfAI;
}
}

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

    virtual ReturnedCommands operator()(const cmd::Touch&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::Heal&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::Damage&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::Points&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::MoveUp&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::MoveDown&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::MoveLeft&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::MoveRight&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::MoveNone&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::ChangeTerrainType&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::ChangeTileType&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::MoveSpriteLeft&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::MoveSpriteRight&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::MoveSpriteUp&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::MoveSpriteDown&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::ExecuteAI&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::ExecuteEveryAI&) { return ReturnedCommands(); }
    virtual ReturnedCommands operator()(const cmd::KillFlaggedEntities&) { return ReturnedCommands(); }

protected:
    std::shared_ptr<EntityManager> m_entManager;
};

class TouchValue_Sys : public BaseVisitor
{
public:
    TouchValue_Sys(const std::shared_ptr<EntityManager>& entManager);
    ReturnedCommands operator()(const cmd::Touch& cmd) override;
};

class TouchHeal_Sys : public BaseVisitor
{
public:
    TouchHeal_Sys(const std::shared_ptr<EntityManager>& entManager);
    ReturnedCommands operator()(const cmd::Touch& cmd) override;
};

class TouchDamage_Sys : public BaseVisitor
{
public:
    TouchDamage_Sys(const std::shared_ptr<EntityManager>& entManager);
    ReturnedCommands operator()(const cmd::Touch& cmd) override;
};

class TouchDestroy_Sys : public BaseVisitor
{
public:
    TouchDestroy_Sys(const std::shared_ptr<EntityManager>& entManager);
    ReturnedCommands operator()(const cmd::Touch& cmd) override;
};

class Damage_Sys : public BaseVisitor
{
public:
    Damage_Sys(const std::shared_ptr<EntityManager>& entManager);
    ReturnedCommands operator()(const cmd::Damage& cmd);
};

class Points_Sys : public BaseVisitor
{
public:
    Points_Sys(const std::shared_ptr<EntityManager>& entManager);
    ReturnedCommands operator()(const cmd::Points& cmd) override;
};

class Move_Sys : public BaseVisitor
{
public:
    Move_Sys(
        const std::shared_ptr<EntityManager>& entManager,
        const std::shared_ptr<TerrainMap>& terrainMap,
        const std::shared_ptr<TerrainObjectSystem>& terrainObjSys
    );

    ReturnedCommands operator()(const cmd::MoveUp& cmd) override;
    ReturnedCommands operator()(const cmd::MoveDown& cmd) override;
    ReturnedCommands operator()(const cmd::MoveLeft& cmd) override;
    ReturnedCommands operator()(const cmd::MoveRight& cmd) override;
    ReturnedCommands operator()(const cmd::MoveNone& cmd) override;
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

    std::optional<comp::Position*> executeMove(ReturnedCommands& ret, int rowDiff, int colDiff, EntityID::UUID entID);

    std::shared_ptr<TerrainMap> m_terrainMap;
    std::shared_ptr<TerrainObjectSystem> m_terrainObjSys;
};

class MoveSprite_Sys : public BaseVisitor
{
public:
    MoveSprite_Sys(
        const std::shared_ptr<EntityManager>& entManager,
        const std::shared_ptr<render::SpriteRenderer>& spriteRenderer
    );

    ReturnedCommands operator()(const cmd::MoveSpriteLeft& cmd);
    ReturnedCommands operator()(const cmd::MoveSpriteRight& cmd);
    ReturnedCommands operator()(const cmd::MoveSpriteUp& cmd);
    ReturnedCommands operator()(const cmd::MoveSpriteDown& cmd);
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
    );

    ReturnedCommands operator()(const cmd::ChangeTerrainType& cmd);
private:
    std::shared_ptr<TerrainMap> m_terrainMap;
    std::shared_ptr<TerrainObjectSystem> m_terrainObjSys;
};

class ChangeTileType_Rendering_Sys : public BaseVisitor {
public:
    ChangeTileType_Rendering_Sys(
      const std::shared_ptr<EntityManager>& entManager,
      const std::shared_ptr<render::TileRenderer>& tileRenderer
    );

    ReturnedCommands operator()(const cmd::ChangeTileType& cmd);
private:
    std::shared_ptr<render::TileRenderer> m_tileRenderer;
};

class DwarfAI_Sys : public BaseVisitor
{
private:
    enum class MoveAction : unsigned long {
        LEFT, RIGHT, UP, DOWN, NONE
    };

public:
    DwarfAI_Sys(
        const std::shared_ptr<EntityManager>& entManager,
        const std::shared_ptr<TerrainMap>& terrainMap,
        const std::shared_ptr<TerrainObjectSystem>& terrainObjSys
    );

    ReturnedCommands operator()(const cmd::ExecuteAI &cmd) override;
    ReturnedCommands operator()(const cmd::ExecuteEveryAI &) override;
private:
    void initScript(EntityID::UUID id, comp::DwarfAI *ai);
    ReturnedCommands execute(EntityID::UUID id, comp::DwarfAI *ai);

    std::shared_ptr<TerrainMap> m_terrainMap;
    std::shared_ptr<TerrainObjectSystem> m_terrainObjSys;
    MoveAction m_moveAction{ MoveAction::NONE };
};

class KillEntity_Sys : public BaseVisitor
{
public:
    KillEntity_Sys(const std::shared_ptr<EntityManager>& entManager);
    ReturnedCommands operator()(const cmd::KillFlaggedEntities& cmd) override;
};

}
