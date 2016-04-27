#pragma once

#include "silverbullet/base/StdDefines.h"
#include "opengl-base/OpenGLincludes.h"
#include <string>
#include <memory>

class GLProgram;

/** \class GLTexture
 * Abstracted texture usage.
 *
 * GLTexture loads and unloads textures from the GPU. */
class GLTexture{
public:
  /** Constructs an invalid texture.
   * A texture is created with an invalid GL identifier.  No data are
   * pushed to the GPU.  Derived classes are expected to generate the
   * texture in their constructor[s]. */
  GLTexture(GLint internalformat, GLenum format,
            GLenum type, GLint iMagFilter,
            GLint iMinFilter);
  virtual ~GLTexture();
  
  /** Removes this texture from the OpenGL context. */
  virtual void Delete();
  
  /** Initializes the texture data with the given data. */
  virtual void SetData(const void *pixels, bool bRestoreBinding=true) = 0;
  
  /// bind this texture to the given texture unit
  virtual void Bind(uint32_t iUnit=0) const  = 0;
  
  /// bind this texture the given texture in a specific shader
  void Bind(GLProgram& shader, const std::string& name) const;
  
  /// set the texture filter mode (must be called before a bind call to be in effect)
  void SetFilter(GLint iMagFilter = GL_NEAREST, GLint iMinFilter = GL_NEAREST);
  
  /** \return The OpenGL identifier for this texture. */
  GLuint GetGLID() const {return m_iGLID;}
  
  bool isValid() const {return m_iGLID != 0;}
  
  /// expensive read back of texture data
  virtual std::shared_ptr<void> GetData() = 0;
  
  virtual uint64_t GetGPUSize() const = 0;
  virtual uint64_t GetCPUSize() const = 0;
  
protected:
  GLuint m_iGLID;
  GLint  m_iMagFilter;
  GLint  m_iMinFilter;
  GLint  m_internalformat;
  GLenum m_format;
  GLenum m_type;
  
  size_t SizePerElement() const;
};
