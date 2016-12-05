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
#ifndef _DWARFAI_HPP_
#define _DWARFAI_HPP_

#include "DwarfTerrainInterface.hpp"
#include "DwarfTerrainObjectInterface.hpp"

namespace CDwarfs {

  class  Dwarf;
  class  TerrainMap;
  struct DwarfCommand;

  class DwarfAI {
  public:
    DwarfAI() = delete;
    DwarfAI(const std::shared_ptr<TerrainMap> &terrainMap, const std::shared_ptr<TerrainObjectSystem> &terrainObjSys) :
      m_terrain(std::make_unique<DwarfTerrainInterface>(terrainMap)),
      m_terrainObj(std::make_unique<DwarfTerrainObjectInterface>(terrainObjSys)) {}

    virtual void init(const Dwarf&) = 0;
    virtual DwarfCommand generateCommand(const Dwarf&) = 0;
  protected:
    std::unique_ptr<DwarfTerrainInterface>  m_terrain;
    std::unique_ptr<DwarfTerrainObjectInterface>  m_terrainObj;
  };

}

#endif // !_DWARFAI_HPP_
