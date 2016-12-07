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

#ifndef _BASECOMPONENT_HPP_
#define _BASECOMPONENT_HPP_

#include <cstdint>

namespace CDwarfs {

  using  ComponentUUID = uint_fast64_t;

  class BaseComponent {
  public:
    //get unique Component Class ID
    static inline ComponentUUID getCID() {
      static ComponentUUID ID = 0;
      return ID++;
    }

    virtual ~BaseComponent() {}
  };

  template<class T>
  class Component : public BaseComponent {
  public:
    static ComponentUUID componentTypeID;
  };

  template<typename T>
  ComponentUUID Component<T>::componentTypeID = BaseComponent::getCID();

}

#endif // !_BASECOMPONENT_HPP_