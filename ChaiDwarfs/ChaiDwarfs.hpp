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

#ifndef _CHAIDWARFS_HPP_
#define _CHAIDWARFS_HPP_

#include "DwarfSystem.hpp"
#include "TerrainMap.hpp"
#include "Timer.hpp"
#include "ChaiScriptAI.hpp"
#include "GameStateControl.hpp"
#include "TerrainObjectSystem.hpp"
#include "ECSFactory.hpp"
#include "ObjectComponents.hpp"
#include "DwarfComponents.hpp"

namespace CDwarfs {

  class ChaiDwarfs {
  private:
    std::shared_ptr<DwarfSystem>   m_dwarfSys;
    std::shared_ptr<TerrainMap>    m_terrain;
    std::shared_ptr<TerrainObjectSystem>    m_terrainObjSys;
    std::shared_ptr<GameStateControl> m_gameState;
    bool  m_running;

    ECSFactory<BaseObjectComponent> objFac;
    ECSFactory<BaseDwarfComponent>  dwarfFac;

  public:
    ChaiDwarfs() : 
      m_dwarfSys(std::make_shared<DwarfSystem>()),
      m_running(true),
      m_terrain(std::make_shared<TerrainMap>()),
      m_terrainObjSys(std::make_shared<TerrainObjectSystem>()),
      m_gameState(std::make_shared<GameStateControl>(m_terrain, m_dwarfSys)){}


    void init() { 
      m_terrain->load("mapGeneration.chai");

      objFac.registerComponent<TouchValue, const int>("TouchValue");
      objFac.registerComponent<TouchDamage, const int>("TouchDamage");
      objFac.registerComponent<Name, const std::string&>("Name");
      objFac.registerComponent<Position, const int, const int>("Position");
      objFac.loadObjectDefinitions("objectDefinitions.chai");

      dwarfFac.registerComponent<DwarfHP, const int, const int>("DwarfHP");
      dwarfFac.registerComponent<DwarfSpeed, const int>("DwarfSpeed");
      dwarfFac.registerComponent<DwarfPosition, const int, const int>("DwarfPosition");
      dwarfFac.registerComponent<DwarfPoints, const int>("DwarfPoints");
      dwarfFac.registerComponent<DwarfView, const int>("DwarfView");
    }

    void run() {
      Timer timer;
      auto lastTimePoint = timer.currentTime();

      m_dwarfSys->add<ChaiScriptAI>("dwarf.chai", m_terrain, m_terrainObjSys);
      auto& obj = m_terrainObjSys->add<Diamond>();
      obj.pos().addX(10);
      //int counter = 0;
      while (m_running) {
        if (timer.haveMilliSecondsPassed(500, lastTimePoint)) {
          m_gameState->executeCommands(m_dwarfSys->generateCommands());
          lastTimePoint = timer.currentTime();
          /*counter++;*/
        }
        /*if (counter == 15) m_running = false;*/
      }
    
    }

  };


}



#endif