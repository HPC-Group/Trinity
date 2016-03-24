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

  protected:
    virtual void paintInternal(PaintLevel paintlevel) override;

  private:
    bool LoadShaders();
    void LoadGeometry();
    void LoadFrameBuffers();
    void LoadVolumeData();
    void LoadTransferFunction();

    std::vector<Core::Math::Vec4ui8>  m_bufferData;
    
    std::unique_ptr<GLTexture1D>      m_texTransferFunc;
    std::unique_ptr<GLTexture3D>      m_texVolume;
    std::unique_ptr<GLTargetBinder>   m_targetBinder;
    std::unique_ptr<GLProgram>        m_backfaceShader;
    std::unique_ptr<GLProgram>        m_raycastShader;
    std::shared_ptr<GLFBOTex>         m_backfaceBuffer;
    std::shared_ptr<GLFBOTex>         m_resultBuffer;
    std::unique_ptr<GLVolumeBox>      m_bbBox;
    std::shared_ptr<OpenGlHeadlessContext> m_context;
    
  };
}
