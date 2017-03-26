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
#include <optional>
#include "TextureManager.hpp"

namespace cdwarfs {
  namespace render {
    class Texture2DArray {
    public:
      Texture2DArray();

      Texture2DArray(const std::vector<std::string>& m_filepaths,
        GLenum image_format = GL_RGBA       //format the imagea are in
        );

      Texture2DArray(const Texture2DArray&);
      Texture2DArray(Texture2DArray&&);
      Texture2DArray& operator=(const Texture2DArray&);
      Texture2DArray& operator=(Texture2DArray&&);

      ~Texture2DArray();

      inline GLuint texID() const { return m_gl_texID; }
      inline int width() const { return m_width; }
      inline int height() const { return m_height; }
      inline int channels() const { return m_numChannels; }
      inline GLenum imageFormat() const { return m_image_format; }

    private:
      void init();

      void makeDeepCopy(const Texture2DArray& tex);
      void makeMove(Texture2DArray&& tex);

      static TextureManager& m_texManager;

      std::optional<std::vector<std::string>> m_filepaths{};
      GLuint m_gl_texID{ 0 };
      GLenum m_image_format{ GL_RGBA };
      int m_width{ 0 };
      int m_height{ 0 };
      int m_numChannels{ 0 };
    };
  }
}

#endif // !_TEXTURE2DARRAY_HPP_

