#include "SimpleRenderer.h"

#include "common/MemBlockPool.h"
#include "common/VisStream.h"
#include "opengl-base/OpenGLError.h"
#include "silverbullet/math/Vectors.h"
#include "silverbullet/math/MathTools.h"

#include "mocca/log/LogManager.h"

#include <vector>
#include <thread>

using namespace trinity;
using namespace Core::Math;
using namespace std;


SimpleRenderer::SimpleRenderer(std::shared_ptr<VisStream> stream, std::unique_ptr<IIO> ioSession)
: AbstractRenderer(stream, std::move(ioSession))
, m_texTransferFunc(nullptr)
, m_texVolume(nullptr)
, m_targetBinder(nullptr)
, m_backfaceShader(nullptr)
, m_raycastShader(nullptr)
, m_backfaceBuffer(nullptr)
, m_resultBuffer(nullptr)
, m_bbBox(nullptr)
, m_context(nullptr) {}

void SimpleRenderer::deleteContext() {
  m_texTransferFunc = nullptr;
  m_texVolume = nullptr;
  m_targetBinder = nullptr;
  m_backfaceShader = nullptr;
  m_raycastShader = nullptr;
  m_backfaceBuffer = nullptr;
  m_resultBuffer = nullptr;
  m_bbBox = nullptr;
  m_context = nullptr;
}

SimpleRenderer::~SimpleRenderer() {
  LINFO("(p) destroying a SimpleRenderer");
}

void SimpleRenderer::initContext() {
  std::thread::id threadId = std::this_thread::get_id();
  LINFO("(p) performs context init from thread " << threadId);
  m_context = mocca::make_unique<OpenGlHeadlessContext>();
  
  if (!m_context->isValid()) {
    LERROR("(p) can't create opengl context");
    m_context = nullptr;
    return;
  }
  
  resizeFramebuffer();
  loadShaders();
  loadGeometry();
  loadVolumeData();
  loadTransferFunction();
  
  m_targetBinder = mocca::make_unique<GLTargetBinder>();
  LINFO("(p) OpenGL Version " << m_context->getVersion());
}

void SimpleRenderer::resizeFramebuffer() {
  AbstractRenderer::resizeFramebuffer();
  
  if (!m_context) {
    LWARNING("(p) resizeFramebuffer called without a valid context");
    return;
  }
  
  const uint32_t width = m_visStream->getStreamingParams().getResX();
  const uint32_t height = m_visStream->getStreamingParams().getResY();
  m_bufferData.resize(width * height);
  LINFO("(p) resolution: " << width << " x " << height);
  
  initFrameBuffers();
}


