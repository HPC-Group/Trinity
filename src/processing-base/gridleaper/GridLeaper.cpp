#include "GridLeaper.h"

#include "common/VisStream.h"
#include "opengl-base/OpenGLError.h"
#include "silverbullet/math/Vectors.h"
#include "silverbullet/io/FileTools.h"

#include "mocca/log/LogManager.h"

#include <thread>
#include <vector>

// this is used when the NVIDIA specific getGPUMem call fails
#define DEFAULT_GPU_MEM (1024*1024)

using namespace trinity;
using namespace Core::Math;
using namespace std;
using namespace Core::IO::FileTools;

GridLeaper::GridLeaper(std::shared_ptr<VisStream> stream,
                       std::unique_ptr<IIO> ioSession) :
AbstractRenderer(stream, std::move(ioSession)),
m_texTransferFunc(nullptr),
m_targetBinder(nullptr),
m_programRenderFrontFaces(nullptr),
m_programRenderFrontFacesNearPlane(nullptr),
m_programRayCast1D(nullptr),
m_programRayCast1DColor(nullptr),
m_programRayCast1DLighting(nullptr),
m_programRayCast1DLightingColor(nullptr),
m_programRayCast2D(nullptr),
m_programRayCast2DColor(nullptr),
m_programRayCast2DLighting(nullptr),
m_programRayCast2DLightingColor(nullptr),
m_programRayCastISOLighting(nullptr),
m_programRayCastISOColorLighting(nullptr),
m_programCompose(nullptr),
m_programComposeColorDebugMix(nullptr),
m_programComposeColorDebugMixAlpha(nullptr),
m_programComposeClearViewIso(nullptr),
m_activeShaderProgram(nullptr),
m_hashTable(nullptr),
m_volumePool(nullptr),
m_resultBuffer(nullptr),
m_pFBORayStart(nullptr),
m_pFBORayStartNext(nullptr),
m_pFBOStartColor(nullptr),
m_pFBOStartColorNext(nullptr),
m_pFBOFinalColor(nullptr),
m_pFBOFinalColorNext(nullptr),
#ifdef GLGRIDLEAPER_DEBUGVIEW
m_pFBODebug(nullptr),
m_pFBODebugNext(nullptr),
#endif
m_bBox(nullptr),
m_nearPlane(nullptr),
m_context(nullptr),
m_visibilityState(),
m_fLODFactor(0),
m_isIdle(true)
{
}

void GridLeaper::deleteContext() {
  m_texTransferFunc = nullptr;
  //m_texTransfer2DFunc = nullptr;
  
  m_targetBinder = nullptr;
  m_resultBuffer = nullptr;
  m_pFBORayStart = nullptr;
  m_pFBORayStartNext = nullptr;
  m_pFBOStartColor = nullptr;
  m_pFBOStartColorNext = nullptr;
  m_pFBOFinalColor = nullptr;
  m_pFBOFinalColorNext = nullptr;
#ifdef GLGRIDLEAPER_DEBUGVIEW
  m_pFBODebugNext = nullptr;
  m_pFBORayStart = nullptr;
#endif
  
  m_volumePool = nullptr;
  m_hashTable = nullptr;
  
  m_bBox = nullptr;
  m_nearPlane = nullptr;

  m_activeShaderProgram = nullptr;
  
  m_programRenderFrontFaces = nullptr;
  m_programRenderFrontFacesNearPlane = nullptr;
  m_programRayCast1D = nullptr;
  m_programRayCast1DColor = nullptr;
  m_programRayCast1DLighting = nullptr;
  m_programRayCast1DLightingColor = nullptr;
  m_programRayCast2D = nullptr;
  m_programRayCast2DColor = nullptr;
  m_programRayCast2DLighting = nullptr;
  m_programRayCast2DLightingColor = nullptr;
  m_programRayCastISOLighting = nullptr;
  m_programRayCastISOColorLighting = nullptr;
  m_programCompose = nullptr;
  m_programComposeColorDebugMix = nullptr;
  m_programComposeColorDebugMixAlpha = nullptr;
  m_programComposeClearViewIso = nullptr;
  
  m_context = nullptr;
}

GridLeaper::~GridLeaper() {
  LINFO("(p) destroying a gridleaper");
}

