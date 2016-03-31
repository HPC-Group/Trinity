#include "GLFrameBufferObject.h"

#ifdef WIN32
  #ifndef DEBUG
    #pragma warning( disable : 4189 ) // disable unused var warning
  #endif
#endif

GLFBO::GLFBO() {
  glGenFramebuffers(1, &m_hFBO);
  GLenum glerr = glGetError();
  if (GL_NO_ERROR != glerr) {
    GL_CHECK(glDeleteFramebuffers(1, &m_hFBO));
    m_hFBO = 0;
    return;
  }
}

GLFBO::~GLFBO() {
  GL_CHECK(glDeleteFramebuffers(1, &m_hFBO));
}

GLuint GLFBO::getGLID() const {
  return m_hFBO;
}


GLRenderTexture::GLRenderTexture(std::shared_ptr<GLFBO> glFBO,
                   GLenum minfilter,
                   GLenum magfilter, GLenum wrapmode, GLsizei width,
                   GLsizei height, GLenum intformat, GLenum format, GLenum type,
                   bool bHaveDepth, int iNumBuffers) :
  m_iSizeX(width),
  m_iSizeY(height),
  m_hTexture(new GLuint[iNumBuffers]),
  m_glFBO(glFBO),
  m_LastTexUnit(NULL),
  m_LastDepthTextUnit(0),
  m_iNumBuffers(iNumBuffers),
  m_LastAttachment(NULL),
  m_intformat(intformat),
  m_format(format),
  m_type(type)
{
  if (width<1) width=1;
  if (height<1) height=1;
  assert(iNumBuffers>0);
  assert(iNumBuffers<5);
  m_LastTexUnit=new GLenum[iNumBuffers];
  m_LastAttachment=new GLenum[iNumBuffers];
  for (int i=0; i<m_iNumBuffers; i++) {
    m_LastTexUnit[i]=0;
    m_LastAttachment[i]=GL_COLOR_ATTACHMENT0+i;
    m_hTexture[i]=0;
  }

  while(glGetError() != GL_NO_ERROR) { ; } // clear error state.
  if (!initTextures(minfilter,magfilter,wrapmode,width,height,intformat, format, type))
  {
      //T_ERROR("GL Error during texture creation!");
      GL_CHECK(glDeleteTextures(m_iNumBuffers,m_hTexture));
      delete[] m_hTexture;
      m_hTexture=NULL;
      return;
  }
  if (bHaveDepth) {
#ifdef GLRenderTexture_DEPTH_RENDERBUFFER
    GL_CHECK(glGenRenderbuffers(1,&m_hDepthBuffer));
    GL_CHECK(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                                width,height));
#else
    GL_CHECK(glGenTextures(1,&m_hDepthBuffer));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D,m_hDepthBuffer));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                    GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
#endif
  }
  else m_hDepthBuffer=0;
}


/**
 * Destructor: Delete texture object. If no more instances of
 * GLRenderTexture are around, the FBO is deleted as well.
 */
GLRenderTexture::~GLRenderTexture(void) {
  if (m_hTexture) {
    GL_CHECK(glDeleteTextures(m_iNumBuffers,m_hTexture));
    delete[] m_hTexture;
    m_hTexture=NULL;
  }
  delete[] m_LastTexUnit;
  m_LastTexUnit=NULL;

  delete[] m_LastAttachment;
  m_LastAttachment=NULL;

#ifdef GLRenderTexture_DEPTH_RENDERBUFFER
  if (m_hDepthBuffer) GL_CHECK(glDeleteRenderbuffers(1,&m_hDepthBuffer));
#else
  if (m_hDepthBuffer) GL_CHECK(glDeleteTextures(1,&m_hDepthBuffer));
#endif
  m_hDepthBuffer=0;
  m_glFBO = nullptr;
}



/**
 * Build a dummy texture according to the parameters.
 */
bool GLRenderTexture::initTextures(GLenum minfilter, GLenum magfilter,
                            GLenum wrapmode, GLsizei width, GLsizei height,
                            GLenum intformat, GLenum format, GLenum type) {

  //glDeleteTextures(m_iNumBufers,m_hTexture);
  GL_CHECK(glGenTextures(m_iNumBuffers,m_hTexture));
  for (int i=0; i<m_iNumBuffers; i++) {
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_hTexture[i]));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minfilter));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magfilter));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapmode));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapmode));
    int level=0;
    switch (minfilter) {
      case GL_NEAREST_MIPMAP_NEAREST:
      case GL_LINEAR_MIPMAP_NEAREST:
      case GL_NEAREST_MIPMAP_LINEAR:
      case GL_LINEAR_MIPMAP_LINEAR:
        do {
          GL_CHECK(glTexImage2D(GL_TEXTURE_2D, level, intformat, width, height, 0,
                       format, type, NULL));
          width/=2;
          height/=2;
          if (width==0 && height>0) width=1;
          if (width>0 && height==0) height=1;
          ++level;
        } while (width>=1 && height>=1);
        break;
      default:
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, intformat, width, height, 0,
                            format, type, NULL));
        break;
    }
  }
  return true;
}

