#include <cstdio>
#include <iostream>

#include "GLTexture3D.h"

#include "mocca/log/LogManager.h"
#include "opengl-base/OpenGLError.h"

using namespace Core::Math;

GLTexture3D::GLTexture3D(uint32_t iSizeX, uint32_t iSizeY, uint32_t iSizeZ,
                         GLint internalformat, GLenum format, GLenum type,
                         const GLvoid *pixels,
                         GLint iMagFilter, GLint iMinFilter, GLint wrapX,
                         GLint wrapY, GLint wrapZ) :
GLTexture(internalformat, format, type, iMagFilter, iMinFilter),
m_iSizeX(GLuint(iSizeX)),
m_iSizeY(GLuint(iSizeY)),
m_iSizeZ(GLuint(iSizeZ))
{
  GLint prevTex;
  GL_CHECK(glGetIntegerv(GL_TEXTURE_BINDING_3D, &prevTex));
  GL_CHECK(glGenTextures(1, &m_iGLID));
  GL_CHECK(glBindTexture(GL_TEXTURE_3D, m_iGLID));
  GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrapX));
  GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrapY));
  GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrapZ));
  GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, iMagFilter));
  GL_CHECK(glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, iMinFilter));
  
  GL_CHECK(glPixelStorei(GL_PACK_ALIGNMENT, 1));
  GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
  
  // don't GL_CHECK this call, under certain conditions this call is expected
  // to fail (e.g. when we don't know how much memeory is left we may try to
  // create a texture and see what happens)
  glTexImage3D(GL_TEXTURE_3D, 0, m_internalformat,
               m_iSizeX, m_iSizeY, m_iSizeZ,
               0, m_format, m_type, (GLvoid*)pixels);
  
  GLenum err = glGetError();
  if(err == GL_OUT_OF_MEMORY) {
#ifndef NDEBUG
    LWARNING("out of memory allocating 3d texture");
#endif
    m_iGLID = 0;
  } else if(err != GL_NO_ERROR) {
#ifndef NDEBUG
    LWARNING("Unknown error ( " <<
            static_cast<unsigned int>(err) <<
            ") occurred while setting 3D texture");
#endif
    m_iGLID = 0;
  }
  
  GL_CHECK(glBindTexture(GL_TEXTURE_3D, prevTex));
}

void GLTexture3D::SetData(const UINTVECTOR3& offset, const UINTVECTOR3& size,
                          const void *pixels, bool bRestoreBinding) {
  GL_CHECK(glPixelStorei(GL_PACK_ALIGNMENT, 1));
  GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
  
  GLint prevTex=0;
  if (bRestoreBinding) GL_CHECK(glGetIntegerv(GL_TEXTURE_BINDING_3D, &prevTex));
  
  GL_CHECK(glBindTexture(GL_TEXTURE_3D, m_iGLID));
  GL_CHECK(glTexSubImage3D(GL_TEXTURE_3D, 0,
                           offset.x, offset.y, offset.z,
                           size.x, size.y, size.z,
                           m_format, m_type, (GLvoid*)pixels));
  
  if (bRestoreBinding && GLuint(prevTex) != m_iGLID)
    glBindTexture(GL_TEXTURE_3D, prevTex);
}

void GLTexture3D::SetData(const void *pixels, bool bRestoreBinding) {
  GL_CHECK(glPixelStorei(GL_PACK_ALIGNMENT, 1));
  GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
  
  GLint prevTex=0;
  if (bRestoreBinding) GL_CHECK(glGetIntegerv(GL_TEXTURE_BINDING_3D, &prevTex));
  
  GL_CHECK(glBindTexture(GL_TEXTURE_3D, m_iGLID));
  glTexImage3D(GL_TEXTURE_3D, 0, m_internalformat,
               m_iSizeX, m_iSizeY, m_iSizeZ,
               0, m_format, m_type, (GLvoid*)pixels);
  GLenum err = glGetError();
  if(err == GL_OUT_OF_MEMORY) {
    this->Delete();
    LERROR("out of memory allocating 3d texture");
  } else if(err != GL_NO_ERROR) {
    LERROR("Unknown error " << static_cast<unsigned int>(err) <<
           "occurred while setting 3D texture");
  }
  
  if (bRestoreBinding && GLuint(prevTex) != m_iGLID)
    glBindTexture(GL_TEXTURE_3D, prevTex);
}

std::shared_ptr<void> GLTexture3D::GetData()
{
  glPixelStorei(GL_PACK_ALIGNMENT ,1);
  glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
  glBindTexture(GL_TEXTURE_3D, m_iGLID);
  
  const size_t sz = m_iSizeX * m_iSizeY * m_iSizeZ * SizePerElement();
  std::shared_ptr<void> data(new char[sz]);
  glGetTexImage(GL_TEXTURE_3D, 0, m_format, m_type, data.get());
  return data;
}
