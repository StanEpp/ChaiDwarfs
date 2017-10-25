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

#include "TextureManager.hpp"
#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace cdwarfs::render;

GLuint TextureManager::loadTexture2D(const std::string& filename,//where to load the file from
                                     GLsizei& width,            //contains the width of the image after successful loading
                                     GLsizei& height,           //contains the height of the image after successful loading
                                     GLsizei& numChannels,      //contains the number of channels after successful loading
                                     GLint internal_format,     //format to store the image in
                                     GLenum image_format,       //format the image is in
                                     GLint level,               //mipmapping level
                                     GLint border)
{
  auto it = m_loadedTextures.find(filename);
  if (it != m_loadedTextures.end()) {
    if (it->second->refCount > 0) {
      it->second->refCount++;
      return it->second->texID;
    }
    else {
      delete it->second;
      m_loadedTextures.erase(it);
    }
  }

  stbi_set_flip_vertically_on_load(true);
  auto data = stbi_load(filename.c_str(), &width, &height, &numChannels, 0);

  if (data == nullptr) return 0;

  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, image_format, GL_UNSIGNED_BYTE, data);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(data);

  if (texID != 0) {
    auto newTexRef = new TexRef(texID, filename);
    m_loadedTextures[filename] = newTexRef;
    m_textures[texID] = newTexRef;
  }

  return texID;
}

GLuint TextureManager::createTexture2D(GLsizei width,             //contains the width of the image after successful loading
                                       GLsizei height,            //contains the height of the image after successful loading
                                       GLint internal_format,     //format to store the image in
                                       GLenum image_format,       //format the image is in
                                       GLint level,               //mipmapping level
                                       GLint border)              //border size
{
  GLuint texID;
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, image_format, GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  if (texID != 0) {
    auto newTexRef = new TexRef(texID);
    m_textures[texID] = newTexRef;
  }

  return texID;
}

GLuint TextureManager::loadTexture2DArray(const std::vector<std::string>& filenames,
                                          GLsizei& width,
                                          GLsizei& height,
                                          GLsizei& numChannels,
                                          GLenum image_format)       //format the image is in
{
  width = height = numChannels = -1;
  GLsizei tempWidth = width;
  GLsizei tempHeight = height;
  GLsizei tempNumChannels = numChannels;
  std::vector<unsigned char*> loadedImages(filenames.size(), nullptr);

  for (size_t i = 0; i < filenames.size(); ++i) {
    auto& filename = filenames[i];
    stbi_set_flip_vertically_on_load(true);
    auto data = stbi_load(filename.c_str(), &width, &height, &numChannels, 0);
    loadedImages[i] = data;

    if (i != 0) {
      if (tempWidth != width || tempHeight != height || tempNumChannels != numChannels || data == nullptr) {
        throw std::invalid_argument("ERROR: Image \"" + filename + "\" from texture array is not of same size or depth as the others!");
      }
    } else {
      if (!data) throw std::runtime_error("ERROR: Could not load \"" + filename + "\" properly!");
      tempWidth = width;
      tempHeight = height;
      tempNumChannels = numChannels;
    }
  }

  GLuint retValue = 0;

  glGenTextures(1, &retValue);
  if (retValue != 0) {
    glBindTexture(GL_TEXTURE_2D_ARRAY, retValue);
    glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, loadedImages.size());

    for (size_t i = 0; i < loadedImages.size(); ++i) {
      glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, image_format, GL_UNSIGNED_BYTE, loadedImages[i]);
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

    auto newTexRef = new TexRef(retValue);
    m_textures[retValue] = newTexRef;
  }

  for (auto ptr : loadedImages) {
    if (ptr) stbi_image_free(ptr);
  }

  return retValue;
}

void TextureManager::deleteTexture(GLuint ID)
{
  auto it = m_textures.find(ID);
  if (it != m_textures.end()) {
    it->second->refCount--;
    if (it->second->refCount <= 0) {
      glDeleteTextures(1, &(it->second->texID));

      auto it2 = m_loadedTextures.find(it->second->filename);
      if(it2 != m_loadedTextures.end()) m_loadedTextures.erase(it2);

      delete it->second;
      m_textures.erase(it);
    }
  }
}

