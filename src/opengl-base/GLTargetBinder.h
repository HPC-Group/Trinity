#pragma once

#include <cstdlib>
#include <vector>
#include <memory>
#include "OpenGLincludes.h"

class GLFBOTex;
typedef std::shared_ptr<GLFBOTex> GLFBOTexPtr;

		
class GLBufferID {
public:
 GLBufferID(GLFBOTexPtr _pBuffer, uint32_t _iSubBuffer = 0) :
    pBuffer(_pBuffer),
    iSubBuffer(_iSubBuffer)
  { }

    GLFBOTexPtr pBuffer;
    uint32_t iSubBuffer;
};

class GLTargetBinder {
  public:
    GLTargetBinder();
    virtual ~GLTargetBinder();

    virtual void Bind(const std::vector<GLBufferID>& vpFBOs);
    virtual void Bind(GLFBOTexPtr pFBO0, GLFBOTexPtr pFBO1 = NULL, GLFBOTexPtr pFBO2 = NULL, GLFBOTexPtr pFBO3 = NULL);
    virtual void Bind(GLFBOTexPtr pFBO0, int iSubBuffer0,
        GLFBOTexPtr pFBO1 = NULL, int iSubBuffer1 = 0,
        GLFBOTexPtr pFBO2 = NULL, int iSubBuffer2 = 0,
        GLFBOTexPtr pFBO3 = NULL, int iSubBuffer3 = 0);

    virtual void Unbind();

  protected:
    std::vector<GLBufferID>  m_vpBoundFBOs;

    void UnbindInternal();
};
