#include <iostream>
#include "DwarfTerrainInterface.hpp"
#include "Datastructs.hpp"
#include "Dwarf.hpp"
#include "TerrainMap.hpp"

using namespace CDwarfs;

DwarfTerrainInterface::DwarfTerrainInterface(const std::shared_ptr<TerrainMap>& terrainMap) :
  m_terrain(terrainMap) {}

const TerrainType DwarfTerrainInterface::checkTerrain(const Dwarf& dwarf, int x, int y) const{
  if (m_terrain.expired()) return TerrainType::NO_MAP;

  auto map = m_terrain.lock();
  auto viewDist = dwarf.getViewDistance();
  auto posX = static_cast<int>(dwarf.getPosition().x());
  auto posY = static_cast<int>(dwarf.getPosition().y());

  if (std::abs(x) - viewDist > 0 || std::abs(y) - viewDist > 0 ||
      posX + x < 0 || posX + x >= static_cast<int>(map->columns()) ||
      posY + y < 0 || posY + y >= static_cast<int>(map->rows()) )
  {
    return TerrainType::DARK;
  }

  return map->at(posY + y, posX + x);
  
}