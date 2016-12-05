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

#ifndef _DWARFSCRIPT_HPP_
#define _DWARFSCRIPT_HPP_

#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <iostream>

#include <chaiscript\chaiscript.hpp>
#include <chaiscript\utility\utility.hpp>

#include "ChaiScriptTypes.hpp"
#include "DwarfCommand.hpp"
#include "DwarfAI.hpp"
#include "TerrainObject.hpp"

namespace CDwarfs {

class ChaiScriptAI final : public DwarfAI {
private:
  std::string   m_script;
  std::string   m_scriptPath;
  std::tr2::sys::file_time_type   m_lastFileUpdate;
  std::function<void()>  m_scriptEntryPoint;
  chaiscript::ChaiScript m_chai;
  chaiscript::ChaiScript::State m_lastValidScriptState;
  chaiscript::ChaiScript::State m_cleanScriptState;
  DwarfCommand m_command;

  void loadFile(){
      std::ifstream infile(m_scriptPath.c_str(), std::ios::in | std::ios::ate | std::ios::binary);

    if (!infile.is_open()) {
      throw std::runtime_error("\"" + m_scriptPath + "\" file not found!");
    }

    const auto size = infile.tellg();

    if (size == std::streampos(0))
    {
      m_script = "";
    }
    else {
      infile.seekg(0, std::ios::beg);
      std::vector<char> v(static_cast<size_t>(size));
      infile.read(&v[0], size);
      m_script = std::string(v.begin(), v.end());
    }
  }

  bool getScript() {
    std::error_code ec;
    auto lastWrite = std::tr2::sys::last_write_time(m_scriptPath, ec);
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

  bool runScript() {
    if (getScript()) {
      try {
        std::cout << "Evaluating new script " << m_scriptPath << "...\n";
        m_chai.set_state(m_cleanScriptState);
        m_chai.eval(m_script);
        m_scriptEntryPoint = m_chai.eval<std::function<void()> >("main");
        m_lastValidScriptState = m_chai.get_state();
        std::cout << "Successfully evaluated script!" << "\n\n";
      }
      catch (const chaiscript::exception::eval_error &ee) {
        std::cout << ee.what() << '\n';
        if (ee.call_stack.size() > 0) {
          std::cout << "during evaluation at (" << ee.call_stack[0]->start().line << ", " << ee.call_stack[0]->start().column << ")\n";
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

public:
  ChaiScriptAI() = delete;
  ChaiScriptAI(const std::string& scriptPath, const std::shared_ptr<TerrainMap> &terrainMap, const std::shared_ptr<TerrainObjectSystem> &terrainObjSys) : 
    DwarfAI(terrainMap, terrainObjSys), m_scriptPath(scriptPath) {}
  ChaiScriptAI(std::string&& scriptPath, const std::shared_ptr<TerrainMap> &terrainMap, const std::shared_ptr<TerrainObjectSystem> &terrainObjSys) :
    DwarfAI(terrainMap, terrainObjSys), m_scriptPath(scriptPath) {}

  void init(const Dwarf& dwarf) {
    m_chai.add(chaiscript::fun(&DwarfCommand::moveDown, std::ref(m_command)), "moveDown");
    m_chai.add(chaiscript::fun(&DwarfCommand::moveUp, std::ref(m_command)), "moveUp");
    m_chai.add(chaiscript::fun(&DwarfCommand::moveLeft, std::ref(m_command)), "moveLeft");
    m_chai.add(chaiscript::fun(&DwarfCommand::moveRight, std::ref(m_command)), "moveRight");
    m_chai.add(chaiscript::fun(&DwarfCommand::moveStop, std::ref(m_command)), "moveStop");
    m_chai.add(chaiscript::fun(&DwarfTerrainInterface::checkTerrain, m_terrain.get(), std::ref(dwarf)), "checkTerrain");
    m_chai.add(chaiscript::fun(&DwarfTerrainObjectInterface::checkForObject, m_terrainObj.get(), std::ref(dwarf)), "checkForObject");
    
    m_chai.add(chaiscript::fun(&Dwarf::getHP, &dwarf), "getHP");
    m_chai.add(chaiscript::fun(&Dwarf::getSpeed, &dwarf), "getSpeed");
    m_chai.add(chaiscript::fun([&dwarf]() { return dwarf.getPosition().x(); }), "getX");
    m_chai.add(chaiscript::fun([&dwarf]() { return dwarf.getPosition().y(); }), "getY");
    m_chai.add(chaiscript::fun(&Dwarf::getViewDistance, &dwarf), "getViewDistance");
    
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
    chaiscript::utility::add_class<ObjectType>(*m,
      "ObjectType",
      { { ObjectType::INVALID, "INVALID" },
      { ObjectType::DIAMOND, "DIAMOND" },
      { ObjectType::RUBY, "RUBY" },
      { ObjectType::SAPPHIRE, "SAPPHIRE" }
      }
    );
    m_chai.add(m);

    m_lastValidScriptState = m_cleanScriptState = m_chai.get_state();
  }

  DwarfCommand generateCommand(const Dwarf&) {
    m_command.clear();
    runScript();
    return m_command;
  }

  
  
};

}

#endif // !_DWARFSCRIPT_HPP_
