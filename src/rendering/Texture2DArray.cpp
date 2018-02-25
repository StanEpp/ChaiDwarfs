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


Texture2DArray::Texture2DArray() : m_gl_texID(0), m_width(0), m_height(0) {}

Texture2DArray::Texture2DArray(GLuint texID,
                                 const std::vector<std::string>& filepaths,
                                 GLsizei width, GLsizei height, GLsizei numChannels,
                                 GLenum image_format) : //format the images are in
    m_filepaths(filepaths),
    m_gl_texID(texID),
    m_image_format(image_format),
    m_width(width),
    m_height(height),
    m_numChannels(numChannels)
{
    init();
}

Texture2DArray::~Texture2DArray() {}

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

