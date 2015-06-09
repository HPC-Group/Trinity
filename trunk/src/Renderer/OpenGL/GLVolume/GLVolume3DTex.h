//
//  GLVolume3DTex.h
//  TNG
//
//  Created by Andre Waschk on 22/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef __TNG__GLVolume3DTex__
#define __TNG__GLVolume3DTex__
#include "GLVolume.h"
#include <memory>
#include <string>

namespace Tuvok{
  namespace Renderer{
    namespace OpenGL{
      namespace GLCore{
        class GLTexture3D;
        typedef std::shared_ptr<GLTexture3D> GLTexture3DPtr;
        class GLProgram;
        typedef std::shared_ptr<GLProgram> GLProgramPtr;
      }
      
      namespace GLVolume{
        
        
        /// Controls 3D volume data as a texture.
        class GLVolume3DTex : public GLVolume {
        public:
          GLVolume3DTex(uint32_t iSizeX, uint32_t iSizeY, uint32_t iSizeZ,
                        GLint internalformat, GLenum format, GLenum type,
                        const GLvoid *voxels = 0,
                        GLint iMagFilter = GL_NEAREST,
                        GLint iMinFilter = GL_NEAREST,
                        GLint wrapX = GL_CLAMP_TO_EDGE,
                        GLint wrapY = GL_CLAMP_TO_EDGE,
                        GLint wrapZ = GL_CLAMP_TO_EDGE);
          GLVolume3DTex();
          virtual ~GLVolume3DTex();
          
          virtual void Bind(uint32_t iUnit=0);
          virtual void Bind(GLCore::GLProgramPtr glslProgram, const std::string& name);
          virtual void SetData(const void *voxels);
          virtual std::shared_ptr<void> GetData();
          
          virtual uint64_t GetCPUSize() const;
          virtual uint64_t GetGPUSize() const;
          
          virtual void SetFilter(GLint iMagFilter = GL_NEAREST,
                                 GLint iMinFilter = GL_NEAREST);

		  GLuint GetGLID();
        private:
          GLCore::GLTexture3DPtr m_pTexture;
          void FreeGLResources();
        };
      }
    }
  }
}

#endif /* defined(__TNG__GLVolume3DTex__) */