/**
 * Check the FBO for consistency.
 */
bool GLRenderTexture::CheckFBO(const char* method) {
  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  switch(status) {
    case GL_FRAMEBUFFER_COMPLETE:
      return true;
    case GL_FRAMEBUFFER_UNSUPPORTED:
      //T_ERROR("%s() - Unsupported Format!",method);
      return false;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      //T_ERROR("%s() - Incomplete attachment",method);
      return false;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      //T_ERROR("%s() - Incomplete missing attachment",method);
      return false;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      //T_ERROR("%s() - Incomplete draw buffer",method);
      return false;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      //T_ERROR("%s() - Incomplete read buffer",method);
      return false;
    default:  return false;
  }
}

void GLRenderTexture::SetViewport() {
    GL_CHECK(glViewport(0, 0, m_iSizeX, m_iSizeY));
}

/**
 * Lock texture for writing. Texture may not be bound any more!
 */
void GLRenderTexture::Write(unsigned int iTargetBuffer, int iBuffer, bool bCheckBuffer) {
  GLenum target = GL_COLOR_ATTACHMENT0 + iTargetBuffer;

  if (!m_glFBO->getGLID()) {
    //T_ERROR("FBO not initialized!");
    return;
  }

  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER,m_glFBO->getGLID()));
  assert(iBuffer>=0);
  assert(iBuffer<m_iNumBuffers);
  m_LastAttachment[iBuffer]=target;
  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D,
                               m_hTexture[iBuffer], 0));
  if (m_hDepthBuffer)
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                 GL_TEXTURE_2D, m_hDepthBuffer, 0));
  if (bCheckBuffer) {
#ifdef _DEBUG
  if (!CheckFBO("Write")) return;
#endif
  }
}

void GLRenderTexture::FinishWrite(int iBuffer) {
  assert(iBuffer>=0);
  assert(iBuffer<m_iNumBuffers);
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER,m_glFBO->getGLID()));
  GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, m_LastAttachment[iBuffer],
                               GL_TEXTURE_2D, 0, 0));
  if (m_hDepthBuffer)
    GL_CHECK(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                 GL_TEXTURE_2D, 0, 0));
  GL_CHECK(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void GLRenderTexture::Read(unsigned int iTargetUnit, int iBuffer) {
  GLenum texunit = GL_TEXTURE0 + iTargetUnit;
#ifdef _DEBUG
  if (m_LastTexUnit[iBuffer]!=0) {
    //T_ERROR("Missing FinishRead()!");
  }
#endif
  assert(iBuffer>=0);
  assert(iBuffer<m_iNumBuffers);
  m_LastTexUnit[iBuffer]=texunit;
  GL_CHECK(glActiveTexture(texunit));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D,m_hTexture[iBuffer]));
}

void GLRenderTexture::ReadDepth(unsigned int iTargetUnit) {
  GLenum texunit = GL_TEXTURE0 + iTargetUnit;
#ifdef _DEBUG
  if (m_LastDepthTextUnit!=0) {
    //T_ERROR("Missing FinishDepthRead()!");
  }
#endif
  m_LastDepthTextUnit=texunit;
  GL_CHECK(glActiveTexture(texunit));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D,m_hDepthBuffer));
}

// Finish reading from the depth texture
void GLRenderTexture::FinishDepthRead() {
  GL_CHECK(glActiveTexture(m_LastDepthTextUnit));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D,0));
  m_LastDepthTextUnit=0;
}

void GLRenderTexture::CopyToFramebuffer(unsigned int iBuffer) {
  GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_glFBO->getGLID()));
  GL_CHECK(glReadBuffer(GL_COLOR_ATTACHMENT0+iBuffer));

  GL_CHECK(glBlitFramebuffer(0,0,m_iSizeX, m_iSizeY,
                    0,0,m_iSizeX, m_iSizeY,
                    GL_COLOR_BUFFER_BIT, GL_NEAREST));

  GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
}

void GLRenderTexture::CopyToFramebuffer(unsigned int x, unsigned int w,
                                 unsigned int y, unsigned int h,
                                 unsigned int tx, unsigned int tw,
                                 unsigned int ty, unsigned int th,
                                 unsigned int iBuffer,
                                 GLenum eFilter) {
  GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_glFBO->getGLID()));
  GL_CHECK(glReadBuffer(GL_COLOR_ATTACHMENT0+iBuffer));

  GL_CHECK(glBlitFramebuffer(x, y, x+w, y+h,
                    tx, ty, tx+tw, ty+th,
                    GL_COLOR_BUFFER_BIT, eFilter));

  GL_CHECK(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
}


