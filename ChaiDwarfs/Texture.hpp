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

#include <optional>
#include <string>
#include "TextureManager.hpp"

namespace cdwarfs {
  namespace render {
    class Texture2D {
    public:
      Texture2D();

      Texture2D(const std::string& filepath,
        GLint internal_format = GL_RGBA,    //format to store the image in
        GLenum image_format = GL_RGBA,      //format the image is in
        GLint level = 0,                    //mipmapping level
        GLint border = 0);

      Texture2D(GLsizei width, GLsizei height,
        GLint internal_format = GL_RGBA,     //format to store the image in
        GLenum image_format = GL_RGBA,       //format the image is in
        GLint level = 0,                     //mipmapping level
        GLint border = 0);

      Texture2D(const Texture2D&);
      Texture2D(Texture2D&&);
      Texture2D& operator=(const Texture2D&);
      Texture2D& operator=(Texture2D&&);

      ~Texture2D();

      inline GLuint texID() const { return m_gl_texID; }
      inline int width() const { return m_width; }
      inline int height() const { return m_height; }
      inline int channels() const { return m_numChannels; }
      inline GLint internalFormat() const { return m_internal_format; };
      inline GLint level() const { return m_level; };
      inline GLint border() const { return m_border; };
      inline auto filename() const { return m_filepath; };
      inline GLenum imageFormat() const { return m_image_format; };

    private:
      void init();

      void makeDeepCopy(const Texture2D& tex);
      void makeMove(Texture2D&& tex);

      static TextureManager& m_texManager;

      std::optional<std::string> m_filepath{};
      int m_width{ 0 };
      int m_height{ 0 };
      int m_numChannels{ 0 };
      GLuint m_gl_texID{ 0 };
      GLint m_internal_format{ GL_RGBA };
      GLenum m_image_format{ GL_RGBA };
      GLint m_level{ 0 };
      GLint m_border{ 0 };
    };
  }
}

#endif // !_TEXTURE_HPP_

