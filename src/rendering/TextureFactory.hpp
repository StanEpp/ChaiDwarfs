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
class Texture2D;
class Texture2DArray;
class WindowGLFW;

class TextureFactory : public std::enable_shared_from_this<TextureFactory>
{
public:
    TextureFactory() = delete;
    TextureFactory(const std::shared_ptr<WindowGLFW> &window);
    TextureFactory(TextureFactory&) = delete;
    TextureFactory(TextureFactory&&) = delete;
    TextureFactory& operator=(TextureFactory const&) = delete;

    std::shared_ptr<Texture2D>
      loadTexture2D(const std::string& filename,     //where to load the file from
                    GLint internal_format = GL_RGBA, //format to store the image in
                    GLenum image_format = GL_RGBA,   //format the image is in
                    GLint level = 0,                 //mipmapping level
                    GLint border = 0);               //border size

    std::shared_ptr<Texture2D>
      createTexture2D(GLsizei width,                   //width of the image
                      GLsizei height,                  //height of the image
                      GLsizei numChannels = 4,         //number of channels of the image
                      GLint internal_format = GL_RGBA, //format to store the image in
                      GLenum image_format = GL_RGBA,   //format the image is in
                      GLint level = 0,                 //mipmapping level
                      GLint border = 0);               //border size

    std::shared_ptr<Texture2DArray>
      loadTexture2DArray(const std::vector<std::string>& filenames,
                         GLenum image_format = GL_RGBA); //format the image is in

private:
    std::shared_ptr<WindowGLFW> m_window;
    std::unordered_map<std::string, std::weak_ptr<Texture2D>>  m_loadedTextures2D;
    std::unordered_map<GLuint, std::weak_ptr<Texture2D>> m_textures2D;
    std::unordered_map<GLuint, std::weak_ptr<Texture2DArray>>  m_textureArrays2D;
};

}
