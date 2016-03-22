#pragma once
#include <memory>

#include "common/IRenderer.h"

#include <memory>

#include "opengl-base/GLProgram.h"
#include "opengl-base/GLFrameBufferObject.h"
#include "opengl-base/OpenGlHeadlessContext.h"
#include "opengl-base/GLVolumeBox.h"
#include "opengl-base/ShaderDescriptor.h"
#include "opengl-base/GLTargetBinder.h"

namespace trinity {

class GridLeaper : public IRenderer {

public:
    GridLeaper(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession);
    ~GridLeaper();
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
    std::unique_ptr<GLTargetBinder>         m_targetBinder;
    std::unique_ptr<GLProgram>      m_sampleShader;
    std::shared_ptr<GLFBOTex>       m_sampleFrameBuffer;
    std::unique_ptr<GLVolumeBox>    m_sampleBox;
    std::shared_ptr<OpenGlHeadlessContext>          m_context;
};
}
