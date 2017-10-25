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

#include "TerrainMap.hpp"
#include "TerrainObjectSystem.hpp"
#include "DwarfTerrainInterface.hpp"
#include "DwarfTerrainObjectInterface.hpp"
#include "Entity.hpp"

namespace cdwarfs
{

class DwarfRoundActions;
class EntityManager;

class DwarfAI {
public:
  DwarfAI() = delete;
  DwarfAI(const std::shared_ptr<TerrainMap> &terrainMap, const std::shared_ptr<TerrainObjectSystem> &terrainObjSys, const std::shared_ptr<EntityManager>& entManager) :
    m_terrain(std::make_unique<DwarfTerrainInterface>(terrainMap)),
    m_terrainObj(std::make_unique<DwarfTerrainObjectInterface>(terrainObjSys)),
    m_entManager(entManager)
  {}

  virtual void init() = 0;
  virtual DwarfRoundActions generateCommand() = 0;

  void setTerrainInterface(const std::shared_ptr<TerrainMap>& terrainMap)
  {
    m_terrain = std::make_unique<DwarfTerrainInterface>(terrainMap);
  }

  void setTerrainObjectInterface(const std::shared_ptr<TerrainObjectSystem>& terrainObjSys)
  {
    m_terrainObj = std::make_unique<DwarfTerrainObjectInterface>(terrainObjSys);
  }

  void setEntityManager(const std::shared_ptr<EntityManager>& entManager)
  {
    m_entManager = entManager;
  }

  void setDwarfID(EntityID::UUID eID)
  {
    m_dwarfID = eID;
  }

protected:
  std::unique_ptr<DwarfTerrainInterface>  m_terrain;
  std::unique_ptr<DwarfTerrainObjectInterface>  m_terrainObj;
  std::shared_ptr<EntityManager> m_entManager;
  EntityID::UUID m_dwarfID{ 0 };
};

}
