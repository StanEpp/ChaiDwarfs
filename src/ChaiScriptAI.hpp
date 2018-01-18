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

#include <string>
#include <fstream>
#include <chrono>
#include <experimental/filesystem>
#include <iostream>

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/utility/utility.hpp>

#include "DwarfRoundActions.hpp"
#include "TerrainMap.hpp"
#include "interfaces/DwarfAI.hpp"
#include "ecs/Components.hpp"
#include "ecs/EntityManager.hpp"


namespace cdwarfs
{

class ChaiscriptAI final : public DwarfAI
{
public:
  ChaiscriptAI() = delete;
  ChaiscriptAI(const std::string& scriptPath, const std::shared_ptr<TerrainMap> &terrainMap, const std::shared_ptr<TerrainObjectSystem> &terrainObjSys) :
    DwarfAI(terrainMap, terrainObjSys, nullptr),
    m_scriptPath(scriptPath) {}

  ChaiscriptAI(const std::string& scriptPath) :
    DwarfAI(nullptr, nullptr, nullptr),
    m_scriptPath(scriptPath) {}

  void init()
  {
    if (!m_terrain || !m_terrainObj) throw std::runtime_error("Cannot initialize ChaiscriptAI when pointers are not set!");

    m_chai.add(chaiscript::fun(&DwarfRoundActions::moveDown, std::ref(m_command)), "moveDown");
    m_chai.add(chaiscript::fun(&DwarfRoundActions::moveUp, std::ref(m_command)), "moveUp");
    m_chai.add(chaiscript::fun(&DwarfRoundActions::moveLeft, std::ref(m_command)), "moveLeft");
    m_chai.add(chaiscript::fun(&DwarfRoundActions::moveRight, std::ref(m_command)), "moveRight");
    m_chai.add(chaiscript::fun(&DwarfRoundActions::moveStop, std::ref(m_command)), "moveStop");

    auto pos = m_entManager->getComponent<comp::Position>(m_dwarfID);
    auto view = m_entManager->getComponent<comp::View>(m_dwarfID);
    auto speed = m_entManager->getComponent<comp::Speed>(m_dwarfID);
    auto hp = m_entManager->getComponent<comp::HP>(m_dwarfID);
    auto points = m_entManager->getComponent<comp::Points>(m_dwarfID);

    if (pos && view && speed && hp && points) {
      auto terrainPtr = m_terrain.get();
      auto terrainObjPtr = m_terrainObj.get();

      m_chai.add(chaiscript::fun([terrainPtr, pos, view](int x, int y) {
          return terrainPtr->checkTerrain(pos->row, pos->col, view->dist, y, x);
      }),
      "checkTerrain");

      m_chai.add(chaiscript::fun([terrainObjPtr, pos, view](int x, int y) {
          return terrainObjPtr->checkForObject(pos->row, pos->col, view->dist, y, x);
      }),
      "checkForObject");

      m_chai.add(chaiscript::fun([hp]() { return hp->hp; }), "getHP");
      m_chai.add(chaiscript::fun([speed]() { return speed->speed; }), "getSpeed");
      m_chai.add(chaiscript::fun([pos]() { return pos->col; }), "getX");
      m_chai.add(chaiscript::fun([pos]() { return pos->row; }), "getY");
      m_chai.add(chaiscript::fun([view]() { return view->dist; }), "getViewDistance");
      m_chai.add(chaiscript::fun([points]() { return points->points; }), "getPoints");
    }

    chaiscript::ModulePtr m = chaiscript::ModulePtr(new chaiscript::Module());
    chaiscript::utility::add_class<TerrainType>(*m,
      "TerrainType",
      { { TerrainType::SOIL, "SOIL" },
        { TerrainType::DARK, "DARK" },
        { TerrainType::PASSABLE, "PASSABLE"},
        { TerrainType::NO_MAP, "NOMAP" },
        { TerrainType::STONE, "STONE" }
      }
    );

    m_chai.add(m);

    m_lastValidScriptState = m_cleanScriptState = m_chai.get_state();
  }

  DwarfRoundActions generateCommand()
  {
    m_command.clear();
    runScript();
    return m_command;
  }

private:

  void loadFile()
  {
    std::ifstream infile(m_scriptPath.c_str(), std::ios::in | std::ios::ate | std::ios::binary);

    if (!infile.is_open()) {
      throw std::runtime_error("\"" + m_scriptPath + "\" file not found!");
    }

    const auto size = infile.tellg();

    if (size == std::streampos(0)) {
      m_script = "";
    } else {
      infile.seekg(0, std::ios::beg);
      std::vector<char> v(static_cast<size_t>(size));
      infile.read(&v[0], size);
      m_script = std::string(v.begin(), v.end());
    }
  }

  bool getScript()
  {
    std::error_code ec;
    auto lastWrite = std::experimental::filesystem::last_write_time(m_scriptPath, ec);
    bool ret = false;

    if (m_lastFileUpdate != lastWrite) {
      try {
        loadFile();
        ret = true;
      }
      catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
      }
      m_lastFileUpdate = lastWrite;
    }
    return ret;
  }

  bool runScript()
  {
    if (getScript()) {
      try {
        std::cout << "Evaluating new script " << m_scriptPath << "...\n";
        m_chai.set_state(m_cleanScriptState);
        m_chai.eval(m_script);
        m_scriptEntryPoint = m_chai.eval<std::function<void()>>("main");
        m_lastValidScriptState = m_chai.get_state();
        std::cout << "Successfully evaluated script!" << "\n\n";
      }
      catch (const chaiscript::exception::eval_error &ee) {
        std::cout << ee.what() << '\n';
        if (ee.call_stack.size() > 0) {
          std::cout << "during evaluation at (" << ee.call_stack[0].start().line << ", " << ee.call_stack[0].start().column << ")\n";
        }
        std::cout << "Falling back on last valid script!" << "\n\n";
        m_chai.set_state(m_lastValidScriptState);
        return false;
      }
    }

    if (m_scriptEntryPoint) {
      try {
        m_scriptEntryPoint();
      }
      catch (const std::exception &e) {
        std::cout << "Error while executing script " << m_scriptPath << ":\n";
        std::cout << e.what() << "\n\n";
        return false;
      }
    }

    return true;
  }

private:
  std::string   m_script;
  std::string   m_scriptPath;
  std::experimental::filesystem::file_time_type   m_lastFileUpdate;
  std::function<void()>  m_scriptEntryPoint;

  chaiscript::ChaiScript m_chai;
  chaiscript::ChaiScript::State m_lastValidScriptState;
  chaiscript::ChaiScript::State m_cleanScriptState;

  DwarfRoundActions m_command;
};

}
