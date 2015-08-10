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
  \file    GLTexture2D.h
  \author  Jens Krueger
           SCI Institute
           University of Utah
  \date    August 2008
*/
#pragma once

#ifndef GLTEXTURE2D_H
#define GLTEXTURE2D_H

#include <memory>
#include "GLTexture.h"
#include <core/Math/Vectors.h>

namespace Tuvok {
  namespace Renderer {
    namespace OpenGL {
      namespace GLCore {

class GLTexture2D : public GLTexture {
  public:
    GLTexture2D(uint32_t iSizeX, uint32_t iSizeY, GLint internalformat,
                GLenum format, GLenum type, const GLvoid *pixels = 0,
                GLint iMagFilter = GL_NEAREST, GLint iMinFilter = GL_NEAREST,
                GLint wrapX = GL_CLAMP_TO_EDGE, GLint wrapY = GL_CLAMP_TO_EDGE
    );
    virtual ~GLTexture2D() {}

    void Bind(uint32_t iUnit=0) const;

    void SetData(const void *pixels, bool bRestoreBinding=true);
    void SetData(const Core::Math::Vec2ui& offset, const Core::Math::Vec2ui& size,
                 const void *pixels, bool bRestoreBinding=true);

    std::shared_ptr<void> GetData();

    uint64_t GetCPUSize() const {
      return uint64_t(m_iSizeX*m_iSizeY*SizePerElement());
    }
    uint64_t GetGPUSize() const {
      return uint64_t(m_iSizeX*m_iSizeY*SizePerElement());
    }

    Core::Math::Vec2ui GetSize() const {
      return Core::Math::Vec2ui(uint32_t(m_iSizeX), uint32_t(m_iSizeY));
    }


  protected:
    GLuint m_iSizeX;
    GLuint m_iSizeY;
};
typedef std::shared_ptr<GLTexture2D> GLTexture2DPtr;

      };
    };
  };
};
#endif // GLTEXTURE2D_H
