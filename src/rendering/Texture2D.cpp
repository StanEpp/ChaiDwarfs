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

#include "src/rendering/Texture2D.hpp"

using namespace cdwarfs::render;

Texture2D::Texture2D() {}

Texture2D::Texture2D(GLuint texID,
                     const std::string& filepath,
                     GLsizei width, GLsizei height, GLsizei numChannels,
                     GLint internal_format,  // format to store the image in
                     GLenum image_format,    // format the image is in
                     GLint level,            // mipmapping level
                     GLint border) :
  m_filepath(filepath),
  m_width(width),
  m_height(height),
  m_numChannels(numChannels),
  m_gl_texID(texID),
  m_internal_format(internal_format),
  m_image_format(image_format),
  m_level(level),
  m_border(border)
{
  init();
}

Texture2D::Texture2D(GLuint texID,
                     GLsizei width,
                     GLsizei height,
                     GLsizei numChannels,
                     GLint internal_format,  // format to store the image in
                     GLenum image_format,    // format the image is in
                     GLint level,            // mipmapping level
                     GLint border) :
  m_width(width),
  m_height(height),
  m_numChannels(numChannels),
  m_gl_texID(texID),
  m_internal_format(internal_format),
  m_image_format(image_format),
  m_level(level),
  m_border(border)
{
  init();
}

Texture2D::~Texture2D() {}

void Texture2D::init()
{
  glBindTexture(GL_TEXTURE_2D, m_gl_texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}
