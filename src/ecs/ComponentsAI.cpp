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

#include "ComponentsAI.hpp"
#include "PathResolver.hpp"
#include "src/TerrainMap.hpp"
#include "src/interfaces/TerrainObjectInterface.hpp"
#include "src/interfaces/TerrainInterface.hpp"

using namespace cdwarfs::comp;

DwarfAI::DwarfAI() {}

DwarfAI::DwarfAI(const std::string& filePath) :
    filePath(pathRes.script(filePath))
{
    chaiscript::ModulePtr m = chaiscript::ModulePtr(new chaiscript::Module());
    chaiscript::utility::add_class<TerrainType>(*m,
        "TerrainType",
        { { TerrainType::SOIL, "SOIL" },
          { TerrainType::DARK, "DARK" },
          { TerrainType::PASSABLE, "PASSABLE"},
          { TerrainType::NO_MAP, "NOMAP" },
          { TerrainType::STONE, "STONE" }
        }
    );
    chai.add(m);
}
