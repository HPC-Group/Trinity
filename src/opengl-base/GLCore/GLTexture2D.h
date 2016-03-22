#pragma once

#include <memory>
#include "GLTexture.h"
#include <silverbullet/math/Vectors.h>

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
