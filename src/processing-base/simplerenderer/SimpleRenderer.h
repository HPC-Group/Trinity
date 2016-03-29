#pragma once
#include <memory>

#include "../AbstractRenderer.h"

#include "opengl-base/GLProgram.h"
#include "opengl-base/GLTexture1D.h"
#include "opengl-base/GLTexture3D.h"
#include "opengl-base/GLFrameBufferObject.h"
#include "opengl-base/OpenGlHeadlessContext.h"
#include "opengl-base/GLVolumeBox.h"
#include "opengl-base/ShaderDescriptor.h"
#include "opengl-base/GLTargetBinder.h"

namespace trinity {

  class SimpleRenderer : public AbstractRenderer {
    
  public:
    SimpleRenderer(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession);
    ~SimpleRenderer();

    virtual void initContext() override;
    virtual void deleteContext() override;
    virtual void resizeFramebuffer() override;
    virtual bool isIdle() override;
    virtual bool proceedRendering() override;
    

  protected:
    virtual void paintInternal(PaintLevel paintlevel) override;

  private:
    
    bool loadShaders();
    void loadGeometry();
    void loadVolumeData();
    void loadTransferFunction();
    void initFrameBuffers();

    std::vector<std::array<uint8_t, 4>>  m_bufferData;
    
    std::unique_ptr<GLTexture1D>      m_texTransferFunc;
    std::unique_ptr<GLTexture3D>      m_texVolume;
    std::unique_ptr<GLTargetBinder>   m_targetBinder;
    std::unique_ptr<GLProgram>        m_backfaceShader;
    std::unique_ptr<GLProgram>        m_raycastShader;
    std::shared_ptr<GLFBOTex>         m_backfaceBuffer;
    std::shared_ptr<GLFBOTex>         m_resultBuffer;
    std::unique_ptr<GLVolumeBox>      m_bbBox;
    std::shared_ptr<OpenGlHeadlessContext> m_context;
    
    Core::Math::Mat4f m_domainTransform;
    float             m_transferFuncScaleValue;
    
  };
}
