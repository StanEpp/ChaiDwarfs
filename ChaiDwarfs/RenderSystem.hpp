#ifndef _RENDERSYSTEM_HPP_
#define _RENDERSYSTEM_HPP_

#ifndef _GL3W_
#define _GL3W_
#include <GL\gl3w.h>
#include <GLFW\glfw3.h>
#endif

#include <memory>
#include <string>

namespace CDwarfs {

  class TerrainObjectSystem;
  class TerrainMap;
  class EntityManager;
  class DwarfSystem;

  namespace render {

    class WindowGLFW;
    class TileRenderer;
    class ShaderManager;

    class RenderSystem {
    public:
      RenderSystem() = delete;
      RenderSystem(const std::shared_ptr<TerrainObjectSystem>&, const std::shared_ptr<TerrainMap>&,
        const std::shared_ptr<EntityManager>&, const std::shared_ptr<DwarfSystem>&);
      ~RenderSystem();
      
      void init(int wnd_Width, int wnd_Height, const std::string& windowName);
      void render();
      
    private:
      std::shared_ptr<TerrainObjectSystem> m_terrainObjSys;
      std::shared_ptr<TerrainMap>          m_terrainMap;
      std::shared_ptr<EntityManager>       m_entManager;
      std::shared_ptr<DwarfSystem>         m_dwarfSys;

      std::shared_ptr<ShaderManager>       m_shaderManager;
      std::shared_ptr<TileRenderer>        m_tileRend;

      std::shared_ptr<WindowGLFW>          m_window;

      GLuint m_vao, m_vbo;
      GLuint m_screenquadProg;

    };

  }
}

#endif // !_RENDERSYSTEM_HPP_
