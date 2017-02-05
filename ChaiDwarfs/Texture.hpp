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

#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#ifndef _GL3W_
#define _GL3W_
#include <GL\gl3w.h>
#include <GLFW\glfw3.h>
#endif

#include "TextureManager.hpp"

namespace CDwarfs {
  namespace render {
    class Texture2D {
    public:
      Texture2D();

      Texture2D(const std::string& filename,
        GLint internal_format = GL_RGBA,    //format to store the image in
        GLenum image_format = GL_RGBA,      //format the image is in
        GLint level = 0,                    //mipmapping level
        GLint border = 0);

      Texture2D(GLsizei width, GLsizei height,
        GLint internal_format = GL_RGBA,     //format to store the image in
        GLenum image_format = GL_RGBA,       //format the image is in
        GLint level = 0,                     //mipmapping level
        GLint border = 0);

      ~Texture2D();

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

#endif // !_TEXTURE_HPP_