void GridLeaper::initContext() {
  std::thread::id threadId = std::this_thread::get_id();
  LINFO("gridleaper performs cotext init from thread " << threadId);
  m_context = mocca::make_unique<OpenGlHeadlessContext>();

  if (!m_context->isValid()) {
    LERROR("(p) can't create opengl context");
    m_context = nullptr;
    return;
  }

  initHashTable();
  initVolumePool(getFreeGPUMemory()*1024);

  resizeFramebuffer();
  loadShaders(GLVolumePool::MissingBrickStrategy::SkipTwoLevels);
  loadGeometry();
  loadTransferFunction();

  m_targetBinder = mocca::make_unique<GLTargetBinder>();
  LINFO("(p) grid leaper created. OpenGL Version " << m_context->getVersion());
}

void GridLeaper::resizeFramebuffer() {
  AbstractRenderer::resizeFramebuffer();

  const uint32_t width = m_visStream->getStreamingParams().getResX();
  const uint32_t height = m_visStream->getStreamingParams().getResY();
  m_bufferData.resize(width * height * 4);

  const float fScreenSpaceError = 0.5f;

  m_fLODFactor = 2.0f * tan(m_viewAngle * ((3.1416f / 180.0f) / 2.0f)) *
                 fScreenSpaceError / float(height);
  /*
  m_fLODFactor = 2.0f * tan(m_viewAngle *
                            ((3.1416f / 180.0f) / 2.0f)) * 1.0f / float(width);
                            */
  if (!m_context) {
    LWARNING("(p) resizeFramebuffer called without a valid context");
    return;
  }

  initFrameBuffers();

  LINFO("(p) resolution: " << width << " x " << height);
}

