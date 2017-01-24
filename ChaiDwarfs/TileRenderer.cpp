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

#include "TileRenderer.hpp"
#include "TerrainMap.hpp"

using namespace CDwarfs;
using namespace CDwarfs::render;

TileRenderer::TileRenderer(const std::shared_ptr<TerrainMap>& terrainMap) : m_terrainMap(terrainMap) {}

TileRenderer::~TileRenderer(){}

void TileRenderer::init() {

}

void TileRenderer::render() {

}