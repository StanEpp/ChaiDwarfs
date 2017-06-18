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

#include <iostream>

#include "Components.hpp"
#include "ChaiDwarfs.hpp"

int main(){

  //std::cout << cdwarfs::TouchValue::componentTypeID << '\n';
  //std::cout << cdwarfs::TouchDamage::componentTypeID << '\n';
  //std::cout << cdwarfs::Name::componentTypeID << '\n';
  //std::cout << cdwarfs::Position::componentTypeID << '\n';
  //std::cout << cdwarfs::DwarfHP::componentTypeID << '\n';
  //std::cout << cdwarfs::DwarfPosition::componentTypeID << '\n';

  //std::cin.get();

  try {
    cdwarfs::ChaiDwarfs app;
    app.init();
    app.run();
  }
  catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    std::cin.get();
  }

  return 0;
}