#define LOAD_SHADER(p, ...)  \
do { \
p = std::unique_ptr<GLProgram>(GLProgram::FromFiles(searchDirs,__VA_ARGS__));\
if (!p) { \
LINFO("(p) invalid shader program " << #p); \
p = nullptr; \
return false; \
} \
} while (0)

#define LOAD_SHADER_WITH_FRAGMENT(p,sd,id)  \
do { \
  p = mocca::make_unique<GLProgram>();\
  p->Load(sd);\
  if (!p->IsValid()) {\
    LERROR("(p) invalid "<< id <<" shader program");\
    p = nullptr;\
    return false;\
  } \
} while (0)

bool GridLeaper::loadShaders(GLVolumePool::MissingBrickStrategy brickStrategy) {
  std::vector<std::string> searchDirs;
  searchDirs.push_back(".");
  searchDirs.push_back("shader");
  searchDirs.push_back("../../../src/processing-base/gridleaper/shader");


  // COMPOSE SHADERS

  LOAD_SHADER(m_programCompose,
             "ComposeVS.glsl",NULL,"ComposeFS.glsl",NULL);
  LOAD_SHADER(m_programComposeColorDebugMix,
             "ComposeVS.glsl",NULL,"ComposeFSColorDebug.glsl",NULL);
  LOAD_SHADER(m_programComposeColorDebugMixAlpha,
             "ComposeVS.glsl",NULL,"ComposeFSColorDebugAlpha.glsl",NULL);
  LOAD_SHADER(m_programComposeClearViewIso,
             "ComposeVS.glsl",NULL,"ComposeFS_CViso.glsl",NULL);


  // FIRST PASS SHADERS
  LOAD_SHADER(m_programRenderFrontFaces,
             "GLGridLeaper-entry-VS.glsl",NULL,"GLGridLeaper-frontfaces-FS.glsl",NULL);
  LOAD_SHADER(m_programRenderFrontFacesNearPlane,
             "GLGridLeaper-NearPlane-VS.glsl",NULL,"GLGridLeaper-frontfaces-FS.glsl",NULL);

  // TRAVERSAL SHADERS
  const std::string poolFragment = m_volumePool->getShaderFragment(
                                                                   3, 4,
                                                                   GLVolumePool::MissingBrickStrategy(brickStrategy)
                                                                   );
  const std::string hashFragment = m_hashTable->getShaderFragment(5);

  std::vector<std::string> vs, fs;
  vs.push_back(findFileInDirs("GLGridLeaper-entry-VS.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-blend.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-Method-1D.glsl", searchDirs));
  fs.push_back(findFileInDirs("Compositing.glsl", searchDirs));
  ShaderDescriptor sd(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);
  LOAD_SHADER_WITH_FRAGMENT(m_programRayCast1D, sd, "1D TF");


  vs.clear(); fs.clear();
  vs.push_back(findFileInDirs("GLGridLeaper-entry-VS.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-blend.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-Method-1D-color.glsl", searchDirs));
  fs.push_back(findFileInDirs("Compositing.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);
  LOAD_SHADER_WITH_FRAGMENT(m_programRayCast1DColor, sd, "Color 1D TF");


  vs.clear(); fs.clear();
  vs.push_back(findFileInDirs("GLGridLeaper-entry-VS.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-blend.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-Method-1D-L.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  fs.push_back(findFileInDirs("lighting.glsl", searchDirs));
  fs.push_back(findFileInDirs("Compositing.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);
  LOAD_SHADER_WITH_FRAGMENT(m_programRayCast1DLighting, sd, "1D TF lighting");


  vs.clear(); fs.clear();
  vs.push_back(findFileInDirs("GLGridLeaper-entry-VS.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-blend.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-Method-1D-L-color.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  fs.push_back(findFileInDirs("lighting.glsl", searchDirs));
  fs.push_back(findFileInDirs("Compositing.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);
  LOAD_SHADER_WITH_FRAGMENT(m_programRayCast1DLightingColor, sd, "Color 1D TF lighting");

  vs.clear(); fs.clear();
  vs.push_back(findFileInDirs("GLGridLeaper-entry-VS.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-blend.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-Method-2D.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  fs.push_back(findFileInDirs("Compositing.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);
  LOAD_SHADER_WITH_FRAGMENT(m_programRayCast2D, sd, "2D TF");

  vs.clear(); fs.clear();
  vs.push_back(findFileInDirs("GLGridLeaper-entry-VS.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-blend.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-Method-2D-color.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  fs.push_back(findFileInDirs("Compositing.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);
  LOAD_SHADER_WITH_FRAGMENT(m_programRayCast2DColor, sd, "Color 2D TF");

  vs.clear(); fs.clear();
  vs.push_back(findFileInDirs("GLGridLeaper-entry-VS.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-blend.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-Method-2D-L.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  fs.push_back(findFileInDirs("lighting.glsl", searchDirs));
  fs.push_back(findFileInDirs("Compositing.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);
  LOAD_SHADER_WITH_FRAGMENT(m_programRayCast2DLighting, sd, "2D TF lighting");

  vs.clear(); fs.clear();
  vs.push_back(findFileInDirs("GLGridLeaper-entry-VS.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-blend.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-Method-2D-L-color.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  fs.push_back(findFileInDirs("lighting.glsl", searchDirs));
  fs.push_back(findFileInDirs("Compositing.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);
  LOAD_SHADER_WITH_FRAGMENT(m_programRayCast2DLightingColor, sd, "Color 2D TF lighting");

  vs.clear(); fs.clear();
  vs.push_back(findFileInDirs("GLGridLeaper-entry-VS.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-iso.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-Method-iso.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);
  LOAD_SHADER_WITH_FRAGMENT(m_programRayCastISOLighting, sd, "Isosurface");

  vs.clear(); fs.clear();
  vs.push_back(findFileInDirs("GLGridLeaper-entry-VS.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-iso.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-Method-iso-color.glsl", searchDirs));
  fs.push_back(findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);
  LOAD_SHADER_WITH_FRAGMENT(m_programRayCastISOColorLighting, sd, "Color Isosurface");

  return true;
}

void GridLeaper::loadTransferFunction() {
  LINFO("(p) creating transfer function");

  m_texTransferFunc = mocca::make_unique<GLTexture1D>(m_1Dtf.getSize(),
                                                      GL_RGBA,
                                                      GL_RGBA,
                                                      GL_UNSIGNED_BYTE,
                                                      m_1Dtf.getRAWData().data());
  LINFO("(p) transfer function created");
}

void GridLeaper::loadGeometry() {
  updateBBox();
  m_nearPlane = mocca::make_unique<GLRenderPlane>();
}

void GridLeaper::initFrameBuffers() {
  const uint32_t width = m_visStream->getStreamingParams().getResX();
  const uint32_t height = m_visStream->getStreamingParams().getResY();

  std::shared_ptr<GLFBO> fbo = std::make_shared<GLFBO>();

  m_pFBORayStart = std::make_shared<GLRenderTexture>(fbo, GL_NEAREST, GL_NEAREST,
                                              GL_CLAMP_TO_EDGE,
                                              width, height,
                                              GL_RGBA32F, GL_RGBA,
                                              GL_FLOAT, true, 1);

  m_pFBORayStartNext = std::make_shared<GLRenderTexture>(fbo, GL_NEAREST, GL_NEAREST,
                                                  GL_CLAMP_TO_EDGE,
                                                  width, height,
                                                  GL_RGBA32F, GL_RGBA,
                                                  GL_FLOAT, true, 1);

  m_pFBOStartColor = std::make_shared<GLRenderTexture>(fbo, GL_NEAREST, GL_NEAREST,
                                                GL_CLAMP_TO_EDGE,
                                                width, height,
                                                GL_RGBA32F, GL_RGBA,
                                                GL_FLOAT, true, 1);

  m_pFBOStartColorNext = std::make_shared<GLRenderTexture>(fbo, GL_NEAREST, GL_NEAREST,
                                                    GL_CLAMP_TO_EDGE,
                                                    width, height,
                                                    GL_RGBA32F, GL_RGBA,
                                                    GL_FLOAT, true, 1);

  m_pFBOFinalColor = std::make_shared<GLRenderTexture>(fbo, GL_NEAREST, GL_NEAREST,
                                                GL_CLAMP_TO_EDGE,
                                                width, height,
                                                GL_RGBA32F, GL_RGBA,
                                                GL_FLOAT, true, 1);

  m_pFBOFinalColorNext = std::make_shared<GLRenderTexture>(fbo, GL_NEAREST, GL_NEAREST,
                                                    GL_CLAMP_TO_EDGE,
                                                    width, height,
                                                    GL_RGBA32F, GL_RGBA,
                                                    GL_FLOAT, true, 1);

#ifdef GLGRIDLEAPER_DEBUGVIEW
  m_pFBODebug = std::make_shared<GLRenderTexture>(fbo, GL_NEAREST, GL_NEAREST,
                                           GL_CLAMP_TO_EDGE,
                                           width, height,
                                           GL_RGBA32F, GL_RGBA,
                                           GL_FLOAT, true, 1);

  m_pFBODebugNext = std::make_shared<GLRenderTexture>(fbo, GL_NEAREST, GL_NEAREST,
                                               GL_CLAMP_TO_EDGE,
                                               width, height,
                                               GL_RGBA32F, GL_RGBA,
                                               GL_FLOAT, true, 1);
#endif

  m_resultBuffer = std::make_shared<GLRenderTexture>(fbo, GL_NEAREST, GL_NEAREST,
	  GL_CLAMP_TO_EDGE, width, height,
	  GL_RGBA, GL_RGBA,
	  GL_UNSIGNED_BYTE, true, 1);
}

void GridLeaper::initHashTable() {
  Vec3ui const finestBrickLayout(m_io->getBrickLayout(0, m_activeModality));
  m_hashTable = mocca::make_unique<GLHashTable>(finestBrickLayout);
  m_hashTable->initGL();
}

void GridLeaper::initVolumePool(uint64_t gpuMemorySizeInByte) {
  //CalculateUsedGPUMemory();
  
  m_volumePool = mocca::make_unique<GLVolumePool>(gpuMemorySizeInByte,
                                                  *m_io,
                                                  m_activeModality,
                                                  GL_LINEAR);
  

  if (m_volumePool){
    // upload a brick that covers the entire domain to make sure have
    // something to render

    // find lowest LoD with only a single brick
    uint64_t singleBrickLoD = m_io->getLargestSingleBrickLOD(0);

    BrickKey brickKey(m_activeModality, m_activeTimestep, singleBrickLoD, 0);

    GL_CHECK_EXT();

    m_volumePool->uploadFirstBrick(brickKey);

    GL_CHECK_EXT();

    RecomputeBrickVisibility(false);
  }
}

void GridLeaper::paintInternal(PaintLevel paintlevel) {

  if (!m_context) {
    LERROR("(p) incomplete OpenGL initialization");
    return;
  }

  m_context->makeCurrent(); // todo: check if we need this

  GL_CHECK(glClearColor(0, 0, 0, 0));

  const uint32_t width = m_visStream->getStreamingParams().getResX();
  const uint32_t height = m_visStream->getStreamingParams().getResY();
  GL_CHECK(glViewport(0, 0, width, height));


  // fall through an purpose
  switch (paintlevel) {
    case IRenderer::PaintLevel::PL_REDRAW_VISIBILITY_CHANGE :
      RecomputeBrickVisibility(false);
    case IRenderer::PaintLevel::PL_REDRAW :
	  computeEyeToModelMatrix();
	  fillRayEntryBuffer();
      m_isIdle = false;
    case IRenderer::PaintLevel::PL_CONTINUE :
    case IRenderer::PaintLevel::PL_RECOMPOSE :
      break;
  }

  if(!m_isIdle){
    m_volumePool->uploadBricks();
                               
    m_hashTable->clearData();

    GL_CHECK_EXT();

    raycast();
	
    GL_CHECK_EXT();
    // compose (sends data to frontend)
    compose();
    GL_CHECK_EXT();

	swapToNextBuffer();

  GL_CHECK_EXT();
  // update volumepool
    std::vector<Vec4ui> hash = m_hashTable->getData();
    GL_CHECK_EXT();

    if (hash.size() > 0) {
      m_volumePool->requestBricks(hash, m_visibilityState);
    } else {
      m_isIdle = true;
    }

    GL_CHECK_EXT();

  } else {
    if (paintlevel == IRenderer::PaintLevel::PL_RECOMPOSE) compose();
  }
  GL_CHECK_EXT();

}


void GridLeaper::computeEyeToModelMatrix() {
  Core::Math::Mat4f mScale, mNormalize;
  mScale.Scaling(1.0f/m_vExtend);
  mNormalize.Translation(0.5f, 0.5f, 0.5f);
  m_EyeToModelMatrix = m_modelView.inverse() * mScale * mNormalize;
}


void GridLeaper::fillRayEntryBuffer() {
#ifdef GLGRIDLEAPER_DEBUGVIEW
  m_targetBinder->bind(m_pFBOFinalColor, m_pFBOFinalColorNext, m_pFBOStartColor, m_pFBOStartColorNext);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_targetBinder->bind(m_pFBODebug, m_pFBODebugNext, m_pFBORayStart, m_pFBORayStartNext);
  glClear(GL_COLOR_BUFFER_BIT);
#else
  m_targetBinder->Bind(m_pFBOFinalColor, m_pFBOFinalColorNext, m_pFBOStartColor, m_pFBOStartColorNext);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_targetBinder->Bind(m_pFBORayStart, m_pFBORayStartNext);
  glClear(GL_COLOR_BUFFER_BIT);
#endif
  m_targetBinder->Unbind();

  //bind the entrance buffer as target
  m_targetBinder->Bind(m_pFBORayStart);

  //! \todo add the statemanager here !
  GL_CHECK(glDisable(GL_CULL_FACE));

  m_programRenderFrontFacesNearPlane->Enable();
  m_programRenderFrontFacesNearPlane->Set("mEyeToModel", m_EyeToModelMatrix);
  m_programRenderFrontFacesNearPlane->Set("mInvProjection", m_projection.inverse());
    
  m_nearPlane->paint();

  m_programRenderFrontFaces->Enable();
  m_programRenderFrontFaces->Set("mEyeToModel", m_EyeToModelMatrix);
  m_programRenderFrontFaces->Set("mModelView", m_modelView);
  m_programRenderFrontFaces->Set("mModelViewProjection", m_modelView*m_projection);

  GL_CHECK(glEnable(GL_CULL_FACE));
  GL_CHECK(glCullFace(GL_BACK));
  GL_CHECK(glDisable(GL_DEPTH_TEST));
  GL_CHECK(glDisable(GL_BLEND));

  m_bBox->paint();

  m_programRenderFrontFaces->Disable();

  m_targetBinder->Unbind();
}

                                  
void GridLeaper::raycast(){
  selectShader();
  setupRaycastShader();

  m_targetBinder->Unbind();

#ifdef GLGRIDLEAPER_DEBUGVIEW
  m_targetBinder->Bind(m_pFBOFinalColorNext,
                        m_pFBOStartColorNext,
                        m_pFBORayStartNext,
                        m_pFBODebugNext);
#else
  m_targetBinder->Bind(m_pFBOFinalColorNext, m_pFBOStartColorNext, m_pFBORayStartNext);
#endif

  m_pFBORayStart->Read(0);
  m_pFBOStartColor->Read(1);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


  //set states
  GL_CHECK(glEnable(GL_CULL_FACE));
  GL_CHECK(glCullFace(GL_FRONT));
  GL_CHECK(glDisable(GL_DEPTH_TEST));
  GL_CHECK(glDepthMask(GL_FALSE));

  // render
  m_bBox->paint();

  m_volumePool->disable();

  m_pFBORayStart->FinishRead();
  m_pFBOStartColor->FinishRead();

  m_targetBinder->Unbind();
}

void GridLeaper::compose(){
  m_targetBinder->Bind(m_resultBuffer);

  GL_CHECK(glCullFace(GL_FRONT));
  GL_CHECK(glDisable(GL_CULL_FACE));
  GL_CHECK(glDisable(GL_DEPTH_TEST));
  GL_CHECK(glDepthMask(GL_FALSE));
  GL_CHECK(glDisable(GL_BLEND));
  
  m_programCompose->Enable();
  m_programCompose->SetTexture2D("compose", m_pFBOFinalColorNext->GetTextureHandle(), 0);

  GL_CHECK_EXT();

  Vec3f c1 = Vec3f(m_backgroundColors.colorOne)/255.0f;
  Vec3f c2 = Vec3f(m_backgroundColors.colorTwo)/255.0f;

  m_programCompose->Set("ColorOne", c1);
  m_programCompose->Set("ColorTwo", c2);

  GL_CHECK_EXT();

  m_nearPlane->paint();

  m_programCompose->Disable();

  GL_CHECK_EXT();

  const uint32_t width = m_visStream->getStreamingParams().getResX();
  const uint32_t height = m_visStream->getStreamingParams().getResY();
  m_resultBuffer->ReadBackPixels(0, 0, width, height, m_bufferData.data());

  GL_CHECK_EXT();

  m_targetBinder->Unbind();
  GL_CHECK_EXT();

  Frame f1;
  f1.assign(begin(m_bufferData), end(m_bufferData));
  getVisStream()->put(std::move(f1));
}


void GridLeaper::selectShader(){
  if (m_semantic == IIO::Semantic::Color){
    if (m_enableLighting){
      switch (m_renderMode) {
        case  IRenderer::ERenderMode::RM_1DTRANS :
			m_texTransferFunc->Bind(2);
			m_activeShaderProgram = m_programRayCast1DLightingColor;
          break;
        case  IRenderer::ERenderMode::RM_2DTRANS :
		//	m_texTransfer2DFunc->Bind(2);
			m_activeShaderProgram = m_programRayCast1DLightingColor;
          break;
        case  IRenderer::ERenderMode::RM_ISOSURFACE :
          m_activeShaderProgram = m_programRayCastISOColorLighting;
          break;
        default:
          throw TrinityError("Rendermode not yet implemented", __FILE__, __LINE__);
      }
    } else {
      switch (m_renderMode) {
        case  IRenderer::ERenderMode::RM_1DTRANS :
			m_texTransferFunc->Bind(2);
			m_activeShaderProgram = m_programRayCast1DColor;
          break;
        case  IRenderer::ERenderMode::RM_2DTRANS :
			//	m_texTransfer2DFunc->Bind(2);
			m_activeShaderProgram = m_programRayCast2DColor;
          break;
        case  IRenderer::ERenderMode::RM_ISOSURFACE :
          m_activeShaderProgram = m_programRayCastISOColorLighting;  // invalid rendermode -> use lighiting instead
          break;
        default:
          throw TrinityError("Rendermode not yet implemented", __FILE__, __LINE__);
      }
    }
  } else {
    if (m_enableLighting){
      switch (m_renderMode) {
        case  IRenderer::ERenderMode::RM_1DTRANS :
			m_texTransferFunc->Bind(2);
			m_activeShaderProgram = m_programRayCast1DLighting;
          break;
        case  IRenderer::ERenderMode::RM_2DTRANS :
			//	m_texTransfer2DFunc->Bind(2);
			m_activeShaderProgram = m_programRayCast2DLighting;
          break;
        case  IRenderer::ERenderMode::RM_ISOSURFACE :
          m_activeShaderProgram = m_programRayCastISOLighting;
          break;
        default:
          throw TrinityError("Rendermode not yet implemented", __FILE__, __LINE__);
      }
    } else {
      switch (m_renderMode) {
        case  IRenderer::ERenderMode::RM_1DTRANS :
			m_texTransferFunc->Bind(2);
			m_activeShaderProgram = m_programRayCast1D;
          break;
        case  IRenderer::ERenderMode::RM_2DTRANS :
			//	m_texTransfer2DFunc->Bind(2);
			m_activeShaderProgram = m_programRayCast2D;
          break;
        case  IRenderer::ERenderMode::RM_ISOSURFACE :
          m_activeShaderProgram = m_programRayCastISOLighting;  // invalid rendermode -> use lighiting instead
          break;
        default:
          throw TrinityError("Rendermode not yet implemented", __FILE__, __LINE__);
      }
    }
  }
}

void GridLeaper::setupRaycastShader() {
  
  m_volumePool->enable(m_fLODFactor,
                        m_vExtend,
                        m_vScale,
                        m_activeShaderProgram); // bound to 3 and 4
  GL_CHECK_EXT();
 
  m_hashTable->enable(); // bound to 5

  GL_CHECK_EXT();

#ifdef GLGRIDLEAPER_DEBUGVIEW
  if (m_iDebugView == 2)
    m_pFBODebug->Read(6);
#endif
#ifdef GLGRIDLEAPER_WORKINGSET
  m_pWorkingSetTable->Enable(); // bound to 7
#endif

  // set shader parameters
  m_activeShaderProgram->Enable();
  m_activeShaderProgram->Set("sampleRateModifier", m_fSampleRateModifier);
  m_activeShaderProgram->Set("mEyeToModel", m_EyeToModelMatrix);
  m_activeShaderProgram->Set("mModelView", m_modelView);
  m_activeShaderProgram->Set("mModelViewProjection", m_modelView*m_projection);

  if (m_renderMode == IRenderer::ERenderMode::RM_ISOSURFACE) {
    m_activeShaderProgram->Set("fIsoval", m_isoValue[0]);
    Vec3f invScale = 1.0f/m_vScale;
    m_activeShaderProgram->Set("vDomainScale",invScale);
    m_activeShaderProgram->Set("mModelToEye",m_EyeToModelMatrix.inverse());
    m_activeShaderProgram->Set("mModelViewIT", m_modelView.inverse(), true);
  } else {
    m_activeShaderProgram->Set("fTransScale",m_1DTFScale);
    if (m_renderMode == IRenderer::ERenderMode::RM_2DTRANS) {
      // m_activeShaderProgram->Set("fGradientScale",m_2DTFScale);
    }

    if (m_enableLighting) {
      Vec3f a(Vec3f(m_lightingColors.ambient.xyz())*(m_lightingColors.ambient.w / 255.f));
      Vec3f d(Vec3f(m_lightingColors.diffuse.xyz())*(m_lightingColors.diffuse.w / 255.f));
      Vec3f s(Vec3f(m_lightingColors.specular.xyz())*(m_lightingColors.specular.w / 255.f));
      
	  a = a / 255.f;
	  d = d / 255.f;
	  s = s / 255.f;

      Vec3f invScale = 1.0f/m_vScale;
      
      Vec3f vModelSpaceLightDir = (Vec4f(m_lightDirection,0.0f) * m_EyeToModelMatrix ).xyz().normalized();
      Vec3f vModelSpaceEyePos   = (Vec4f(m_eyePos,1.0f) /* used to be Vec4f(0,0,0,1) */ * m_EyeToModelMatrix).xyz();
      
      m_activeShaderProgram->Set("vLightAmbient",a);
      m_activeShaderProgram->Set("vLightDiffuse",d);
      m_activeShaderProgram->Set("vLightSpecular",s);
      m_activeShaderProgram->Set("vModelSpaceLightDir",vModelSpaceLightDir);
      m_activeShaderProgram->Set("vModelSpaceEyePos",vModelSpaceEyePos);
      m_activeShaderProgram->Set("vDomainScale",invScale);
    }
  }
}

void GridLeaper::swapToNextBuffer(){
  std::swap(m_pFBORayStart, m_pFBORayStartNext);
  std::swap(m_pFBOStartColor, m_pFBOStartColorNext);
  std::swap(m_pFBOFinalColor, m_pFBOFinalColorNext);
#ifdef GLGRIDLEAPER_DEBUGVIEW
  std::swap(m_pFBODebug, m_pFBODebugNext);
#endif
}

Vec4ui GridLeaper::RecomputeBrickVisibility(bool bForceSynchronousUpdate) {
  // (totalProcessedBrickCount, emptyBrickCount, childEmptyBrickCount)
  Vec4ui vEmptyBrickCount(0, 0, 0, 0);
  if (!m_volumePool) return vEmptyBrickCount;

  double const fMaxValue = (m_io->getRange(m_activeModality).x > m_io->getRange(m_activeModality).y) ? m_1Dtf.getSize() : m_io->getRange(m_activeModality).y;
  double const fRescaleFactor = fMaxValue / double(m_1Dtf.getSize() - 1);

  // render mode dictates how we look at data ...
  switch (m_renderMode) {
    case ERenderMode::RM_1DTRANS: {
      double const fMin = double(m_1Dtf.getNonZeroLimits().x) * fRescaleFactor;
      double const fMax = double(m_1Dtf.getNonZeroLimits().y) * fRescaleFactor;
      if (m_visibilityState.needsUpdate(fMin, fMax) ||
          bForceSynchronousUpdate) {
        vEmptyBrickCount = m_volumePool->recomputeVisibility(m_visibilityState,
                                                             m_activeTimestep,
                                                             m_activeModality,
                                                             bForceSynchronousUpdate);
      }
      break; }
    case ERenderMode::RM_2DTRANS: {
      /* double const fMin = double(m_p2DTrans->GetNonZeroLimits().x) * fRescaleFactor;
       double const fMax = double(m_p2DTrans->GetNonZeroLimits().y) * fRescaleFactor;
       double const fMinGradient = double(m_p2DTrans->GetNonZeroLimits().z);
       double const fMaxGradient = double(m_p2DTrans->GetNonZeroLimits().w);
       if (m_VisibilityState->NeedsUpdate(fMin, fMax, fMinGradient, fMaxGradient) ||
       bForceSynchronousUpdate) {
       vEmptyBrickCount = m_volumePool->recomputeVisibility(m_visibilityState,
       m_activeTimestep,
       m_activeModality,
       bForceSynchronousUpdate);
       }*/
      break; }
    case ERenderMode::RM_ISOSURFACE: {
      double const fIsoValue = m_isoValue[0];
      if (m_visibilityState.needsUpdate(fIsoValue) ||
          bForceSynchronousUpdate) {
        vEmptyBrickCount = m_volumePool->recomputeVisibility(m_visibilityState,
                                                             m_activeTimestep,
                                                             m_activeModality,
                                                             bForceSynchronousUpdate);
      }
      break; }
    case ERenderMode::RM_CLEARVIEW: {
      double const fIsoValue1 = m_isoValue[0];
      double const fIsoValue2 = m_isoValue[1];
      if (m_visibilityState.needsUpdateCV(fIsoValue1, fIsoValue2) ||
          bForceSynchronousUpdate) {
        vEmptyBrickCount = m_volumePool->recomputeVisibility(m_visibilityState,
                                                             m_activeTimestep,
                                                             m_activeModality,
                                                             bForceSynchronousUpdate);
      }
      break; }
    default:
      LERRORC("GLGridLeaper", "Unhandled rendering mode.");
      break;
  }
  return vEmptyBrickCount;
}

const uint64_t GridLeaper::getFreeGPUMemory(){
  GLint freememory = DEFAULT_GPU_MEM;

  std::string vendor = (char*)glGetString(GL_VENDOR);
  LINFOC("GridLeaper", "gpu vendor: " << vendor);
  if (vendor == "NVIDIA Corporation"){

    // get the currently AVAILABLE!! free gpu memory
    glGetIntegerv(0x9049, &freememory);

    //reduce useable memory by 100 MByte
    freememory = freememory - (102400);
    if (freememory < 0){
      LWARNINGC("GridLeaper", "not enough free VRAM.");
      return DEFAULT_GPU_MEM;
    }

    LINFOC("GridLeaper", "available VRAM in kb: " << freememory);
  }
  return freememory;
}


void GridLeaper::set1DTransferFunction(const TransferFunction1D& tf) {
  if ( m_context ) {
    m_texTransferFunc = mocca::make_unique<GLTexture1D>(tf.getSize(),
                                                        GL_RGBA,
                                                        GL_RGBA,
                                                        GL_UNSIGNED_BYTE,
                                                        tf.getRAWData().data());
  }
  AbstractRenderer::set1DTransferFunction(tf);
}

/*
void GridLeaper::set2DTransferFunction(const TransferFunction2D& tf) {
  if ( m_context ) {
  }

  AbstractRenderer::set2DTransferFunction(tf);
}
 */

bool GridLeaper::isIdle() {
  return m_isIdle;
}

void GridLeaper::performClipping() {
  updateBBox();
}

void GridLeaper::updateBBox() {
  Vec3f vMinPoint(-m_vExtend/2.0);
  Vec3f vMaxPoint(m_vExtend/2.0);
  if (m_enableClipping) {
    Vec3f vScaledMinPoint((Vec3f(1.0f,1.0f,1.0f)-m_clipVolumeMin) * vMinPoint +
                          m_clipVolumeMin * vMaxPoint);
    Vec3f vScaledMaxPoint((Vec3f(1.0f,1.0f,1.0f)-m_clipVolumeMax) * vMinPoint +
                          m_clipVolumeMax * vMaxPoint);
    m_bBox = mocca::make_unique<GLVolumeBox>(vScaledMinPoint,
                                             vScaledMaxPoint);
  } else {
    m_bBox = mocca::make_unique<GLVolumeBox>(vMinPoint, vMaxPoint);
  }
}

bool GridLeaper::proceedRendering() {
  if (isIdle()) {
    return false;
  } else {
    paintInternal(IRenderer::PaintLevel::PL_CONTINUE);
    return true;
  }
}
