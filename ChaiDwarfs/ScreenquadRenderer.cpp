#include "ScreenquadRenderer.hpp"
#include "ShaderManager.hpp"
#include "Texture.hpp"

using namespace CDwarfs::render;

ScreenquadRenderer::ScreenquadRenderer(const std::shared_ptr<ShaderManager>& shaderManager) : m_shaderManager(shaderManager) {}

void ScreenquadRenderer::init(int windowWidth, int windowHeight, const std::shared_ptr<Texture2D>& displayTexture) {

  m_displayTexture = displayTexture;

  GLfloat screenquadPos[] = { -1.f, -1.f, 0.f, 0.f,   1.f, -1.f, 1.f, 0.f,   -1.f, 1.f, 0.f, 1.f,
                              -1.f,  1.f, 0.f, 1.f,   1.f, -1.f, 1.f, 0.f,    1.f, 1.f, 1.f, 1.f };

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glGenBuffers(1, &m_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(screenquadPos), screenquadPos, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, (void*)(sizeof(GLfloat) * 2));
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  m_shaderManager->compileShader(R"_(
    #version 430
    layout(location=0) in vec2 pos;
    layout(location=1) in vec2 uv;

    out vec2 ex_uv;

    void main(void){
      gl_Position = vec4(pos, 0, 1);
      ex_uv = uv;
    }
    )_", "vertexShader", GL_VERTEX_SHADER);

  m_shaderManager->compileShader(R"_(
    #version 430
    smooth in vec2 ex_uv;
    out vec4 color;

    uniform sampler2D tex;
    uniform vec2 scale;

    void main(void){
      color = texture2D(tex, ex_uv * scale);
    }
    )_", "fragmentShader", GL_FRAGMENT_SHADER);

  m_screenquadProg = m_shaderManager->createProgram("screenquadProg");
  m_shaderManager->attachShader("vertexShader", "screenquadProg");
  m_shaderManager->attachShader("fragmentShader", "screenquadProg");
  m_shaderManager->linkProgram("screenquadProg");
  m_shaderManager->deleteShader("vertexShader");
  m_shaderManager->deleteShader("fragmentShader");

  m_glsl_scaleLoc = m_shaderManager->getUniformLocation(m_screenquadProg, "scale");

  m_backgroundTexture = std::make_shared<Texture2D>("tiles\\background.png");
  auto width = static_cast<float>(windowWidth);
  auto height = static_cast<float>(windowHeight);
  m_scaleX = width / static_cast<float>(m_backgroundTexture->width());
  m_scaleY = (height / static_cast<float>(m_backgroundTexture->height()));
}

void ScreenquadRenderer::render() {
  // Enable blending to in order to see the background after rendering the field.
  glEnable(GL_BLEND);
  glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);
  glBindVertexArray(m_vao);
  m_shaderManager->useProgram(m_screenquadProg);
  glActiveTexture(GL_TEXTURE0);

  // Render background
  glBindTexture(GL_TEXTURE_2D, m_backgroundTexture->texID());
  m_shaderManager->loadUniform(m_glsl_scaleLoc, m_scaleX, m_scaleY);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  // Render field 
  glBindTexture(GL_TEXTURE_2D, m_displayTexture->texID());
  m_shaderManager->loadUniform(m_glsl_scaleLoc, 1.f, 1.f);
  glDrawArrays(GL_TRIANGLES, 0, 6);


  glBindTexture(GL_TEXTURE_2D, 0);
  m_shaderManager->resetProgram();
  glBindVertexArray(0);
  glDisable(GL_BLEND);
}