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

#include <memory>
#include <string>

namespace cdwarfs
{
  class TerrainObjectSystem;
  class TerrainMap;
  class EntityManager;
  class DwarfSystem;
}

namespace cdwarfs::render
{

class WindowGLFW;
class TileRenderer;
class ScreenquadRenderer;
class ShaderManager;
class Texture2D;
class OrthographicCamera;
class GLFWInput;
class SpriteRenderer;
class TextureFactory;

class RenderSystem
{
public:
    RenderSystem() = delete;
    RenderSystem(const std::shared_ptr<TerrainObjectSystem>&,
                 const std::shared_ptr<TerrainMap>&,
                 const std::shared_ptr<EntityManager>&,
                 const std::shared_ptr<GLFWInput>&);
    ~RenderSystem();

    void init(int wnd_Width, int wnd_Height, const std::string& windowName);
    void render(double dt);

    std::shared_ptr<WindowGLFW> getWindow();
    const std::shared_ptr<TileRenderer> getTileRenderer();
    const std::shared_ptr<SpriteRenderer> getSpriteRenderer();

private:
    // Order is important! Window needs to be destroyed last to keep the
    // openGL context alive until everything else is destroyed.
    std::shared_ptr<WindowGLFW>          m_window;
    std::shared_ptr<TextureFactory>      m_textureFactory;
    std::shared_ptr<TerrainObjectSystem> m_terrainObjSys;
    std::shared_ptr<TerrainMap>          m_terrainMap;
    std::shared_ptr<EntityManager>       m_entManager;

    std::shared_ptr<ShaderManager>       m_shaderManager;
    std::shared_ptr<TileRenderer>        m_tileRend;
    std::shared_ptr<ScreenquadRenderer>  m_screenquad;
    std::shared_ptr<SpriteRenderer>      m_spriteRend;

    std::shared_ptr<Texture2D>           m_outputTexture;

    std::shared_ptr<OrthographicCamera>  m_camera;

    std::shared_ptr<GLFWInput>           m_input;
};

}
