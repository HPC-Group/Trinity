//
//  GLProgramm.h
//  TNG
//
//  Created by Andre Waschk on 10/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef __TNG__GLProgram__
#define __TNG__GLProgram__

#include <renderer/OpenGL/OpenGLDefines.h>
#include <core/Math/Vectors.h>
#include <string>
#include <map>


namespace Tuvok{
  namespace Renderer{

    class ShaderDescriptor;

    namespace OpenGL{
        namespace GLCore{

          class GLTexture;

          typedef std::map<std::string, int> texMap;

          class GLProgram{
          public:
              GLProgram();
              virtual ~GLProgram();


              virtual bool Load(ShaderDescriptor& sd);

              /// Enables this shader for rendering.
              void Enable(void);
              /// Disables all shaders for rendering (use fixed function pipeline again)
              void Disable(void);

              /// returns true if this program is valid
              bool IsValid() const;

              void Set(const char *name, const float value);
              void Set(const char *name, const Core::Math::Vec2f value);
              void Set(const char *name, const Core::Math::Vec3f value);
              void Set(const char *name, const Core::Math::Vec4f value);

              void Set(const char *name, const int32_t value);
              void Set(const char *name, const Core::Math::Vec2i value);
              void Set(const char *name, const Core::Math::Vec3i value);
              void Set(const char *name, const Core::Math::Vec4i value);

              void Set(const char *name, const Core::Math::Mat4f& value);

              void SetTexture3D(const char *name, const GLuint value, const uint8_t id);

              /// Sets a texture parameter.
              void SetTexture(const std::string& name, const GLTexture& pTexture);
              void SetTexture(const std::string& name, const std::shared_ptr<GLTexture> pTexture);
			  void SetTexture(const std::string& name, const GLuint value);
			  void SetTexture1D(const std::string& name, const GLuint value, const GLuint shaderLocation);
			  void SetTexture2D(const std::string& name, const GLuint value, const GLuint shaderLocation);
              /// Force a specific name/texID binding
              void ConnectTextureID(const std::string& name, const int iUnit);

              GLuint GetLocation(const char *name) const;

			  uint64_t GetGPUSize() const {
				  return 1;
			  }

			  uint64_t GetCPUSize() const {
				  return 1;
			  }

          private:
              bool                Initialize(void); // to old ??
              bool                WriteInfoLog(const char*, GLuint, bool);
              bool                CheckGLError(const char *pcError=NULL,
                                   const char *pcAdditional=NULL) const;
              void                CheckSamplerType(const char *name) const;

              bool                m_bInitialized;

              GLuint              m_ShaderProgramm;
              GLuint              m_currentVariableLocation;

              GLuint              m_VertexShaderHandle;
              GLuint              m_FragmentShaderHandle;

              texMap              m_mBindings;

              bool CompileShader(GLuint& handle, const char* source, GLenum type);
              bool CheckProgramStatus();
          };

        };
    };
  };
};

#endif /* defined(__TNG__GLProgramm__) */
