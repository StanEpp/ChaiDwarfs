/*
*  Copyright(c) 2016 - 2018 Stanislaw Eppinger
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
*
*/
#pragma once

#include <memory>
#include <string>

#include "BaseComponent.hpp"
#include <chaiscript/chaiscript.hpp>

namespace cdwarfs
{
    class TerrainInterface;
    class TerrainObjectInterface;
}

namespace cdwarfs::comp
{

struct DwarfAI : public Component<DwarfAI>
{
    std::string filePath;
    ::chaiscript::ChaiScript chai;
    std::function<void()> entryPoint;
    std::unique_ptr<::cdwarfs::TerrainInterface> terrain;
    std::unique_ptr<::cdwarfs::TerrainObjectInterface> terrainObj;
    bool initialized{ false };

    DwarfAI();
    DwarfAI(const std::string& filePath);
};

}
