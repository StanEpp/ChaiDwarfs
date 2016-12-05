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

#include "DwarfComponents.hpp"
#include "ObjectComponents.hpp"
#include "ChaiDwarfs.hpp"

#include <queue>

struct EL{
  int pr;
  std::string str;
  EL(int pr, const std::string& str) : pr(pr), str(str) {}
};

bool operator<(const EL& lhs, const EL& rhs) {
  return lhs.pr < rhs.pr;
}

bool operator<=(const EL& lhs, const EL& rhs) {
  return lhs.pr <= rhs.pr;
}


int main(){

  //std::cout << CDwarfs::TouchValue::componentTypeID << '\n';
  //std::cout << CDwarfs::TouchDamage::componentTypeID << '\n';
  //std::cout << CDwarfs::Name::componentTypeID << '\n';
  //std::cout << CDwarfs::Position::componentTypeID << '\n';
  //std::cout << CDwarfs::DwarfHP::componentTypeID << '\n';
  //std::cout << CDwarfs::DwarfPosition::componentTypeID << '\n';

  //std::priority_queue<EL> qu;

  //qu.emplace(1, "3");
  //qu.emplace(3, "2");
  //qu.emplace(1, "4");
  //qu.emplace(4, "1");
  //
  //while (!qu.empty()) {
  //  std::cout << qu.top().str << '\n';
  //  qu.pop();
  //}

  //std::cin.get();

  try {
    CDwarfs::ChaiDwarfs app;
    app.init();
    app.run();
  }
  catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}