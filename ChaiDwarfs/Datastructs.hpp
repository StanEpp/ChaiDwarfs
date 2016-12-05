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
#ifndef _DATASTRUCTS_HPP_
#define _DATASTRUCTS_HPP_

namespace CDwarfs{

class Point {
public:
  Point() : m_x(0), m_y(0) {}
  Point(int x, int y) : m_x(x), m_y(y) {}

  Point& operator+=(std::initializer_list<int> rhs) {
    addX(*rhs.begin());
    addY(*(rhs.begin() + 1));
  }

  void addX(int val) {
    m_x += val;
    if (m_x < 0) m_x = 0;
  }

  void addY(int val) {
    m_y += val;
    if (m_y < 0) m_y = 0;
  }

  int x() const {
    return m_x;
  }

  int y() const {
    return m_y;
  }

private:
  int m_x;
  int m_y;

};

}

#endif