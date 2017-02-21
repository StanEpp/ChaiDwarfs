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
*
*/
#ifndef _SPRITERENDERER_HPP_
#define _SPRITERENDERER_HPP_

#include "Entity.hpp"
#include <memory>
#include <unordered_map>
#include <string>

namespace CDwarfs {
  class EntityManager;

  namespace render {

    class SpriteRenderer {
    public:
      SpriteRenderer(std::shared_ptr<EntityManager> entManager);
      SpriteRenderer(SpriteRenderer&) = delete;
      SpriteRenderer(SpriteRenderer&&) = delete;

      void init();

      void render(double dt);

    private:
      std::shared_ptr<EntityManager> m_entManager;
    };

  }
}


#endif // !_SPRITERENDERER_HPP_