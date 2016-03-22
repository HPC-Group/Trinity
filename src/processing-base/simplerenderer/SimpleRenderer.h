#pragma once
#include <memory>

#include "common/IRenderer.h"

#include <memory>

#include "opengl-base/GLProgram.h"
#include "opengl-base/GLTexture1D.h"
#include "opengl-base/GLTexture3D.h"
#include "opengl-base/GLFrameBufferObject.h"
#include "opengl-base/OpenGlHeadlessContext.h"
#include "opengl-base/GLVolumeBox.h"
#include "opengl-base/ShaderDescriptor.h"
#include "opengl-base/GLTargetBinder.h"

namespace trinity {
  
  class SimpleRenderer : public IRenderer {
    
  public:
    SimpleRenderer(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession);
    ~SimpleRenderer();
    void setIsoValue(const float) override;
    void initContext() override;
    void deleteContext() override;
    void paint();
    void zoomCamera(float f) override;
  private:
    bool LoadShaders();
    void LoadGeometry();
    void LoadFrameBuffers();
    void LoadVolumeData();
    void LoadTransferFunction();
    
  private:
    float       m_isoValue;
    uint32_t    m_width;
    uint32_t    m_height;

    std::vector<Core::Math::Vec4ui8>              m_bufferData;
    
    std::unique_ptr<GLTexture1D>  m_texTransferFunc;
    std::unique_ptr<GLTexture3D>  m_texVolume;
    std::unique_ptr<GLTargetBinder>       m_targetBinder;
    std::unique_ptr<GLProgram>    m_backfaceShader;
    std::unique_ptr<GLProgram>    m_raycastShader;
    std::shared_ptr<GLFBOTex>     m_backfaceBuffer;
    std::shared_ptr<GLFBOTex>     m_resultBuffer;
    std::unique_ptr<GLVolumeBox>  m_bbBox;
    std::shared_ptr<OpenGlHeadlessContext>        m_context;
    
  };
}
