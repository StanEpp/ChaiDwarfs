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

#include <cstdint>

namespace cdwarfs
{

class EntityID
{
public:
  using UUID = uint_fast64_t;

  EntityID() = delete;

  //get Class ID
  static inline EntityID::UUID getID()
  {
    static EntityID::UUID ID = 0;
    return ++ID; // ++ID only return ID's > 0. 0 is an invalid ID
  }

  static void releaseID(EntityID::UUID)
  {
    // TODO: Implement releasing entity ID's
  }
};

}
