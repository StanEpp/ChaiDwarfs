#ifndef _ORTHOGRAPHICCAMERA_HPP_
#define _ORTHOGRAPHICCAMERA_HPP_ 

#pragma warning(disable : 4201)
#include <glm\glm.hpp>
#pragma warning(default : 4201)

#include <memory>

namespace CDwarfs {
  namespace render {

    class GLFWInput;

    class OrthographicCamera {
    public:
      OrthographicCamera();

      void init(int width, int height, const std::shared_ptr<GLFWInput>& input);
      const float* mvpPtr();
      void update(double dt);

    private:
      int   m_width, m_height;
      float m_aspectratio;

      glm::mat4 m_orthoProj;
      glm::vec3 m_scale;
      glm::vec3 m_translate;
      glm::mat4 m_MVP;

      std::shared_ptr<GLFWInput> m_input;
    };

  }
}

#endif // !_ORTHOGRAPHICCAMERA_HPP_
