#include "SimpleRenderer.h"
#include "mocca/log/LogManager.h"


#include <opengl-base/OpenGLError.h>

#include <vector>
#include "silverbullet/math/Vectors.h"

using namespace trinity;
using namespace OpenGL::GLCore;
using namespace Core::Math;
using namespace OpenGL;
using namespace std;


SimpleRenderer::SimpleRenderer(std::shared_ptr<VisStream> stream,
                       std::unique_ptr<IIO> ioSession) :
IRenderer(stream, std::move(ioSession)),
m_texTransferFunc(nullptr),
m_texVolume(nullptr),
m_targetBinder(nullptr),
m_sampleShader(nullptr),
m_sampleFrameBuffer(nullptr),
m_sampleBox(nullptr),
m_context(nullptr)
{
}

void SimpleRenderer::deleteContext() {
  m_texTransferFunc = nullptr;
  m_texVolume = nullptr;
	m_targetBinder = nullptr;
	m_sampleShader = nullptr;
	m_sampleFrameBuffer = nullptr;
	m_sampleBox = nullptr;
	m_context = nullptr;
}

SimpleRenderer::~SimpleRenderer() {
  LINFO("(p) destroying a gridleaper");
}

void SimpleRenderer::setIsoValue(const float isoValue) {
  m_isoValue = isoValue;
  paint();
}

void SimpleRenderer::zoomCamera(float f) {
  LINFO("(p) cam zoom of gridleaper set to " + std::to_string(f));
}

void SimpleRenderer::initContext() {
	std::thread::id threadId = std::this_thread::get_id();
	LINFO("gridleaper performs cotext init from thread " << threadId);
  m_context = mocca::make_unique<OpenGlHeadlessContext>();

  if (!m_context->isValid()) {
    LERROR("(p) can't create opengl context");
    m_context = nullptr;
    return;
  }

  m_width = m_visStream->getStreamingParams().getResX();
  m_height = m_visStream->getStreamingParams().getResY();
  m_bufferData.resize(m_width * m_height);

  
  LINFO("(p) grid leaper: resolution: " +
        std::to_string(m_width) + " x " + std::to_string(m_height));

  LoadFrameBuffer();
  LoadShader();
  LoadGeometry();
  LoadVolumeData();
  LoadTransferFunction();

  m_targetBinder = mocca::make_unique<GLTargetBinder>();
  LINFO("(p) grid leaper created. OpenGL Version " << m_context->getVersion());
}

bool SimpleRenderer::LoadShader() {
  vector<string> fs, vs;
  vs.push_back("SampleVertex.glsl");
  fs.push_back("SampleFragment.glsl");
  ShaderDescriptor sd(vs, fs);
  
  m_sampleShader = mocca::make_unique<GLProgram>();
  m_sampleShader->Load(sd);
  
  if (!m_sampleShader->IsValid()) {
    LERROR("(p) invalid shader program");
    m_sampleShader = nullptr;
    return false;
  }
  
  return true;
}

void SimpleRenderer::LoadVolumeData() {
  LINFO("(p) creating volume");
  
  // this simple renderer can only handle scalar 8bit uints
  // so check if the volume satisfies these conditions
  if (m_io->getComponentCount(0) != 1 ||
      m_io->getType(0) != IIO::ValueType::T_UINT8) {
    LERROR("(p) invalid volume format");
    return;
  }
  
  // this simple renderer cannot handle bricks, so we render the
  // "best" LoD that consists of a single brick
  uint64_t singleBrickLoD = m_io->getLargestSingleBrickLOD(0);

  std::vector<uint8_t> volume;
  BrickKey brickKey(0, 0, singleBrickLoD, 0);
  m_io->getBrick(brickKey, volume);
  
  Core::Math::Vec3ui brickSize = m_io->getBrickVoxelCounts(brickKey);
  
  if (volume.size() != brickSize.volume()) {
    LERROR("invalid volume data vector. size should be " <<
           brickSize.volume() << " but is " << volume.size());
    return;
  }

  m_texVolume = mocca::make_unique<GLTexture3D>(brickSize.x,
                                                brickSize.y,
                                                brickSize.z,
                                                GL_RED,
                                                GL_RED,
                                                GL_UNSIGNED_BYTE,
                                                volume.data(),
                                                GL_LINEAR,
                                                GL_LINEAR);
  
  LINFO("(p) volume created");
}

void SimpleRenderer::LoadTransferFunction() {
  LINFO("(p) creating transfer function");
  
  std::vector<uint8_t> linearRamp(4*256);  // RGBA * 256
  for (size_t i = 0;i<256;++i) {
    linearRamp[i*4+0] = uint8_t(i);
    linearRamp[i*4+1] = uint8_t(i);
    linearRamp[i*4+2] = uint8_t(i);
    linearRamp[i*4+3] = uint8_t(i);
  }
  m_texTransferFunc = mocca::make_unique<GLTexture1D>(256,
                                                      GL_RGBA,
                                                      GL_RGBA,
                                                      GL_UNSIGNED_BYTE,
                                                      linearRamp.data());
  LINFO("(p) transfer function created");
}

void SimpleRenderer::LoadGeometry() {
  m_sampleBox = mocca::make_unique<GLVolumeBox>();
}

void SimpleRenderer::LoadFrameBuffer() {
  m_sampleFrameBuffer = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                                   GL_CLAMP_TO_EDGE,
                                                   m_width, m_height,
                                                   GL_RGBA, GL_RGBA,
                                                   GL_UNSIGNED_BYTE, true, 1);
}

void SimpleRenderer::paint() {
  if (!m_context || !m_sampleShader) return;
  
  m_context->makeCurrent();

  m_targetBinder->Bind(m_sampleFrameBuffer);
  GL_CHECK(glViewport(0, 0, m_width, m_height));


  m_sampleFrameBuffer->ClearPixels(0.0f, 0.0f, 0.0f, 0.0f);

  Mat4f projection;
  Mat4f view;
  Mat4f world;
  Mat4f rotx, roty;

  projection.Perspective(45.0f, (float)m_width / (float)m_height,
                         0.01f, 1000.0f);
  view.BuildLookAt(Vec3f(0, 0, 10), Vec3f(0, 0, 0), Vec3f(0, 1, 0));

  rotx.RotationX(m_isoValue);
  roty.RotationY(m_isoValue * 1.14f);
  world = rotx * roty;

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);

  m_sampleShader->Enable();
  m_sampleShader->Set("projectionMatrix", projection);
  m_sampleShader->Set("viewMatrix", view);
  m_sampleShader->Set("worldMatrix", world);

  m_sampleBox->paint();

  m_sampleShader->Disable();

  
  m_sampleFrameBuffer->ReadBackPixels(0, 0, m_width, m_height,
                                      m_bufferData.data());

  m_targetBinder->Unbind();
  
  auto f1 = Frame::createFromRaw(m_bufferData.data(),
                                 m_bufferData.size()*sizeof(Vec4ui8));
  getVisStream()->put(std::move(f1));
}
