#pragma once
#include <memory>

#include "common/IRenderer.h"

#include <memory>

#include "opengl-base/GLCore/GLProgram.h"
#include "opengl-base/GLCore/GLFrameBufferObject.h"
#include "opengl-base/OpenGlHeadlessContext.h"
#include "opengl-base/GLCore/GLVolumeBox.h"
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
    bool LoadShader();
    void LoadGeometry();
    void LoadFrameBuffer();

private:
    float       m_isoValue;
    uint32_t    m_width;
    uint32_t    m_height;

    std::vector<Core::Math::Vec4ui8> m_bufferData;
    std::unique_ptr<OpenGL::GLTargetBinder>         m_targetBinder;
    std::unique_ptr<OpenGL::GLCore::GLProgram>      m_sampleShader;
    std::shared_ptr<OpenGL::GLCore::GLFBOTex>       m_sampleFrameBuffer;
    std::unique_ptr<OpenGL::GLCore::GLVolumeBox>    m_sampleBox;
    std::shared_ptr<OpenGlHeadlessContext>          m_context;
};
}