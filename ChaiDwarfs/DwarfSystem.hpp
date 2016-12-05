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
#ifndef _DWARFSYSTEM_HPP_
#define _DWARFSYSTEM_HPP_

#include <vector>

#include "ChaiScriptTypes.hpp"
#include "Dwarf.hpp"
#include "DwarfAI.hpp"

namespace CDwarfs {

class DwarfSystem {

  struct DwarfWithAI{
    Dwarf dwarf;
    std::unique_ptr<DwarfAI> ai;
    DwarfWithAI(unsigned int ID) : dwarf(ID), ai(nullptr) {}
  };

public:
  DwarfSystem() = default;
  DwarfSystem(DwarfSystem&) = delete; DwarfSystem& operator=(DwarfSystem&) = delete;
  DwarfSystem(DwarfSystem&&) = delete; DwarfSystem& operator=(DwarfSystem&&) = delete;
  ~DwarfSystem() = default;

  Dwarf& getDwarf(DwarfID ID) {
    return m_Dwarfs[ID].dwarf;
  }

  template<class AI, class... AIArgs>
  Dwarf& add(AIArgs&&... aiParams) {
    auto ID = static_cast<unsigned int>(m_Dwarfs.size());
    m_Dwarfs.emplace_back(ID);
    auto& dwarfPack = m_Dwarfs[ID];
    dwarfPack.ai = std::make_unique<AI>(std::forward<AIArgs>(aiParams)...);
    dwarfPack.ai->init(dwarfPack.dwarf);

    return getDwarf(ID);
  }

  void erase(DwarfID ID) {
    if (ID >= m_Dwarfs.size()) return;
    m_Dwarfs.erase(m_Dwarfs.cbegin() + ID);
  }

  std::vector<DwarfCommand> generateCommands() {
    std::vector<DwarfCommand> commands;
    commands.resize(m_Dwarfs.size());
    for (auto& dwarfPack : m_Dwarfs) {
      auto ID = dwarfPack.dwarf.getID();
      auto& cmd = commands[ID];
      cmd = dwarfPack.ai->generateCommand(dwarfPack.dwarf);
      cmd.dwarfID = dwarfPack.dwarf.getID();
    }
    return commands;
  }

private:
  std::vector<DwarfWithAI>  m_Dwarfs;

};

}

#endif