#define LOADSHADER(p, ...)  \
do { \
p = std::unique_ptr<GLProgram>(GLProgram::FromFiles(searchDirs,__VA_ARGS__));\
if (!p) { \
LINFO("(p) invalid shader program " << #p); \
p = nullptr; \
return false; \
} \
} while (0)

bool SimpleRenderer::loadShaders() {
  std::vector<std::string> searchDirs;
  searchDirs.push_back(".");
  searchDirs.push_back("shader");
  searchDirs.push_back("../../../src/processing-base/simplerenderer");
  searchDirs.push_back("../../../src/processing-base/simplerenderer/shader");
  
  
  LOADSHADER(m_backfaceShader,
             "vertex.glsl",NULL,"backfaceFragment.glsl",NULL);
  LOADSHADER(m_raycastShader,
             "vertex.glsl",NULL,"raycastFragment.glsl",NULL);
  
  return true;
}

void SimpleRenderer::loadVolumeData() {
  LINFO("(p) creating volume");
  
  if (m_io->getComponentCount(0) != 1) {
    LERROR("(p) this basic renderer only supports scalar data");
    return;
  }
  
  GLenum format;
  size_t byteWidth;
  if (m_io->getType(0) == IIO::ValueType::T_UINT8 ||
      m_io->getType(0) == IIO::ValueType::T_INT8) {
    format = GL_UNSIGNED_BYTE;
    byteWidth = 1;
  } else {
    if (m_io->getType(0) == IIO::ValueType::T_UINT16 ||
        m_io->getType(0) == IIO::ValueType::T_INT16) {
      format = GL_UNSIGNED_SHORT;
      byteWidth = 2;
    } else {
      LERROR("(p) this basic renderer only supports 8bit and 16bit data");
      return;
    }
  }
  
  // this simple renderer cannot handle bricks, so we render the
  // "best" LoD that consists of a single brick
  uint64_t singleBrickLoD = m_io->getLargestSingleBrickLOD(0);
  
  BrickKey brickKey(0, 0, singleBrickLoD, 0);
  
  Core::Math::Vec3ui brickSize = m_io->getBrickVoxelCounts(brickKey);
  
  LINFO("(p) found suitable volume with single brick of size " << brickSize);
  
  bool success;
  auto volume = m_io->getBrick(brickKey, success);
  
  if (volume->size() != brickSize.volume()*byteWidth) {
    LERROR("invalid volume data vector. size should be " << brickSize.volume()*byteWidth << " but is " << volume->size());
    return;
  } else {
    LINFO("(p) volume size data ok");
  }
  
  m_texVolume = mocca::make_unique<GLTexture3D>(brickSize.x, brickSize.y,
                                                brickSize.z, GL_RED, GL_RED,
                                                format, volume->data(),
                                                GL_LINEAR, GL_LINEAR);
  
  LINFO("(p) volume created");
    
  m_domainTransform = Core::Math::Mat4f(m_io->getTransformation(0));
}

void SimpleRenderer::loadTransferFunction() {
  LINFO("(p) creating transfer function");
  
  uint64_t maxIndex = m_io->getDefault1DTransferFunctionCount() - 1;
  
  LINFO("(p) using default function " << maxIndex);
  
  TransferFunction1D tf = m_io->getDefault1DTransferFunction(maxIndex);
  
  LINFO("(p) filling openGL resource");
  
  m_texTransferFunc = mocca::make_unique<GLTexture1D>(tf.getSize(), GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, tf.getRAWData().data());
  LINFO("(p) transfer function created");
}

void SimpleRenderer::loadGeometry() {
  Core::Math::Vec3f vMinPoint, vMaxPoint;
  vMinPoint = -m_vExtend/2.0;
  vMaxPoint =  m_vExtend/2.0;
  
  m_bbBox = mocca::make_unique<GLVolumeBox>(vMinPoint, vMaxPoint);
}

void SimpleRenderer::initFrameBuffers() {
  const uint32_t width = m_visStream->getStreamingParams().getResX();
  const uint32_t height = m_visStream->getStreamingParams().getResY();
  
  std::shared_ptr<GLFBO> fbo = std::make_shared<GLFBO>();

  m_resultBuffer = std::make_shared<GLRenderTexture>(fbo, GL_NEAREST, GL_NEAREST,
                                              GL_CLAMP_TO_EDGE, width, height,
                                              GL_RGBA, GL_RGBA,
                                              GL_UNSIGNED_BYTE, true, 1);
  
  // TODO: check if we need depth (the true below)
  m_backfaceBuffer = std::make_shared<GLRenderTexture>(fbo, GL_NEAREST, GL_NEAREST,
                                                GL_CLAMP_TO_EDGE, width, height,
                                                GL_RGBA32F, GL_RGBA,
                                                GL_FLOAT, true, 1);
}

void SimpleRenderer::paintInternal(PaintLevel paintlevel) {
  
  if (!m_context || !m_backfaceShader || !m_texVolume || !m_texTransferFunc) {
    LERROR("(p) incomplete OpenGL initialization");
    return;
  }
  
  m_context->makeCurrent();
  
  if (paintlevel == IRenderer::PaintLevel::PL_REDRAW_VISIBILITY_CHANGE ||
      paintlevel == IRenderer::PaintLevel::PL_REDRAW)  {
    
    const uint32_t width = m_visStream->getStreamingParams().getResX();
    const uint32_t height = m_visStream->getStreamingParams().getResY();
    GL_CHECK(glViewport(0, 0, width, height));

    /*
     DEBUG CODE
    Mat4f world;
    Mat4f rotx, roty;
    rotx.RotationX(m_isoValue[0]);
    roty.RotationY(m_isoValue[0] * 1.14f);
    world = rotx * roty;
     */
    
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    
    m_texTransferFunc->Bind(0);
    m_texVolume->Bind(1);
    
    // pass 1: backface (i.e. ray exit coordinates)
    
    m_targetBinder->Bind(m_backfaceBuffer);
    m_backfaceBuffer->ClearPixels(0.0f, 0.0f, 0.0f, 0.0f);
    
    m_backfaceShader->Enable();
    m_backfaceShader->Set("projectionMatrix", m_projection);
    m_backfaceShader->Set("viewMatrix", m_view);
    m_backfaceShader->Set("worldMatrix", m_domainTransform*m_model);
    m_bbBox->paint();
    m_backfaceShader->Disable();
    
    
    // pass 2: raycasting
    
    m_backfaceBuffer->Read(2);
    
    m_targetBinder->Bind(m_resultBuffer);
    glCullFace(GL_BACK);
    
    m_resultBuffer->ClearPixels(0.0f, 0.0f, 0.0f, 0.0f);
    
    m_raycastShader->Enable();
    m_raycastShader->Set("projectionMatrix", m_projection);
    m_raycastShader->Set("viewMatrix", m_view);
    m_raycastShader->Set("worldMatrix", m_domainTransform*m_model);
    m_raycastShader->Set("transferFuncScaleValue", m_1DTFScale);
    
    m_raycastShader->ConnectTextureID("transferfunc", 0);
    m_raycastShader->ConnectTextureID("volume", 1);
    m_raycastShader->ConnectTextureID("rayExit", 2);
    
    m_bbBox->paint();
    m_raycastShader->Disable();
    
    m_backfaceBuffer->FinishRead();
    //auto t1 = std::chrono::high_resolution_clock::now();
    m_resultBuffer->ReadBackPixels(0, 0, width, height, m_bufferData.data());
    //auto t2 = std::chrono::high_resolution_clock::now();
    //LINFO("Time " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count());
    
    m_targetBinder->Unbind();
  }
  
  auto f1 = Frame::createFromRaw(m_bufferData.data(), m_bufferData.size() * 4 * sizeof(uint8_t));
  getVisStream()->put(std::move(f1));
}

bool SimpleRenderer::isIdle() {
  // this renderer is progressive
  return true;
}

bool SimpleRenderer::proceedRendering() {
  if (isIdle()) {
    return false;
  } else {
    paintInternal(IRenderer::PaintLevel::PL_CONTINUE);
    return true;
  }
}


