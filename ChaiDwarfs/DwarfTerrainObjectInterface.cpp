#include "DwarfTerrainObjectInterface.hpp"
#include "TerrainObjectSystem.hpp"

using namespace CDwarfs;

DwarfTerrainObjectInterface::DwarfTerrainObjectInterface(const std::shared_ptr<TerrainObjectSystem>& terrainObjSys) : 
  m_terrainObjSys(terrainObjSys){}

const std::string& DwarfTerrainObjectInterface::checkForObject(int currRow, int currCol, int viewDist, int diffRow, int diffCol) const {
  if (m_terrainObjSys.expired()) return "Unknown";
  auto sys = m_terrainObjSys.lock();

  if (std::abs(diffRow) > viewDist || std::abs(diffCol) > viewDist ||
      currRow + diffRow < 0 || currCol + diffCol < 0)
  {
    return "OutOfReach";
  }

  auto objects = sys->at(currRow + diffRow, currRow + diffCol);
  if (objects.empty()) return "Nothing";
  return objects[0].second;
}