#include "GridLeaper.h"
#include "mocca/log/LogManager.h"


#include <opengl-base/OpenGLError.h>

#include <vector>
#include "silverbullet/math/Vectors.h"

using namespace trinity;
using namespace OpenGL::GLCore;
using namespace OpenGL;
using namespace std;


GridLeaper::GridLeaper(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession)
    : IRenderer(stream, std::move(ioSession))
    , _context(nullptr) {}

void GridLeaper::setIsoValue(const float isoValue) {
    LINFO("(p) iso value of gridleaper set to " + std::to_string(isoValue));
    m_isoValue = isoValue;
    if (_context != nullptr) {
        paint();
    }
}

void GridLeaper::initContext() {
    _context = std::unique_ptr<OpenGlHeadlessContext>(new OpenGlHeadlessContext());

    if (!_context->isValid()) {
        LERROR("(p) can't create opengl context");
        // std::cout << "can't create context" << std::endl;
    }

    LINFO("(p) grid leaper created. OpenGL Version " << _context->getVersion());

    _width = 512;
    _height = 512;

    LoadFrameBuffer();
    LoadShader();
    LoadGeometry();

    _targetBinder = std::unique_ptr<GLTargetBinder>(new GLTargetBinder());
}

bool GridLeaper::LoadShader() {
    vector<string> fs, vs;
    vs.push_back("SampleVertex.glsl");
    fs.push_back("SampleFragment.glsl");
    ShaderDescriptor sd(vs, fs);
    if (!LoadAndCheckShaders(_sampleShader, sd))
        return false;
}

void GridLeaper::LoadGeometry() {
    _sampleBox = std::unique_ptr<GLVolumeBox>(new GLVolumeBox());
}

void GridLeaper::LoadFrameBuffer() {
    _sampleFrameBuffer =
        std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, _width, _height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, true, 1);
}

bool GridLeaper::LoadAndCheckShaders(std::shared_ptr<GLProgram>& programPtr, ShaderDescriptor& sd) {
    programPtr = std::make_shared<GLProgram>();
    programPtr->Load(sd);

    if (!programPtr->IsValid()) {
        cout << "programm not valid" << endl;
        return false;
    }
    return true;
}

void GridLeaper::paint() {
    _context->makeCurrent();

    _targetBinder->Bind(_sampleFrameBuffer);
    GL_CHECK(glViewport(0, 0, _width, _height));


    _sampleFrameBuffer->ClearPixels(0.0f, 0.0f, 0.0f, 0.0f);

    Core::Math::Mat4f projection;
    Core::Math::Mat4f view;
    Core::Math::Mat4f world;
    Core::Math::Mat4f rotx, roty;

#ifndef DETECTED_OS_APPLE
    projection.Perspective(45.0f, (float)_width / (float)_height, 0.01f, 1000.0f);
    view.BuildLookAt(Core::Math::Vec3f(0, 0, 10), Core::Math::Vec3f(0, 0, 0), Core::Math::Vec3f(0, 1, 0));
#endif
    rotx.RotationX(m_isoValue);
    roty.RotationY(m_isoValue * 1.14f);
    world = rotx * roty;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    _sampleShader->Enable();
    _sampleShader->Set("projectionMatrix", projection);
    _sampleShader->Set("viewMatrix", view);
    _sampleShader->Set("worldMatrix", world);

    _sampleBox->paint();

    _sampleShader->Disable();


    // read buffer
    std::vector<Core::Math::Vec4ui8> buffer;
    buffer.resize(_width * _height);

    _sampleFrameBuffer->ReadBackPixels(0, 0, _width, _height, &buffer[0]);

    auto f1 = trinity::Frame::createFromRaw(&buffer[0], buffer.size()*sizeof(Core::Math::Vec4ui8));
    getVisStream()->put(std::move(f1));
}
