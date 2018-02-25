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
#include "TerrainMap.hpp"

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/utility/utility.hpp>

using namespace cdwarfs;

void TerrainMap::load(const std::string& chaiscriptPath)
{
    chaiscript::ChaiScript chai;

    chai.add(chaiscript::fun([&](int rows, int cols) {
        if (rows <= 0 || cols <= 0)
            throw std::runtime_error("Terrain cannot have negative/empty size: " + std::to_string(rows) + " " + std::to_string(cols));

        m_map.resize(rows); // number of Rows
        std::for_each(m_map.begin(), m_map.end(), [&](auto& row) {
            row.resize(cols);
            std::for_each(row.begin(), row.end(), [](auto& val) { val = TerrainType::SOIL; });
        });
    }), "setSize");

    chaiscript::ModulePtr m = chaiscript::ModulePtr(new chaiscript::Module());
    chaiscript::utility::add_class<TerrainType>(*m,
      "TerrainType",
      { { TerrainType::SOIL, "SOIL" },
      { TerrainType::DARK, "DARK" },
      { TerrainType::PASSABLE, "PASSABLE" },
      { TerrainType::STONE, "STONE" }
      }
    );
    chai.add(m);

    chai.add(chaiscript::fun([&](int row, int col, TerrainType type) {
        m_map[row][col] = type;
    }), "setType");

    chai.add(chaiscript::fun([&]() {
        return this->rows();
    }), "numRows");

    chai.add(chaiscript::fun([&]() {
        return this->columns();
    }), "numColumns");

    std::cout << "Load map from \"" << chaiscriptPath << "\"\n";
    chai.eval_file(chaiscriptPath);
    std::cout << "Finished loading map!\n\n";
}

void TerrainMap::set(int row, int col, TerrainType newType)
{
    if (row >= rows() || row < 0 || col >= columns() || col < 0)
        return;
    m_map[row][col] = newType;
}

TerrainType TerrainMap::at(int row, int col)
{
    if (row >= rows() || row < 0 || col >= columns() || col < 0)
        return TerrainType::DARK;
    return m_map[row][col];
}
