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

#include "src/rendering/TextureFactory.hpp"
#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Texture2D.hpp"
#include "Texture2DArray.hpp"

using namespace cdwarfs::render;

TextureFactory::TextureFactory(const std::shared_ptr<WindowGLFW> &window) :
    m_window(window)
{}

std::shared_ptr<Texture2D>
  TextureFactory::loadTexture2D(const std::string& filename,//where to load the file from
                                GLint internal_format,     //format to store the image in
                                GLenum image_format,       //format the image is in
                                GLint level,               //mipmapping level
                                GLint border)
{
    auto it = m_loadedTextures2D.find(filename);
    if (it != m_loadedTextures2D.end()) {
        if (auto retPtr = it->second.lock()) {
            return retPtr;
        } else {
            m_loadedTextures2D.erase(it);
        }
    }

    int width = 0;
    int height = 0;
    int numChannels = 0;

    stbi_set_flip_vertically_on_load(true);
    auto data = stbi_load(filename.c_str(), &width, &height, &numChannels, 0);

    if (data == nullptr) return nullptr;

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, image_format, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    if (texID != 0) {
        auto tex = std::shared_ptr<Texture2D>(
            new Texture2D(texID,
                          filename,
                          width, height, numChannels,
                          internal_format, image_format,
                          level, border),
            [factory=this->shared_from_this()](Texture2D *tex){
                auto it = factory->m_loadedTextures2D.find(*(tex->filename()));
                if (it != factory->m_loadedTextures2D.end() &&
                    it->second.use_count() <= 1)
                {
                    const auto texID = tex->texID();
                    glDeleteTextures(1, &texID);
                    factory->m_loadedTextures2D.erase(it);
                    delete tex;
                }
        });
        m_loadedTextures2D[filename] = tex;
        return tex;
    }

    return nullptr;
}

std::shared_ptr<Texture2D>
  TextureFactory::createTexture2D(GLsizei width,
                                  GLsizei height,
                                  GLsizei numChannels,
                                  GLint internal_format,
                                  GLenum image_format,
                                  GLint level,
                                  GLint border)
{
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, image_format, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (texID != 0) {
        auto tex = std::shared_ptr<Texture2D>(
            new Texture2D(texID,
                          width, height, numChannels,
                          internal_format, image_format,
                          level, border) ,
            [factory=this->shared_from_this()](Texture2D *tex) {
                      auto it = factory->m_textures2D.find(tex->texID());
                      if (it != factory->m_textures2D.end() &&
                          it->second.use_count() <= 1)
                      {
                          const auto texID = tex->texID();
                          glDeleteTextures(1, &texID);
                          factory->m_textures2D.erase(it);
                          delete tex;
                      }
              });
        m_textures2D[texID] = tex;
        return tex;
    }

    return nullptr;
}

std::shared_ptr<Texture2DArray>
  TextureFactory::loadTexture2DArray(const std::vector<std::string>& filenames,
                                     GLenum image_format)  //format the image is in
{
    int width, height, numChannels;
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

    GLuint texID = 0;

    glGenTextures(1, &texID);
    if (texID != 0) {
        glBindTexture(GL_TEXTURE_2D_ARRAY, texID);
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, width, height, loadedImages.size());

        for (size_t i = 0; i < loadedImages.size(); ++i) {
            glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, image_format, GL_UNSIGNED_BYTE, loadedImages[i]);
        }

        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);

        auto tex = std::shared_ptr<Texture2DArray>(
            new Texture2DArray(texID,
                               filenames,
                               width, height, numChannels,
                               image_format),
            [factory=this->shared_from_this()](Texture2DArray *tex){
                auto it = factory->m_textureArrays2D.find(tex->texID());
                if (it != factory->m_textureArrays2D.end() &&
                    it->second.use_count() <= 1)
                {
                    const auto texID = tex->texID();
                    glDeleteTextures(1, &texID);
                    factory->m_textureArrays2D.erase(it);
                    delete tex;
                }
            }
        );

        m_textureArrays2D[texID] = tex;

        for (auto ptr : loadedImages) {
            if (ptr) stbi_image_free(ptr);
        }

        return tex;
    }

    return nullptr;
}
