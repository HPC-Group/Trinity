#pragma once

#include <opengl-base/OpenGLincludes.h>
#include <opengl-base/OpenGLError.h>
#include <silverbullet/math/Vectors.h>
#include "GLCommon.h"


class GLFBOTex{
public:
  GLFBOTex(GLenum minfilter,
           GLenum magfilter, GLenum wrapmode, GLsizei width, GLsizei height,
           GLenum intformat, GLenum format, GLenum type,
           bool bHaveDepth=false, int iNumBuffers=1);
  virtual ~GLFBOTex(void);
  virtual void SetViewport();
  virtual void Write(unsigned int iTargetBuffer=0, int iBuffer=0,
                     bool bCheckBuffer=true);
  virtual void Read(unsigned int iTargetUnit,int iBuffer=0);
  virtual void FinishWrite(int iBuffer=0);
  virtual void FinishRead(int iBuffer=0);
  virtual void ReadDepth(unsigned int iTargetUnit);
  virtual void FinishDepthRead();
  virtual void CopyToFramebuffer(unsigned int iBuffer=0);
  virtual void CopyToFramebuffer(unsigned int x, unsigned int w,
                                 unsigned int y, unsigned int h,
                                 unsigned int tx, unsigned int tw,
                                 unsigned int ty, unsigned int th,
                                 unsigned int iBuffer=0,
                                 GLenum eFilter=GL_NEAREST);
  virtual operator GLuint(void) { return m_hTexture[0]; }
  virtual operator GLuint*(void) { return m_hTexture; }

  /// \todo check how much mem an FBO really occupies
  virtual uint64_t GetCPUSize() const;
  virtual uint64_t GetGPUSize() const;

  static uint64_t EstimateCPUSize(GLsizei width, GLsizei height,
                                  size_t iSizePerElement,
                                  bool bHaveDepth=false, int iNumBuffers=1);
  static uint64_t EstimateGPUSize(GLsizei width, GLsizei height,
                                  size_t iSizePerElement,
                                  bool bHaveDepth=false, int iNumBuffers=1);

  bool Valid() const { return m_hFBO != 0; }

  static void NoDrawBuffer();
  static void OneDrawBuffer();
  static void TwoDrawBuffers();
  static void ThreeDrawBuffers();
  static void FourDrawBuffers();

  void ReadBackPixels(int x, int y, int sX, int sY, void* pData, int iBuffer=0);

  template <typename T>
  void debugOut() const {
	  std::vector<T> pData(gl_components(m_format)*m_iSizeX*m_iSizeY);
	  GL_CHECK(glReadBuffer(GL_COLOR_ATTACHMENT0));
	  GL_CHECK(glReadPixels(0, 0, m_iSizeX, m_iSizeY, m_format, m_type, pData.data()));
	  for (size_t y = 0; y < m_iSizeY; ++y) {
		  for (size_t x = 0; x < m_iSizeX; ++x) {
			  std::cout << "[";
			  for (size_t comp = 0; comp < gl_components(m_format); ++comp) {
				  std::cout << pData[comp + gl_components(m_format)*(x + m_iSizeX*y)] << ",";
			  }
			  std::cout << "] ";
		  }
		  std::cout << std::endl;
	  }
  }


  GLuint Width() const {return m_iSizeX;}
  GLuint Height() const {return m_iSizeY;}

  void SetData(const void *pixels, int iBuffer=0, bool bRestoreBinding=true);
  void SetData(const Core::Math::Vec2ui& offset, const Core::Math::Vec2ui& size,
               const void *pixels, int iBuffer=0, bool bRestoreBinding=true);

  GLuint GetTextureHandle(){ return *m_hTexture; }
  void ClearPixels(float r, float g, float b, float a);

private:
  GLuint              m_iSizeX;
  GLuint              m_iSizeY;
  GLuint*             m_hTexture;
  GLuint              m_hDepthBuffer;
  GLuint			  m_hFBO;
  GLenum*             m_LastTexUnit;
  GLenum              m_LastDepthTextUnit;
  int                 m_iNumBuffers;
  GLenum*             m_LastAttachment;
  GLenum              m_intformat;
  GLenum              m_format;
  GLenum              m_type;

  bool CheckFBO(const char* method);
  void initFBO(void);
  bool initTextures(GLenum minfilter, GLenum magfilter, GLenum wrapmode,
                    GLsizei width, GLsizei height, GLenum intformat,
                    GLenum format, GLenum type);
};
