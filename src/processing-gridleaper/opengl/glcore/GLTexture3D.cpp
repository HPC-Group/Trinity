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
  \file    GLTexture3D.cpp
  \author    Jens Krueger
        SCI Institute
        University of Utah
  \date    August 2008
*/

#include "GLTexture3D.h"

//! \todo add glerror
//#include "GLError.h"
//#include "Controller/Controller.h"

#include <cstdio>
#include <iostream>

using namespace Tuvok::Renderer::OpenGL::GLCore;
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
  glGetIntegerv(GL_TEXTURE_BINDING_3D, &prevTex);

  glGenTextures(1, &m_iGLID);
  glBindTexture(GL_TEXTURE_3D, m_iGLID);

  GLenum err = glGetError();
  if(err == GL_OUT_OF_MEMORY) {
    //throw OUT_OF_MEMORY("allocating 3d texture");
  } else if(err != GL_NO_ERROR) {
    printf("Unknown error (%x) occurred while setting 3D texture.\n",
           static_cast<unsigned int>(err));
  }

  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrapX);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrapY);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrapZ);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, iMagFilter);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, iMinFilter);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

err = glGetError();
  if(err == GL_OUT_OF_MEMORY) {
    //throw OUT_OF_MEMORY("allocating 3d texture");
  } else if(err != GL_NO_ERROR) {
    printf("Unknown error (%x) occurred while setting 3D texture.\n",
           static_cast<unsigned int>(err));
  }
  glTexImage3D(GL_TEXTURE_3D, 0, m_internalformat,
               m_iSizeX, m_iSizeY, m_iSizeZ,
               0, m_format, m_type, (GLvoid*)pixels);

err = glGetError();
  if(err == GL_OUT_OF_MEMORY) {
    this->Delete();
    //throw OUT_OF_MEMORY("allocating 3d texture");
  } else if(err != GL_NO_ERROR) {
    printf("Unknown error (%x) occurred while setting 3D texture.\n",
            static_cast<unsigned int>(err));
  }

  glBindTexture(GL_TEXTURE_3D, prevTex);
}

void GLTexture3D::SetData(const UINTVECTOR3& offset, const UINTVECTOR3& size,
                          const void *pixels, bool bRestoreBinding) {
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  GLint prevTex=0;
  if (bRestoreBinding) glGetIntegerv(GL_TEXTURE_BINDING_3D, &prevTex);

  glBindTexture(GL_TEXTURE_3D, m_iGLID);
  glTexSubImage3D(GL_TEXTURE_3D, 0,
                     offset.x, offset.y, offset.z,
                     size.x, size.y, size.z,
                     m_format, m_type, (GLvoid*)pixels);

  if (bRestoreBinding && GLuint(prevTex) != m_iGLID) glBindTexture(GL_TEXTURE_3D, prevTex);
}

void GLTexture3D::SetData(const void *pixels, bool bRestoreBinding) {
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  GLint prevTex=0;
  if (bRestoreBinding) glGetIntegerv(GL_TEXTURE_BINDING_3D, &prevTex);

  glBindTexture(GL_TEXTURE_3D, m_iGLID);
  glTexImage3D(GL_TEXTURE_3D, 0, m_internalformat,
               m_iSizeX, m_iSizeY, m_iSizeZ,
               0, m_format, m_type, (GLvoid*)pixels);
  GLenum err = glGetError();
  if(err == GL_OUT_OF_MEMORY) {
    this->Delete();
    //throw OUT_OF_MEMORY("allocating 3d texture");
  } else if(err != GL_NO_ERROR) {
    //WARNING("Unknown error (%x) occurred while setting 3D texture.",
    //        static_cast<unsigned int>(err));
  }

  if (bRestoreBinding && GLuint(prevTex) != m_iGLID) glBindTexture(GL_TEXTURE_3D, prevTex);
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
