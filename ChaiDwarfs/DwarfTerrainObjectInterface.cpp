#include "DwarfTerrainObjectInterface.hpp"
#include "Dwarf.hpp"
#include "Datastructs.hpp"
#include "TerrainObject.hpp"
#include "TerrainObjectSystem.hpp"

using namespace CDwarfs;

DwarfTerrainObjectInterface::DwarfTerrainObjectInterface(const std::shared_ptr<TerrainObjectSystem>& terrainObjSys) : 
  m_terrainObjSys(terrainObjSys){}

const ObjectType DwarfTerrainObjectInterface::checkForObject(const Dwarf& dwarf, int x, int y) const {
  if (m_terrainObjSys.expired()) return ObjectType::INVALID;
  auto sys = m_terrainObjSys.lock();

  auto viewDist = 3 * dwarf.getViewDistance();
  auto posX = static_cast<int>(dwarf.getPosition().x());
  auto posY = static_cast<int>(dwarf.getPosition().y());

  if (std::abs(x) - viewDist > 0 || std::abs(y) - viewDist > 0 ||
      posX + x < 0 || posY + y < 0)
  {
    return ObjectType::INVALID;
  }

  auto objects = sys->at(posX + x, posY + y);
  if (objects.empty()) return ObjectType::INVALID;
  return objects[0]->type();
}