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

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#ifndef _GL3W_
#define _GL3W_
#include "GL/gl3w.h"
#include <GLFW/glfw3.h>
#endif

namespace cdwarfs::render
{

class TextureManager
{
public:
  TextureManager(TextureManager&) = delete;
  TextureManager(TextureManager&&) = delete;
  TextureManager& operator=(TextureManager const&) = delete;

  GLuint loadTexture2D(const std::string& filename,     //where to load the file from
                       GLsizei& width,                  //contains the width of the image after successful loading
                       GLsizei& height,                 //contains the height of the image after successful loading
                       GLsizei& numChannels,            //contains the number of channels after successful loading
                       GLint internal_format = GL_RGB,  //format to store the image in
                       GLenum image_format = GL_RGB,    //format the image is in
                       GLint level = 0,                 //mipmapping level
                       GLint border = 0);               //border size

  GLuint loadTexture2DArray(const std::vector<std::string>& filenames,
                            GLsizei& width,
                            GLsizei& height,
                            GLsizei& numChannels,
                            GLenum image_format);       //format the image is in

  GLuint createTexture2D(GLsizei width,                   //contains the width of the image after successful loading
                         GLsizei height,                  //contains the height of the image after successful loading
                         GLint internal_format = GL_RGB,  //format to store the image in
                         GLenum image_format = GL_RGB,    //format the image is in
                         GLint level = 0,                 //mipmapping level
                         GLint border = 0);               //border size

  void deleteTexture(GLuint ID);

  static TextureManager& getInstance()
  {
    static TextureManager texManager;
    return texManager;
  }

private:
  TextureManager() = default;

  struct TexRef
  {
    GLuint texID{ 0 };
    unsigned int refCount{ 0 };
    std::string filename;
    TexRef() {}
    TexRef(GLuint texID) : texID(texID), refCount(1){}
    TexRef(GLuint texID, const std::string& filename) : texID(texID), refCount(1), filename(filename){}
  };

  std::unordered_map<std::string, TexRef*>  m_loadedTextures;
  std::unordered_map<GLuint, TexRef*> m_textures;

};

}
