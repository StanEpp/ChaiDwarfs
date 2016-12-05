#ifndef _GAMESTATECHANGER_HPP_
#define _GAMESTATECHANGER_HPP_

#include <memory>

#include "TerrainMap.hpp"
#include "DwarfSystem.hpp"
#include <iostream>
namespace CDwarfs {

  class GameStateControl {
  public:
    GameStateControl() = delete;
    GameStateControl(const std::shared_ptr<TerrainMap> &terrain, const std::shared_ptr<DwarfSystem> &dwarfSys)
      : m_terrain(terrain), m_dwarfSys(dwarfSys) {}


    void executeCommands(const std::vector<DwarfCommand> &&commands) {
      for (auto& cmd : commands) {
        auto& dwarf = m_dwarfSys->getDwarf(cmd.dwarfID);
        auto row = dwarf.getPosition().x();
        auto col = dwarf.getPosition().y();

        if (cmd.movement == DwarfCommand::MoveDirection::RIGHT) {
          if (m_terrain->at(row + 1 , col) == TerrainType::PASSABLE) {
            dwarf.moveRight();
          }
        }
        else if (cmd.movement == DwarfCommand::MoveDirection::UP) {
          if (m_terrain->at(row, col - 1) == TerrainType::PASSABLE) {
            dwarf.moveUp();
          }
        }
        else if (cmd.movement == DwarfCommand::MoveDirection::DOWN) {
          if (m_terrain->at(row, col + 1) == TerrainType::PASSABLE) {
            dwarf.moveDown();
          }
        }
        else if (cmd.movement == DwarfCommand::MoveDirection::LEFT) {
          if (m_terrain->at(row - 1, col) == TerrainType::PASSABLE) {
            dwarf.moveLeft();
          }
        }
      }
    }

  private:

    std::shared_ptr<TerrainMap>  m_terrain;
    std::shared_ptr<DwarfSystem> m_dwarfSys;
  };

};

#endif // !_GAMESTATECHANGER_HPP_
