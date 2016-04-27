#include "GLTargetBinder.h"
#include "opengl-base/GLFrameBufferObject.h"
#include "mocca/log/LogManager.h"

GLTargetBinder::GLTargetBinder()
{
}

GLTargetBinder::~GLTargetBinder() {
  Unbind();
}


void GLTargetBinder::Bind(const std::vector<GLBufferID>& vpFBOs) {
  // now for the fun stuff with OpenGL binding
  
  UnbindInternal();
  
  switch (vpFBOs.size()) {
    case 1  : vpFBOs[0].pBuffer->Write(0,vpFBOs[0].iSubBuffer);
      GLRenderTexture::OneDrawBuffer();
      break;
    case 2  : vpFBOs[0].pBuffer->Write(0,vpFBOs[0].iSubBuffer);
      vpFBOs[1].pBuffer->Write(1,vpFBOs[1].iSubBuffer);
      GLRenderTexture::TwoDrawBuffers();
      break;
    case 3  : vpFBOs[0].pBuffer->Write(0,vpFBOs[0].iSubBuffer);
      vpFBOs[1].pBuffer->Write(1,vpFBOs[1].iSubBuffer);
      vpFBOs[2].pBuffer->Write(2,vpFBOs[2].iSubBuffer);
      GLRenderTexture::ThreeDrawBuffers();
      break;
    case 4  : vpFBOs[0].pBuffer->Write(0,vpFBOs[0].iSubBuffer);
      vpFBOs[1].pBuffer->Write(1,vpFBOs[1].iSubBuffer);
      vpFBOs[2].pBuffer->Write(2,vpFBOs[2].iSubBuffer);
      vpFBOs[3].pBuffer->Write(3,vpFBOs[3].iSubBuffer);
      GLRenderTexture::FourDrawBuffers();
      break;
    default :
      LERROR("Invalid number of FBOs " << static_cast<unsigned>(vpFBOs.size()));
      return;
  }
  
  m_vpBoundFBOs = vpFBOs;
}

void GLTargetBinder::Bind(GLRenderTexturePtr pFBO0, int iSubBuffer0,
                          GLRenderTexturePtr pFBO1, int iSubBuffer1,
                          GLRenderTexturePtr pFBO2, int iSubBuffer2,
                          GLRenderTexturePtr pFBO3, int iSubBuffer3) {
  std::vector<GLBufferID> vpFBOs;
  vpFBOs.push_back(GLBufferID(pFBO0, iSubBuffer0));
  if (pFBO1) vpFBOs.push_back(GLBufferID(pFBO1, iSubBuffer1));
  if (pFBO2) vpFBOs.push_back(GLBufferID(pFBO2, iSubBuffer2));
  if (pFBO3) vpFBOs.push_back(GLBufferID(pFBO3, iSubBuffer3));
  
  Bind(vpFBOs);
}

void GLTargetBinder::Bind(GLRenderTexturePtr pFBO0, GLRenderTexturePtr pFBO1, GLRenderTexturePtr pFBO2, GLRenderTexturePtr pFBO3) {
  
  std::vector<GLBufferID> vpFBOs;
  vpFBOs.push_back(GLBufferID(pFBO0));
  if (pFBO1) vpFBOs.push_back(GLBufferID(pFBO1));
  if (pFBO2) vpFBOs.push_back(GLBufferID(pFBO2));
  if (pFBO3) vpFBOs.push_back(GLBufferID(pFBO3));
  
  Bind(vpFBOs);
}

void GLTargetBinder::Unbind() {
  UnbindInternal();
}

void GLTargetBinder::UnbindInternal() {
  
  if (m_vpBoundFBOs.size() == 1) {
    m_vpBoundFBOs[0].pBuffer->FinishWrite(m_vpBoundFBOs[0].iSubBuffer);
  } else
    if (m_vpBoundFBOs.size() == 2) {
      GLRenderTexture::NoDrawBuffer();
      m_vpBoundFBOs[1].pBuffer->FinishWrite(m_vpBoundFBOs[1].iSubBuffer);
      m_vpBoundFBOs[0].pBuffer->FinishWrite(m_vpBoundFBOs[0].iSubBuffer);
    } else
      if (m_vpBoundFBOs.size() == 3) {
        GLRenderTexture::NoDrawBuffer();
        m_vpBoundFBOs[2].pBuffer->FinishWrite(m_vpBoundFBOs[2].iSubBuffer);
        m_vpBoundFBOs[1].pBuffer->FinishWrite(m_vpBoundFBOs[1].iSubBuffer);
        m_vpBoundFBOs[0].pBuffer->FinishWrite(m_vpBoundFBOs[0].iSubBuffer);
      } else
        if (m_vpBoundFBOs.size() == 4) {
          GLRenderTexture::NoDrawBuffer();
          m_vpBoundFBOs[3].pBuffer->FinishWrite(m_vpBoundFBOs[3].iSubBuffer);
          m_vpBoundFBOs[2].pBuffer->FinishWrite(m_vpBoundFBOs[2].iSubBuffer);
          m_vpBoundFBOs[1].pBuffer->FinishWrite(m_vpBoundFBOs[1].iSubBuffer);
          m_vpBoundFBOs[0].pBuffer->FinishWrite(m_vpBoundFBOs[0].iSubBuffer);
        }
  
  m_vpBoundFBOs.clear();
}
