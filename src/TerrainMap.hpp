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

#include <vector>
#include <string>
#include <algorithm>

namespace cdwarfs
{

enum class TerrainType : unsigned int
{
  SOIL = 0,
  PASSABLE,
  STONE,
  DARK,      // Everything outside the terrain bounds.
  NO_MAP,    // When there's no map at all and you try to access one.
};

class TerrainMap
{
public:
  TerrainMap() = default;
  TerrainMap(TerrainMap&) = delete; TerrainMap& operator=(const TerrainMap&) = delete;
  TerrainMap(TerrainMap&&) = delete; TerrainMap& operator=(TerrainMap&&) = delete;

  void load(const std::string& chaiscriptPath);

  TerrainType at(int row, int col);

  void set(int row, int col, TerrainType newType);

  inline int rows()
  {
    return static_cast<int>(m_map.size());
  }

  inline int columns()
  {
    return static_cast<int>(m_map[0].size());
  }

private:
  std::vector<std::vector<TerrainType>>  m_map;
};

}
