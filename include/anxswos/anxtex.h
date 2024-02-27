#pragma once
#include <gl/glew.h>
#include <cstdint>
#include <string>

class Texture
{
private:
  GLint m_Width;
  GLint m_Height;
  GLuint m_ID;
  void CreateTexture(GLint w, GLint h, uint8_t* data);
public:
  Texture(int w, int h);
  Texture(const std::string& filename);
  inline void Bind() { glBindTexture(GL_TEXTURE_2D, m_ID); }
  inline void UnBind() { glBindTexture(GL_TEXTURE_2D, 0); }
  inline GLuint GetID() { return m_ID; }
};

