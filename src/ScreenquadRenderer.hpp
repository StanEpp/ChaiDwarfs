#ifndef _SCREENQUADRENDERER_HPP_
#define _SCREENQUADRENDERER_HPP_

#ifndef _GL3W_
#define _GL3W_
#include "GL/gl3w.h"
#include <GLFW/glfw3.h>
#endif

#include <memory>

namespace cdwarfs {
  namespace render {
    class ShaderManager;
    class Texture2D;

    class ScreenquadRenderer {
    public:
      ScreenquadRenderer(const std::shared_ptr<ShaderManager>& shaderManager);

      void init(int windowWdth, int windowHeight, const std::shared_ptr<Texture2D>& displayTexture);
      void render();

    private:
      std::shared_ptr<ShaderManager> m_shaderManager;
      std::shared_ptr<Texture2D>     m_displayTexture;
      std::shared_ptr<Texture2D>     m_backgroundTexture;

      GLuint m_vao{ 0 };
      GLuint m_vbo{ 0 };
      GLuint m_screenquadProg{ 0 };
      GLint  m_glsl_scaleLoc{ -1 };
      float  m_scaleX{ 1.f };
      float  m_scaleY{ 1.f };
    };

  }
}

#endif // !_SCREENQUADRENDERER_HPP_
