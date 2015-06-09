/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2008 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

/**
  \file    GLTexture2D.cpp
  \author    Jens Krueger
        SCI Institute
        University of Utah
  \date    August 2008
*/
#include <stdexcept>

#include "GLTexture2D.h"
//#include "GLCommon.h"

using namespace Tuvok::Renderer::OpenGL::GLCore;
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
