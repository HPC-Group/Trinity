#include <limits>
#define UINT32_INVALID (std::numeric_limits<uint32_t>::max())

#include "GLTexture.h"
#include "GLCommon.h"
#include <cassert>

#include "GLProgram.h"

#include <mocca/log/LogManager.h>

GLTexture::GLTexture(GLint internalformat, GLenum format,
                     GLenum type, GLint iMagFilter,
                     GLint iMinFilter) : 
  m_iGLID(UINT32_INVALID),
  m_iMagFilter(iMagFilter),
  m_iMinFilter(iMinFilter),
  m_internalformat(internalformat),
  m_format(format),
  m_type(type)
{
}

GLTexture::~GLTexture() {
  /*! \todo We'd like to call ::Delete() here, but we're not guaranteed to be
   *        in the correct context.  Instead, we'll make sure the texture was
   *        previously Deleted, or at least never initialized. */
  //assert(m_iGLID == UINT32_INVALID);

  if (m_iGLID != UINT32_INVALID)
    LWARNING("~GLTexture destructor called but OpenGL resource is not freed");
}

void GLTexture::Delete() {
  glDeleteTextures(1,&m_iGLID);
  m_iGLID = UINT32_INVALID;
}


void GLTexture::Bind(GLProgram& shader, const std::string& name) const {
  shader.SetTexture(name, (*this));
}

void GLTexture::SetFilter(GLint iMagFilter, GLint iMinFilter) {
  m_iMagFilter = iMagFilter;
  m_iMinFilter = iMinFilter;
}

size_t GLTexture::SizePerElement() const {
  return gl_byte_width(m_type) * gl_components(m_format);
}
