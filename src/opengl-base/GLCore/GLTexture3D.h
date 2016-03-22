#pragma once

#include <memory>
#include "GLTexture.h"
#include <silverbullet/math/Vectors.h>

class GLTexture3D : public GLTexture {
  public:
    GLTexture3D(uint32_t iSizeX, uint32_t iSizeY, uint32_t iSizeZ,
                GLint internalformat, GLenum format, GLenum type,
                const GLvoid *pixels = 0,
                GLint iMagFilter = GL_NEAREST,
                GLint iMinFilter = GL_NEAREST,
                GLint wrapX = GL_CLAMP_TO_EDGE,
                GLint wrapY = GL_CLAMP_TO_EDGE,
                GLint wrapZ = GL_CLAMP_TO_EDGE);
    virtual ~GLTexture3D() {}

    void Bind(uint32_t iUnit=0) const {
      GLint iPrevUint;
      glGetIntegerv(GL_ACTIVE_TEXTURE, &iPrevUint);

      glActiveTexture(GLenum(GL_TEXTURE0 + iUnit));
      glBindTexture(GL_TEXTURE_3D, m_iGLID);

      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, m_iMagFilter);
      glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, m_iMinFilter);

      glActiveTexture(iPrevUint);
    }

    void SetData(const void *pixels, bool bRestoreBinding=true);
    void SetData(const Core::Math::Vec3ui& offset, const Core::Math::Vec3ui& size,
                 const void *pixels, bool bRestoreBinding=true);

    std::shared_ptr<void> GetData();

    uint64_t GetCPUSize() const {
      return uint64_t(m_iSizeX*m_iSizeY*m_iSizeZ*SizePerElement());
    }
    uint64_t GetGPUSize() const {
      return uint64_t(m_iSizeX*m_iSizeY*m_iSizeZ*SizePerElement());
    }

    Core::Math::Vec3ui GetSize() const {
      return Core::Math::Vec3ui(uint32_t(m_iSizeX), uint32_t(m_iSizeY),
                         uint32_t(m_iSizeZ));
    }


  protected:
    GLuint m_iSizeX;
    GLuint m_iSizeY;
    GLuint m_iSizeZ;
};
typedef std::shared_ptr<GLTexture3D> GLTexture3DPtr;
