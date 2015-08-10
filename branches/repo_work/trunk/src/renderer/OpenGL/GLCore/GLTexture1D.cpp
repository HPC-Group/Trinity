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
  \file    GLTexture1D.cpp
  \author  Jens Krueger
           SCI Institute
           University of Utah
  \date    August 2008
*/

#include "GLTexture1D.h"
//#include "GLCommon.h"
#include <iostream>

using namespace Tuvok::Renderer::OpenGL::GLCore;

GLTexture1D::GLTexture1D(uint32_t iSize, GLint internalformat, GLenum format,
                         GLenum type,
                         const GLvoid *pixels, GLint iMagFilter, GLint iMinFilter,
                         GLint wrap) :
  GLTexture(internalformat, format, type, iMagFilter, iMinFilter),
  m_iSize(GLuint(iSize))
{
  glGenTextures(1, &m_iGLID);
  glBindTexture(GL_TEXTURE_1D, m_iGLID);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, wrap);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, iMagFilter);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, iMinFilter);
  glTexImage1D(GL_TEXTURE_1D, 0, m_internalformat, m_iSize, 0, m_format,
                  m_type, (GLvoid*)pixels);

  GLenum err = glGetError();
  if (err == GL_OUT_OF_MEMORY) {
	  //throw OUT_OF_MEMORY("allocating 3d texture");
  }
  else if (err != GL_NO_ERROR) {
	  printf("[GLTexture 1D] Unknown error (%x) occurred while creating an texture.\n",
		  static_cast<unsigned int>(err));
  }
}

void GLTexture1D::SetData(uint32_t offset, uint32_t size, const void *pixels, bool bRestoreBinding) {
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  GLint prevTex=0;
  if (bRestoreBinding) {
    glGetIntegerv(GL_TEXTURE_BINDING_1D, &prevTex);
    if (GLuint(prevTex) != m_iGLID) glBindTexture(GL_TEXTURE_1D, m_iGLID);
  } else {
    glBindTexture(GL_TEXTURE_1D, m_iGLID);
  }

  glTexSubImage1D(GL_TEXTURE_1D, 0,
                     offset,
                     size,
                     m_format, m_type, (GLvoid*)pixels);

  if (bRestoreBinding && GLuint(prevTex) != m_iGLID) glBindTexture(GL_TEXTURE_1D, GLuint(prevTex));
}

void GLTexture1D::SetData(const void *pixels, bool bRestoreBinding) {
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  GLint prevTex=0;
  if (bRestoreBinding) {
    glGetIntegerv(GL_TEXTURE_BINDING_1D, &prevTex);
    if (GLuint(prevTex) != m_iGLID) glBindTexture(GL_TEXTURE_1D, m_iGLID);
  } else {
    glBindTexture(GL_TEXTURE_1D, m_iGLID);
  }

  glTexImage1D(GL_TEXTURE_1D, 0, m_internalformat, m_iSize, 0, m_format,
                  m_type, (GLvoid*)pixels);

  if (bRestoreBinding && GLuint(prevTex) != m_iGLID) glBindTexture(GL_TEXTURE_1D, GLuint(prevTex));
}

std::shared_ptr<void> GLTexture1D::GetData() {
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_1D, m_iGLID);

  const size_t sz = m_iSize * this->SizePerElement();

  std::shared_ptr<void> data(new char[sz]);

  glGetTexImage(GL_TEXTURE_1D, 0, m_format, m_type, data.get());

  return data;
}

void GLTexture1D::Bind(uint32_t iUnit) const {
  GLint iPrevUint;
  glGetIntegerv(GL_ACTIVE_TEXTURE, &iPrevUint);
  
  glActiveTexture(GLenum(GL_TEXTURE0 + iUnit));
  glBindTexture(GL_TEXTURE_1D, m_iGLID);
  
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, m_iMagFilter);
  glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, m_iMinFilter);
  
  glActiveTexture(iPrevUint);
}
