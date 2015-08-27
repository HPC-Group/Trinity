//
//  GLVolume.h
//  TNG
//
//  Created by Andre Waschk on 22/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef __TNG__GLVolume__
#define __TNG__GLVolume__
#include <renderer/OpenGL/OpenGLDefines.h>

namespace Tuvok{
  namespace Renderer{
    namespace OpenGL{
      namespace GLVolume{

        class GLVolume{
        public:
          GLVolume(uint32_t iSizeX, uint32_t iSizeY, uint32_t iSizeZ,
                   GLint internalformat, GLenum format, GLenum type,
                   const GLvoid *pixels = 0,
                   GLint iMagFilter = GL_NEAREST,
                   GLint iMinFilter = GL_NEAREST,
                   GLint wrapX = GL_CLAMP_TO_EDGE,
                   GLint wrapY = GL_CLAMP_TO_EDGE,
                   GLint wrapZ = GL_CLAMP_TO_EDGE);
          GLVolume();
          virtual ~GLVolume();
          virtual void SetData(const void *voxels) = 0;
          virtual void SetFilter(GLint iMagFilter = GL_NEAREST,
                                 GLint iMinFilter = GL_NEAREST);

        protected:
          GLint  m_iMagFilter;
          GLint  m_iMinFilter;
        };
      }
    }
  }
}

#endif /* defined(__TNG__GLVolume__) */
