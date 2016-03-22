#pragma once

#include "GLTexture.h"

class GLTexture1D : public GLTexture {
  public:
    GLTexture1D(uint32_t iSize, GLint internalformat, GLenum format,
                GLenum type, const void *pixels = 0,
                GLint iMagFilter = GL_NEAREST, GLint iMinFilter = GL_NEAREST,
                GLint wrap = GL_CLAMP_TO_EDGE);
    virtual ~GLTexture1D() {}

    void Bind(uint32_t iUnit=0) const;
    void SetData(const void *pixels, bool bRestoreBinding=true);
    void SetData(uint32_t offset, uint32_t size, const void *pixels,
                 bool bRestoreBinding=true);

    std::shared_ptr<void> GetData();

    uint64_t GetCPUSize() const {
      return uint64_t(m_iSize*SizePerElement());
    }
    uint64_t GetGPUSize() const {
      return uint64_t(m_iSize*SizePerElement());
    }

    uint32_t GetSize() const {return uint32_t(m_iSize);}
  
    void Delete(){};


  protected:
    GLuint m_iSize;
};
