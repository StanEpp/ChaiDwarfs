#include <iostream>
#include "DwarfTerrainInterface.hpp"
#include "TerrainMap.hpp"

using namespace CDwarfs;

DwarfTerrainInterface::DwarfTerrainInterface(const std::shared_ptr<TerrainMap>& terrainMap) :
  m_terrain(terrainMap) {}

const TerrainType DwarfTerrainInterface::checkTerrain(int currRow, int currCol, int viewDist, int diffRow, int diffCol) const{
  if (m_terrain.expired()) return TerrainType::NO_MAP;

  auto map = m_terrain.lock();

  if (std::abs(diffRow) > viewDist || std::abs(diffCol) > viewDist ||
      currRow + diffRow < 0 || currRow + diffRow >= static_cast<int>(map->columns()) ||
      currCol + diffCol < 0 || currCol + diffCol >= static_cast<int>(map->rows()) )
  {
    return TerrainType::DARK;
  }

  return map->at(currRow + diffRow, currCol + diffCol);
  
}