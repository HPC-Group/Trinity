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
namespace processing {


class GridLeaper : public common::IRenderer {

public:
    GridLeaper(std::shared_ptr<common::VisStream> stream, std::unique_ptr<common::IIO> ioSession);

    virtual void setIsoValue(const float);
    virtual void initContext();
    virtual void paint();
private:
    bool LoadShader();
    void LoadGeometry();
    void LoadFrameBuffer();

    bool LoadAndCheckShaders(std::shared_ptr<OpenGL::GLCore::GLProgram>& programPtr, ShaderDescriptor& sd);
private:
    float m_isoValue;
    uint32_t    _width;
    uint32_t    _height;

    std::unique_ptr<OpenGlHeadlessContext>          _context;
    std::shared_ptr<OpenGL::GLCore::GLProgram>      _sampleShader;
    std::shared_ptr<OpenGL::GLCore::GLFBOTex>       _sampleFrameBuffer;
    std::unique_ptr<OpenGL::GLCore::GLVolumeBox>    _sampleBox;
    std::unique_ptr<OpenGL::GLTargetBinder>         _targetBinder;
};
}
}
