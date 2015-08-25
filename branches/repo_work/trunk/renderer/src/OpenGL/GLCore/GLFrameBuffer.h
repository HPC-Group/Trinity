//
//  GLFrameBuffer.h
//  TNG
//
//  Created by Andre Waschk on 09/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef __TNG__GLFrameBuffer__
#define __TNG__GLFrameBuffer__

#include <core/Math/Vectors.h>
#include <renderer/OpenGL/OpenGLDefines.h>

namespace Tuvok{
  namespace Renderer{
    namespace OpenGL{
      namespace GLCore{

      class GLFrameBuffer{
          public:
              //! Constructor
              GLFrameBuffer();
              //! Deconstrutor
              ~GLFrameBuffer(void);

              virtual void Initialize(Core::Math::Vec2ui resolution, uint32_t format, int buffercount);

              virtual void SetViewport();

              //! Sets the framebuffer to be the current render target
              virtual void Write();

              //! Clears the framebuffer with the given color
              virtual void ClearFramebuffer(Core::Math::Vec4f clearColor);

              //! returns a handle to the framebuffer texture
              virtual const uint32_t& GetFramebufferTexture(const int i);


          protected:

          private:

              GLuint                  m_hFBOhandle;        //!< handle to the framebuffer
              GLuint                  m_hDepthhandle;       //!< handle to the depthbuffer

              uint32_t                m_iFrameBufferCount;         //!< number of rendertargets
              Core::Math::Vec2ui      m_vFrameBufferResolution;    //!< resolution of the rendertarget

              GLenum                  *m_DrawBuffers;             //!< attatchments to be drawn
              std::vector<uint32_t>   m_vFrameBufferTextureHandles;//!< handles to the single textures
          };
      };
    };
  };
};

#endif /* defined(__TNG__GLFrameBuffer__) */
