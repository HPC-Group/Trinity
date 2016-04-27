#include <stdexcept>

#include "GLTexture2D.h"
//#include "GLCommon.h"

using namespace Core::Math;

GLTexture2D::GLTexture2D(uint32_t iSizeX, uint32_t iSizeY, GLint internalformat,
                         GLenum format, GLenum type,
                         const GLvoid *pixels,
                         GLint iMagFilter, GLint iMinFilter,
                         GLint wrapX, GLint wrapY) :
  GLTexture(internalformat, format, type, iMagFilter, iMinFilter),
  m_iSizeX(GLuint(iSizeX)),
  m_iSizeY(GLuint(iSizeY))
{
  GLint prevTex;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTex);

  glGenTextures(1, &m_iGLID);
  glBindTexture(GL_TEXTURE_2D, m_iGLID);

  glPixelStorei(GL_PACK_ALIGNMENT ,1);
  glPixelStorei(GL_UNPACK_ALIGNMENT ,1);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapX);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapY);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, iMagFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, iMinFilter);
  glTexImage2D(GL_TEXTURE_2D, 0, m_internalformat,
                  GLuint(m_iSizeX),GLuint(m_iSizeY), 0, m_format, m_type,
                  (GLvoid*)pixels);

  glBindTexture(GL_TEXTURE_2D, prevTex);
}

void GLTexture2D::SetData(const Vec2ui& offset, const Vec2ui& size, const void *pixels, bool bRestoreBinding) {
  glPixelStorei(GL_PACK_ALIGNMENT ,1);
  glPixelStorei(GL_UNPACK_ALIGNMENT ,1);

  GLint prevTex=0;
  if (bRestoreBinding) glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTex);

  glBindTexture(GL_TEXTURE_2D, m_iGLID);
  glTexSubImage2D(GL_TEXTURE_2D, 0,
                     offset.x, offset.y,
                     size.x, size.y,
                     m_format, m_type, (GLvoid*)pixels);

  if (bRestoreBinding && GLuint(prevTex) != m_iGLID) glBindTexture(GL_TEXTURE_2D, prevTex);
}

void GLTexture2D::SetData(const void *pixels, bool bRestoreBinding) {
  glPixelStorei(GL_PACK_ALIGNMENT ,1);
  glPixelStorei(GL_UNPACK_ALIGNMENT ,1);

  GLint prevTex=0;
  if (bRestoreBinding) glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTex);

  glBindTexture(GL_TEXTURE_2D, m_iGLID);
  glTexImage2D(GL_TEXTURE_2D, 0, m_internalformat, m_iSizeX, m_iSizeY,
                  0, m_format, m_type, (GLvoid*)pixels);

  if (bRestoreBinding && GLuint(prevTex) != m_iGLID) glBindTexture(GL_TEXTURE_2D, prevTex);
}

std::shared_ptr<void> GLTexture2D::GetData() {
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, m_iGLID);

  const size_t sz = m_iSizeX * m_iSizeY * SizePerElement();
  std::shared_ptr<void> data(new char[sz]);
  glGetTexImage(GL_TEXTURE_2D, 0, m_format, m_type, data.get());
  return data;
}

void GLTexture2D::Bind(uint32_t iUnit) const {
  GLint iPrevUint;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &iPrevUint);
  
  glActiveTexture(GLenum(GL_TEXTURE0 + iUnit));
  glBindTexture(GL_TEXTURE_2D, m_iGLID);
  
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_iMagFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_iMinFilter);
  
  glActiveTexture(iPrevUint);
}
