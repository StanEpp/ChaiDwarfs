#include "Texture2DArray.hpp"

using namespace CDwarfs::render;

TextureManager& Texture2DArray::m_texManager = TextureManager::getInstance();

Texture2DArray::Texture2DArray() : m_gl_texID(0), m_width(0), m_height(0) {}

Texture2DArray::Texture2DArray(const std::vector<std::string>& filenames,
  GLenum image_format //format the images are in
  )
{
  m_gl_texID = m_texManager.loadTexture2DArray(filenames, m_width, m_height, m_numChannels, image_format);
  if (m_gl_texID == 0) throw std::runtime_error("ERROR: Could not load 2D Array which starts with: " + filenames[0]);
  init();
}

void Texture2DArray::init() {
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_gl_texID);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
  glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

Texture2DArray::~Texture2DArray() {
  m_texManager.deleteTexture(m_gl_texID);
}