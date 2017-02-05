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

#ifndef _TEXTURE2DARRAY_HPP_
#define _TEXTURE2DARRAY_HPP_

#ifndef _GL3W_
#define _GL3W_
#include <GL\gl3w.h>
#include <GLFW\glfw3.h>
#endif

#include <vector>
#include <string>
#include "TextureManager.hpp"

namespace CDwarfs {
  namespace render {
    class Texture2DArray {
    public:
      Texture2DArray();

      Texture2DArray(const std::vector<std::string>& filenames,
        GLenum image_format = GL_RGBA       //format the imagea are in
        );

      ~Texture2DArray();

      inline GLuint texID() { return m_gl_texID; }
      inline int width() { return m_width; }
      inline int height() { return m_height; }
      inline int channels() { return m_numChannels; }

    private:
      void init();

      static TextureManager& m_texManager;
      GLuint m_gl_texID;
      int m_width, m_height, m_numChannels;
    };
  }
}

#endif // !_TEXTURE2DARRAY_HPP_