void GLRenderTexture::NoDrawBuffer() {
  GL_CHECK(glDrawBuffer(GL_NONE));
}

void GLRenderTexture::OneDrawBuffer() {
  GL_CHECK(glDrawBuffer(GL_COLOR_ATTACHMENT0));
}

void GLRenderTexture::TwoDrawBuffers() {
  GLenum twobuffers[]  = { GL_COLOR_ATTACHMENT0,
                           GL_COLOR_ATTACHMENT1 };
  GL_CHECK(glDrawBuffers(2, twobuffers));
}

void GLRenderTexture::ThreeDrawBuffers() {
  GLenum threebuffers[]  = {GL_COLOR_ATTACHMENT0,
                            GL_COLOR_ATTACHMENT1,
                            GL_COLOR_ATTACHMENT2};
  GL_CHECK(glDrawBuffers(3, threebuffers));
}

void GLRenderTexture::FourDrawBuffers() {
  GLenum fourbuffers[]  = { GL_COLOR_ATTACHMENT0,
                            GL_COLOR_ATTACHMENT1,
                            GL_COLOR_ATTACHMENT2,
                            GL_COLOR_ATTACHMENT3};
  GL_CHECK(glDrawBuffers(4, fourbuffers));
}

// Finish reading from this texture
void GLRenderTexture::FinishRead(int iBuffer) {
  assert(iBuffer>=0);
  assert(iBuffer<m_iNumBuffers);
  GL_CHECK(glActiveTexture(m_LastTexUnit[iBuffer]));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D,0));
  m_LastTexUnit[iBuffer]=0;
}


void GLRenderTexture::ReadBackPixels(int x, int y, int sX, int sY, void* pData, int iBuffer) {
  GL_CHECK(glReadBuffer(GL_COLOR_ATTACHMENT0+iBuffer));
  GL_CHECK(glReadPixels(x, y, sX, sY, m_format, m_type, pData));
}

uint64_t GLRenderTexture::GetCPUSize() const {
    return EstimateCPUSize(m_iSizeX, m_iSizeY, gl_byte_width(m_type) * gl_components(m_format),
                           m_hDepthBuffer!=0, m_iNumBuffers);
}

uint64_t GLRenderTexture::GetGPUSize() const {
    return EstimateGPUSize(m_iSizeX, m_iSizeY, gl_byte_width(m_type) * gl_components(m_format),
                           m_hDepthBuffer!=0, m_iNumBuffers);
}

uint64_t GLRenderTexture::EstimateCPUSize(GLsizei width, GLsizei height,
                                  size_t iSizePerElement,
                                  bool bHaveDepth, int iNumBuffers) {
    return iNumBuffers*width*height*iSizePerElement +
           ((bHaveDepth) ? width*height*4 : 0);
}
uint64_t GLRenderTexture::EstimateGPUSize(GLsizei width, GLsizei height,
                              size_t iSizePerElement,
                              bool bHaveDepth, int iNumBuffers) {
  return EstimateCPUSize(width, height, iSizePerElement, bHaveDepth,
                          iNumBuffers);
}

void GLRenderTexture::SetData(const Core::Math::Vec2ui& offset, const Core::Math::Vec2ui& size, const void *pixels, int iBuffer, bool bRestoreBinding) {
  GL_CHECK(glPixelStorei(GL_PACK_ALIGNMENT ,1));
  GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT ,1));

  GLint prevTex=0;
  if (bRestoreBinding) glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTex);

  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_hTexture[iBuffer]));
  GL_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0,
    offset.x, offset.y,
    size.x, size.y,
    m_format, m_type, (GLvoid*)pixels));

  if (bRestoreBinding && GLuint(prevTex) != m_hTexture[iBuffer]) glBindTexture(GL_TEXTURE_2D, prevTex);
}

void GLRenderTexture::SetData(const void *pixels, int iBuffer, bool bRestoreBinding) {
  GL_CHECK(glPixelStorei(GL_PACK_ALIGNMENT ,1));
  GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT ,1));

  GLint prevTex=0;
  if (bRestoreBinding) GL_CHECK(glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTex));

  GL_CHECK(glBindTexture(GL_TEXTURE_2D, m_hTexture[iBuffer]));
  GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, m_intformat, m_iSizeX, m_iSizeY,
    0, m_format, m_type, (GLvoid*)pixels));

  if (bRestoreBinding && GLuint(prevTex) != m_hTexture[iBuffer]) GL_CHECK(glBindTexture(GL_TEXTURE_2D, prevTex));
}

void GLRenderTexture::ClearPixels(float r, float g, float b, float a){
    GL_CHECK(glClearColor(r, g, b, a));
    GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
}

