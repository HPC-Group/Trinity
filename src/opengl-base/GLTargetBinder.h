#pragma once

#include <cstdlib>
#include <vector>
#include <memory>
#include "OpenGLincludes.h"

class GLRenderTexture;
typedef std::shared_ptr<GLRenderTexture> GLRenderTexturePtr;

		
class GLBufferID {
public:
 GLBufferID(GLRenderTexturePtr _pBuffer, uint32_t _iSubBuffer = 0) :
    pBuffer(_pBuffer),
    iSubBuffer(_iSubBuffer)
  { }

    GLRenderTexturePtr pBuffer;
    uint32_t iSubBuffer;
};

class GLTargetBinder {
  public:
    GLTargetBinder();
    virtual ~GLTargetBinder();

    virtual void Bind(const std::vector<GLBufferID>& vpFBOs);
    virtual void Bind(GLRenderTexturePtr pFBO0, GLRenderTexturePtr pFBO1 = NULL, GLRenderTexturePtr pFBO2 = NULL, GLRenderTexturePtr pFBO3 = NULL);
    virtual void Bind(GLRenderTexturePtr pFBO0, int iSubBuffer0,
        GLRenderTexturePtr pFBO1 = NULL, int iSubBuffer1 = 0,
        GLRenderTexturePtr pFBO2 = NULL, int iSubBuffer2 = 0,
        GLRenderTexturePtr pFBO3 = NULL, int iSubBuffer3 = 0);

    virtual void Unbind();

  protected:
    std::vector<GLBufferID>  m_vpBoundFBOs;

    void UnbindInternal();
};
