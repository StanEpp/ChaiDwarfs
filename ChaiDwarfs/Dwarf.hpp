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
#ifndef _DWARF_HPP_
#define _DWARF_HPP_

#include <memory>

#include "Datastructs.hpp"
#include "DwarfCommand.hpp"
#include "DwarfAI.hpp"

namespace CDwarfs{

  class DwarfSystem;

  class Dwarf {
    friend DwarfSystem;
  public:

    inline void damage(unsigned int dmg) {
      if (m_HP - dmg > m_HP) {
        m_HP = 0;
      }
      else {
        m_HP -= dmg;
      } 
    }

    inline void heal(unsigned int heal) {
      if (m_maxHP - m_HP <= heal) {
        m_HP = m_maxHP;
      }
      else {
        m_HP += heal;
      }
    }

    inline void addPoints(unsigned int points) {
      m_Points += points;
    }

    inline void reducePoints(unsigned int points) {
      m_Points -= points;
    }

    // Upper left corner is origin (0, 0)
    inline void moveUp() {
      m_position.addY(-1);
    }

    inline void moveDown() {
      m_position.addY(1);
    }

    inline void moveLeft() {
      m_position.addX(-1);
    }

    inline void moveRight() {
      m_position.addX(1);
    }

    inline unsigned int getHP() const {
      return m_HP;
    }

    inline unsigned int getSpeed() const {
      return m_Speed;
    }

    inline Point getPosition() const {
      return m_position;
    }

    inline int getPoints() const {
      return m_Points;
    }

    inline unsigned int getID() const {
      return m_ID;
    }

    inline int getViewDistance() const {
      return m_viewDistance;
    }

  private:

    Dwarf() = delete;
    Dwarf(unsigned int ID) : m_maxHP(100), m_HP(m_maxHP), m_Speed(1), m_Points(0), m_viewDistance(1), m_position(), m_ID(ID) {}

    Dwarf(Dwarf&) = default; Dwarf& operator=(const Dwarf&) = default;
    Dwarf(Dwarf&&) = default; Dwarf& operator=(Dwarf&&) = default;

    unsigned int  m_maxHP;
    unsigned int  m_HP;
    unsigned int  m_Speed;
    int           m_Points;
    int           m_viewDistance;
    Point         m_position;

    unsigned int  m_ID;
  };

}

#endif