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
#include "ComponentSystem.hpp"
#include "src/ecs/ComponentsAI.hpp"
#include "src/ecs/Components.hpp"
#include "src/interfaces/TerrainObjectInterface.hpp"
#include "src/interfaces/TerrainInterface.hpp"
#include <chaiscript/chaiscript.hpp>

using namespace cdwarfs::compSys;

DwarfAI_Sys::DwarfAI_Sys(
    const std::shared_ptr<EntityManager>& entManager,
    const std::shared_ptr<TerrainMap>& terrainMap,
    const std::shared_ptr<TerrainObjectSystem>& terrainObjSys
) :
    BaseVisitor(entManager),
    m_terrainMap(terrainMap),
    m_terrainObjSys(terrainObjSys)
{}

BaseVisitor::ReturnedCommands DwarfAI_Sys::operator()(const cmd::ExecuteAI &cmd)
{
    auto ai = m_entManager->getComponent<comp::DwarfAI>(cmd.dest);
    if(ai) {
        if(!ai->initialized) initScript(cmd.dest, ai);
        return execute(cmd.dest, ai);
    }
}

BaseVisitor::ReturnedCommands DwarfAI_Sys::operator()(const cmd::ExecuteEveryAI &)
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

void DwarfAI_Sys::initScript(EntityID::UUID id, comp::DwarfAI *ai)
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

BaseVisitor::ReturnedCommands DwarfAI_Sys::execute(EntityID::UUID id, comp::DwarfAI *ai)
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
        cmd::MoveLeft cmdLeft;
        cmdLeft.dest = id;
        ret.push_back(cmdLeft);
    } break;
    case MoveAction::RIGHT: {
        cmd::MoveRight cmdRight;
        cmdRight.dest = id;
        ret.push_back(cmdRight);
    } break;
    case MoveAction::UP: {
        cmd::MoveUp cmdUp;
        cmdUp.dest = id;
        ret.push_back(cmdUp);
    } break;
    case MoveAction::DOWN: {
        cmd::MoveDown cmdDown;
        cmdDown.dest = id;
        ret.push_back(cmdDown);
    } break;
    case MoveAction::NONE: {
        cmd::MoveNone cmdNone;
        cmdNone.dest = id;
        ret.push_back(cmdNone);
    } break;
    }

    m_moveAction = MoveAction::NONE;

    return ret;
}

