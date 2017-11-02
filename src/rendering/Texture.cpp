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

#include "src/rendering/Texture.hpp"

using namespace cdwarfs::render;

TextureManager& Texture2D::m_texManager = TextureManager::getInstance();

Texture2D::Texture2D() {}

Texture2D::Texture2D(const std::string& filepath,
                     GLint internal_format,  // format to store the image in
                     GLenum image_format,    // format the image is in
                     GLint level,            // mipmapping level
                     GLint border) :
  m_filepath(filepath),
  m_internal_format(internal_format),
  m_image_format(image_format),
  m_level(level),
  m_border(border)
{
  m_gl_texID = m_texManager.loadTexture2D(filepath,
                                          m_width,
                                          m_height,
                                          m_numChannels,
                                          internal_format,
                                          image_format,
                                          level,
                                          border);
  if (m_gl_texID == 0) throw std::runtime_error("ERROR: Could not load or create 2D texture at: " + filepath);
  init();
}

Texture2D::Texture2D(GLsizei width,
                     GLsizei height,
                     GLint internal_format,  // format to store the image in
                     GLenum image_format,    // format the image is in
                     GLint level,            // mipmapping level
                     GLint border) :
  m_width(width),
  m_height(height),
  m_internal_format(internal_format),
  m_image_format(image_format),
  m_level(level),
  m_border(border)
{
  m_gl_texID = m_texManager.createTexture2D(width,
                                            height,
                                            internal_format,
                                            image_format,
                                            level,
                                            border);
  if (m_gl_texID == 0) throw std::runtime_error("ERROR: Could not create 2D texture!");
  init();
}

Texture2D::Texture2D(const Texture2D& tex)
{
  makeDeepCopy(tex);
}

Texture2D::Texture2D(Texture2D&& tex)
{
  makeMove(std::move(tex));
}

Texture2D& Texture2D::operator=(const Texture2D& tex)
{
  if (this == &tex) return *this;
  makeDeepCopy(tex);
  return *this;
}

Texture2D& Texture2D::operator=(Texture2D&& tex)
{
  if (this == &tex)return *this;
  makeMove(std::move(tex));
  return *this;
}

void Texture2D::makeDeepCopy(const Texture2D& tex)
{
  if (tex.m_filepath.has_value()) {
    m_gl_texID = m_texManager.loadTexture2D(tex.m_filepath.value(),
                                            m_width, m_height,
                                            m_numChannels,
                                            tex.m_internal_format,
                                            tex.m_image_format,
                                            tex.m_level,
                                            tex.m_border);
    if (m_gl_texID == 0) throw std::runtime_error("ERROR: Could not load or create 2D texture at: " + m_filepath.value());
    m_filepath = std::make_optional<std::string>(tex.m_filepath.value());
  } else {
    m_gl_texID = m_texManager.createTexture2D(tex.m_width,
                                              tex.m_height,
                                              tex.m_internal_format,
                                              tex.m_image_format,
                                              tex.m_level,
                                              tex.m_border);
    if (m_gl_texID == 0) throw std::runtime_error("ERROR: Could not create 2D texture!");
    m_width = tex.m_width;
    m_height = tex.m_height;
  }
  init();
  m_internal_format = tex.m_internal_format;
  m_image_format = tex.m_image_format;
  m_level = tex.m_level;
  m_border = tex.m_border;
}

void Texture2D::makeMove(Texture2D&& tex)
{
  m_gl_texID = tex.m_gl_texID;
  m_filepath = tex.m_filepath;
  m_width = tex.m_width;
  m_height = tex.m_height;
  m_numChannels = tex.m_numChannels;
  m_internal_format = tex.m_internal_format;
  m_image_format = tex.m_image_format;
  m_level = tex.m_level;
  m_border = tex.m_border;

  tex.m_gl_texID = 0;
  tex.m_width = tex.m_height = tex.m_numChannels = 0;
  tex.m_image_format = GL_RGBA;
  tex.m_internal_format = GL_RGBA;
  tex.m_level = tex.m_border = 0;
  tex.m_filepath.reset();
}

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

Texture2D::~Texture2D()
{
  if (m_gl_texID != 0) m_texManager.deleteTexture(m_gl_texID);
}
