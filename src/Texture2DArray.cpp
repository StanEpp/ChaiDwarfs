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

#include "Texture2DArray.hpp"

using namespace cdwarfs::render;

TextureManager& Texture2DArray::m_texManager = TextureManager::getInstance();

Texture2DArray::Texture2DArray() : m_gl_texID(0), m_width(0), m_height(0) {}

Texture2DArray::Texture2DArray(const std::vector<std::string>& filepaths,
                               GLenum image_format) //format the images are in
{
  m_gl_texID = m_texManager.loadTexture2DArray(filepaths, m_width, m_height, m_numChannels, image_format);
  if (m_gl_texID == 0) throw std::runtime_error("ERROR: Could not load 2D Array which starts with: " + filepaths[0]);
  m_filepaths = filepaths;
  init();
}

Texture2DArray::Texture2DArray(const Texture2DArray& tex)
{
  makeDeepCopy(tex);
}

Texture2DArray::Texture2DArray(Texture2DArray&& tex)
{
  makeMove(std::move(tex));
}

Texture2DArray& Texture2DArray::operator=(const Texture2DArray& tex)
{
  if (this == &tex) return *this;
  makeDeepCopy(tex);
  return *this;
}

Texture2DArray& Texture2DArray::operator=(Texture2DArray&& tex)
{
  if (this == &tex) return *this;
  makeMove(std::move(tex));
  return *this;
}

void Texture2DArray::makeDeepCopy(const Texture2DArray& tex)
{
  if (tex.m_filepaths.has_value()) {
    m_gl_texID = m_texManager.loadTexture2DArray(tex.m_filepaths.value(), m_width, m_height, m_numChannels, tex.m_image_format);
    if (m_gl_texID == 0) throw std::runtime_error("ERROR: Could not load or create 2D texture at: " + m_filepaths.value()[0]);
    m_filepaths = std::make_optional<std::vector<std::string>>(tex.m_filepaths.value());
    init();
  }
  m_image_format = tex.m_image_format;
}

void Texture2DArray::makeMove(Texture2DArray&& tex)
{
  m_gl_texID = tex.m_gl_texID;
  m_filepaths = tex.m_filepaths;
  m_width = tex.m_width;
  m_height = tex.m_height;
  m_numChannels = tex.m_numChannels;
  m_image_format = tex.m_image_format;

  tex.m_gl_texID = 0;
  tex.m_width = tex.m_height = tex.m_numChannels = 0;
  tex.m_image_format = GL_RGBA;
  tex.m_filepaths.reset();
}

void Texture2DArray::init()
{
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_gl_texID);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

Texture2DArray::~Texture2DArray()
{
  m_texManager.deleteTexture(m_gl_texID);
}
