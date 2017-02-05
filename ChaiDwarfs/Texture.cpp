#include "Texture.hpp"

using namespace CDwarfs::render;

TextureManager& Texture2D::m_texManager = TextureManager::getInstance();

Texture2D::Texture2D() : m_gl_texID(0), m_width(0), m_height(0) {}

Texture2D::Texture2D(const std::string& filename,
  GLint internal_format,     //format to store the image in
  GLenum image_format,       //format the image is in
  GLint level,               //mipmapping level
  GLint border) 
{
  m_gl_texID = m_texManager.loadTexture2D(filename, m_width, m_height, m_numChannels, internal_format, image_format, level, border);
  if (m_gl_texID == 0) throw std::runtime_error("ERROR: Could not load or create 2D texture at: " + filename);
  init();
}


Texture2D::Texture2D(GLsizei width, GLsizei height,
  GLint internal_format,     //format to store the image in
  GLenum image_format,       //format the image is in
  GLint level,               //mipmapping level
  GLint border) : m_gl_texID(0), m_width(width), m_height(height) 
{
  m_gl_texID = m_texManager.createTexture2D(width, height, internal_format, image_format, level, border);
  if (m_gl_texID == 0) throw std::runtime_error("ERROR: Could not create 2D texture!");
  init();
}

void Texture2D::init() {
  glBindTexture(GL_TEXTURE_2D, m_gl_texID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D() {
  m_texManager.deleteTexture(m_gl_texID);
}