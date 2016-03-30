#include "GridLeaper.h"

#include "common/VisStream.h"
#include "opengl-base/OpenGLError.h"
#include "silverbullet/math/Vectors.h"
#include "silverbullet/io/FileTools.h"

#include "mocca/log/LogManager.h"

#include <thread>
#include <vector>

// this is used when the NVIDIA specific getGPUMem call fails
#define DEFAULT_GPU_MEM 2048

using namespace trinity;
using namespace Core::Math;
using namespace std;

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
m_programRayCastISO(nullptr),
m_programRayCastISOLighting(nullptr),
m_programRayCastISOColor(nullptr),
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
m_bbBox(nullptr),
m_nearPlane(nullptr),
m_context(nullptr),
m_visibilityState(),
m_fLODFactor(0),
m_isIdle(true)
{
}

void GridLeaper::deleteContext() {
  m_texTransferFunc = nullptr;
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
  m_bbBox = nullptr;
  m_nearPlane = nullptr;
  m_context = nullptr;

  m_programRenderFrontFaces = nullptr;
  m_programRenderFrontFacesNearPlane = nullptr;
  m_programRayCast1D = nullptr;
  m_programRayCast1DColor = nullptr;
  m_programRayCast1DLighting = nullptr;
  m_programRayCastISO = nullptr;
  m_programRayCastISOLighting = nullptr;
  m_programRayCastISOColor = nullptr;
  m_programCompose = nullptr;
  m_programComposeColorDebugMix = nullptr;
  m_programComposeColorDebugMixAlpha = nullptr;
  m_programComposeClearViewIso = nullptr;
  m_activeShaderProgram = nullptr;
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
  initVolumePool(getFreeGPUMemory());

  resizeFramebuffer();
  loadShaders(GLVolumePool::MissingBrickStrategy::RequestAll); //todo guess we could use the "renderspecials" here
  loadGeometry();
  loadTransferFunction();

  m_targetBinder = mocca::make_unique<GLTargetBinder>();
  LINFO("(p) grid leaper created. OpenGL Version " << m_context->getVersion());
}

void GridLeaper::resizeFramebuffer() {
  AbstractRenderer::resizeFramebuffer();

  const uint32_t width = m_visStream->getStreamingParams().getResX();
  const uint32_t height = m_visStream->getStreamingParams().getResY();
  m_bufferData.resize(width * height);

  m_fLODFactor = 2.0f * tan(m_viewAngle *
                            ((3.1416f / 180.0f) / 2.0f)) * 1.0f / float(width);

  if (!m_context) {
    LWARNING("(p) resizeFramebuffer called without a valid context");
    return;
  }

  initFrameBuffers();

  LINFO("(p) resolution: " << width << " x " << height);
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

bool GridLeaper::loadShaders(GLVolumePool::MissingBrickStrategy brickStrategy) {
  std::vector<std::string> searchDirs;
  searchDirs.push_back(".");
  searchDirs.push_back("shader");
  searchDirs.push_back("../../../src/processing-base/gridleaper/shader");

  LOADSHADER(m_programCompose,
             "ComposeVS.glsl",NULL,"ComposeFS.glsl",NULL);
  LOADSHADER(m_programComposeColorDebugMix,
             "ComposeVS.glsl",NULL,"ComposeVS.glsl",NULL);
  LOADSHADER(m_programComposeColorDebugMixAlpha,
             "ComposeVS.glsl",NULL,"ComposeFSColorDebugAlpha.glsl",NULL);
  LOADSHADER(m_programComposeClearViewIso,
             "ComposeVS.glsl",NULL,"ComposeFS_CViso.glsl",NULL);
  LOADSHADER(m_programRenderFrontFaces,
             "CubeVertex.glsl",NULL,"CubeFragment.glsl",NULL);
  LOADSHADER(m_programRenderFrontFacesNearPlane,
             "NearPlaneVS.glsl",NULL,"NearPlaneFS.glsl",NULL);

  // Load the traversal shaders
  const std::string poolFragment = m_volumePool->getShaderFragment(
                                                                   3, 4,
                                                                   GLVolumePool::MissingBrickStrategy(brickStrategy)
                                                                   );
  const std::string hashFragment = m_hashTable->getShaderFragment(5);

  std::vector<std::string> vs, fs;

  vs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-entry-VS.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-blend.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-Method-1D.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("Compositing.glsl", searchDirs));
  ShaderDescriptor sd(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);

  m_programRayCast1D = mocca::make_unique<GLProgram>();
  m_programRayCast1D->Load(sd);

  if (!m_programRayCast1D->IsValid()) {
    LERROR("(p) invalid cube shader program");
    m_programRayCast1D = nullptr;
    return false;
  }

  fs.clear();
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-blend.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-Method-1D-color.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("Compositing.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);

  m_programRayCast1DColor = mocca::make_unique<GLProgram>();
  m_programRayCast1DColor->Load(sd);

  if (!m_programRayCast1DColor->IsValid()) {
    LERROR("(p) invalid cube shader program");
    m_programRayCast1DColor = nullptr;
    return false;
  }

  fs.clear();
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-blend.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-Method-1D-L.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("lighting.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("Compositing.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);

  m_programRayCast1DLighting = mocca::make_unique<GLProgram>();
  m_programRayCast1DLighting->Load(sd);

  if (!m_programRayCast1DLighting->IsValid()) {
    LERROR("(p) invalid cube shader program");
    m_programRayCast1DLighting = nullptr;
    return false;
  }

  fs.clear();
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-iso.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-Method-iso.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);

  m_programRayCastISO = mocca::make_unique<GLProgram>();
  m_programRayCastISO->Load(sd);

  if (!m_programRayCastISO->IsValid()) {
    LERROR("(p) invalid cube shader program");
    m_programRayCastISO = nullptr;
    return false;
  }

  fs.clear();
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-iso.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-Method-iso-color.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);

  m_programRayCastISOColor = mocca::make_unique<GLProgram>();
  m_programRayCastISOColor->Load(sd);

  if (!m_programRayCastISOColor->IsValid()) {
    LERROR("(p) invalid cube shader program");
    m_programRayCastISOColor = nullptr;
    return false;
  }

  fs.clear();
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-iso-lighting.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-Method-iso.glsl", searchDirs));
  fs.push_back(Core::IO::FileTools::findFileInDirs("GLGridLeaper-GradientTools.glsl", searchDirs));
  sd = ShaderDescriptor(vs, fs);
  sd.AddFragmentShaderString(poolFragment);
  sd.AddFragmentShaderString(hashFragment);

  m_programRayCastISOLighting = mocca::make_unique<GLProgram>();
  m_programRayCastISOLighting->Load(sd);

  if (!m_programRayCastISOLighting->IsValid()) {
    LERROR("(p) invalid cube shader program");
    m_programRayCastISOLighting = nullptr;
    return false;
  }

  return true;
}

void GridLeaper::loadTransferFunction() {
  LINFO("(p) creating transfer function");

  uint64_t maxIndex = m_io->getDefault1DTransferFunctionCount()-1;

  LINFO("(p) using default function " << maxIndex);

  TransferFunction1D tf = m_io->getDefault1DTransferFunction(maxIndex);

  LINFO("(p) filling openGL resource");

  m_texTransferFunc = mocca::make_unique<GLTexture1D>(tf.getSize(),
                                                      GL_RGBA,
                                                      GL_RGBA,
                                                      GL_UNSIGNED_BYTE,
                                                      tf.getRAWData().data());
  LINFO("(p) transfer function created");
}

void GridLeaper::loadGeometry() {
  Core::Math::Vec3f vMinPoint, vMaxPoint;
  vMinPoint = -m_vExtend/2.0;
  vMaxPoint =  m_vExtend/2.0;

  m_bbBox = mocca::make_unique<GLVolumeBox>(vMinPoint, vMaxPoint);

  m_nearPlane = mocca::make_unique<GLRenderPlane>();
}

void GridLeaper::initFrameBuffers() {
  const uint32_t width = m_visStream->getStreamingParams().getResX();
  const uint32_t height = m_visStream->getStreamingParams().getResY();

  m_pFBORayStart = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                              GL_CLAMP_TO_EDGE,
                                              width, height,
                                              GL_RGBA, GL_RGBA,
                                              GL_FLOAT, false, 1);

  m_pFBORayStartNext = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                                  GL_CLAMP_TO_EDGE,
                                                  width, height,
                                                  GL_RGBA, GL_RGBA,
                                                  GL_FLOAT, false, 1);

  m_pFBOStartColor = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                                GL_CLAMP_TO_EDGE,
                                                width, height,
                                                GL_RGBA, GL_RGBA,
                                                GL_FLOAT, true, 1);

  m_pFBOStartColorNext = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                                    GL_CLAMP_TO_EDGE,
                                                    width, height,
                                                    GL_RGBA, GL_RGBA,
                                                    GL_FLOAT, true, 1);

  m_pFBOFinalColor = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                                GL_CLAMP_TO_EDGE,
                                                width, height,
                                                GL_RGBA, GL_RGBA,
                                                GL_FLOAT, true, 1);

  m_pFBOFinalColorNext = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                                    GL_CLAMP_TO_EDGE,
                                                    width, height,
                                                    GL_RGBA, GL_RGBA,
                                                    GL_FLOAT, true, 1);
#ifdef GLGRIDLEAPER_DEBUGVIEW
  m_pFBODebug = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                           GL_CLAMP_TO_EDGE,
                                           width, height,
                                           GL_RGBA, GL_RGBA,
                                           GL_FLOAT, true, 1);

  m_pFBODebugNext = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                               GL_CLAMP_TO_EDGE,
                                               width, height,
                                               GL_RGBA, GL_RGBA,
                                               GL_FLOAT, true, 1);
#endif


  m_resultBuffer = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                              GL_CLAMP_TO_EDGE,
                                              width, height,
                                              GL_RGBA, GL_RGBA,
                                              GL_FLOAT, false, 1);
}

void GridLeaper::initHashTable() {
  Vec3ui const finestBrickLayout(m_io->getBrickLayout(0, m_activeModality));
  m_hashTable = mocca::make_unique<GLHashTable>(finestBrickLayout);
  m_hashTable->initGL();
}

void GridLeaper::initVolumePool(uint64_t gpuMemorySizeInByte) {
  //CalculateUsedGPUMemory();
  Vec3ui volumepoolsize = calculateVolumePoolSize(gpuMemorySizeInByte,0);

  //todo how?
  m_volumePool = mocca::make_unique<GLVolumePool>(volumepoolsize, *m_io,
                                                  m_activeModality, GL_LINEAR);

  if (m_volumePool){
    // upload a brick that covers the entire domain to make sure have
    // something to render

    // find lowest LoD with only a single brick
    uint64_t singleBrickLoD = m_io->getLargestSingleBrickLOD(0);

    BrickKey brickKey(m_activeModality, m_activeTimestep, singleBrickLoD, 0);

    m_volumePool->uploadFirstBrick(brickKey,*m_io);

    RecomputeBrickVisibility(false);
  }
}

void GridLeaper::paintInternal(PaintLevel paintlevel) {

  if (!m_context) {
    LERROR("(p) incomplete OpenGL initialization");
    return;
  }

  m_context->makeCurrent(); // todo: check if we need this

  // fall through an purpose
  switch (paintlevel) {
    case IRenderer::PaintLevel::PL_REDRAW_VISIBILITY_CHANGE :
      RecomputeBrickVisibility(false);
    case IRenderer::PaintLevel::PL_REDRAW :
      fillRayEntryBuffer();
      computeEyeToModelMatrix();
      m_isIdle = false;
    case IRenderer::PaintLevel::PL_CONTINUE :
    case IRenderer::PaintLevel::PL_RECOMPOSE :
      break;
  }

  if(!m_isIdle){
    // raycast
    m_hashTable->clearData();
    raycast();
    swapToNextBuffer();

    // compose (sends data to frontend)
    compose();

    // update volumepool
    std::vector<Vec4ui> hash = m_hashTable->getData();
    if (hash.size() > 0) {
      m_volumePool->uploadBricks(hash,
                                 m_visibilityState,
                                 *m_io,
                                 m_activeModality);
    } else {
      m_isIdle = true;
    }

  } else {
    if (paintlevel == IRenderer::PaintLevel::PL_RECOMPOSE) compose();
  }

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
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_targetBinder->bind(m_pFBODebug, m_pFBODebugNext,m_pFBORayStart,m_pFBORayStartNext);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
#else
  m_targetBinder->Bind(m_pFBOFinalColor, m_pFBOFinalColorNext, m_pFBOStartColor, m_pFBOStartColorNext);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_targetBinder->Bind(m_pFBORayStart,m_pFBORayStartNext);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
#endif
  m_targetBinder->Unbind();


  //bind the entrance buffer as target
  m_targetBinder->Bind(m_pFBORayStart);

  //! \todo add the statemanager here !
  glCullFace(GL_BACK);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);


  m_programRenderFrontFacesNearPlane->Enable();
  m_programRenderFrontFacesNearPlane->Set("mEyeToModel", m_EyeToModelMatrix);
  m_programRenderFrontFacesNearPlane->Set("mInvProjection", m_projection.inverse());
  
  
  m_nearPlane->paint();

  m_programRenderFrontFaces->Set("mEyeToModel", m_EyeToModelMatrix);
  m_programRenderFrontFaces->Set("mModelView", m_modelView);
  m_programRenderFrontFaces->Set("mModelViewProjection", m_modelView*m_projection);

  m_bbBox->paint();

  //reset opengl
  m_programRenderFrontFacesNearPlane->Disable();
  m_targetBinder->Unbind();
  m_pFBORayStart->FinishWrite();
}

                                  
void GridLeaper::raycast(){
  m_targetBinder->Bind(m_pFBORayStartNext);
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);

#ifdef GLGRIDLEAPER_DEBUGVIEW
  m_pTargetBinder->Bind(m_pFBOFinalColorNext,
                        m_pFBOStartColorNext,
                        m_pFBORayStartNext,
                        m_pFBODebugNext);
#else
  m_targetBinder->Bind(m_pFBOFinalColorNext,
                       m_pFBOStartColorNext,
                       m_pFBORayStartNext);
#endif

  selectShader();
  setupRaycastShader();

  //set states
  glCullFace(GL_FRONT);

  // render
  m_bbBox->paint();
}

void GridLeaper::compose(){
  m_targetBinder->Bind(m_resultBuffer);

  glCullFace(GL_BACK);
  m_programCompose->Enable();
  m_programCompose->SetTexture2D("compose", m_pFBOFinalColor->GetTextureHandle(), 0);
  m_programCompose->Set("ColorOne", m_backgroundColors.colorOne);
  m_programCompose->Set("ColorTwo", m_backgroundColors.colorTwo);

  m_nearPlane->paint();
  m_programCompose->Disable();

  const uint32_t width = m_visStream->getStreamingParams().getResX();
  const uint32_t height = m_visStream->getStreamingParams().getResY();

  m_resultBuffer->ReadBackPixels(0, 0, width, height, m_bufferData.data());

  m_targetBinder->Unbind();

  auto f1 = Frame::createFromRaw(m_bufferData.data(), m_bufferData.size() * 4 * sizeof(uint8_t));
  getVisStream()->put(std::move(f1));
}


void GridLeaper::selectShader(){
  if (m_renderMode == IRenderer::ERenderMode::RM_1DTRANS){
    if (!m_enableLighting){
      m_activeShaderProgram = m_programRayCast1D;
    }
    else{
      if (m_semantic == IIO::Semantic::Color)
        m_activeShaderProgram = m_programRayCast1DColor;
      else
        m_activeShaderProgram = m_programRayCast1DLighting;
    }
  }
  else if (m_renderMode == IRenderer::ERenderMode::RM_ISOSURFACE){
    if (m_semantic == IIO::Semantic::Color)
      m_activeShaderProgram = m_programRayCastISOColor;
    else
      m_activeShaderProgram = m_programRayCastISOLighting;
  }
}

void GridLeaper::setupRaycastShader() {
  
  m_volumePool->enable(m_fLODFactor,
                        m_vExtend,
                        m_vScale,
                        m_activeShaderProgram); // bound to 3 and 4
  m_hashTable->enable(); // bound to 5

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
    FLOATVECTOR3 invScale = 1.0f/m_vScale;
    m_activeShaderProgram->Set("vDomainScale",invScale);
    m_activeShaderProgram->Set("mModelToEye",m_EyeToModelMatrix.inverse());
    m_activeShaderProgram->Set("mModelViewIT", m_modelView.inverse(), true);
  } else {
    m_activeShaderProgram->Set("fTransScale",m_1DTFScale);
    if (m_renderMode == IRenderer::ERenderMode::RM_2DTRANS) {
      // m_activeShaderProgram->Set("fGradientScale",m_2DTFScale);
    }
    
    if (m_enableLighting) {
      Vec3ui8 a = m_lightingColors.ambient.xyz()*m_lightingColors.ambient.w;
      Vec3ui8 d = m_lightingColors.diffuse.xyz()*m_lightingColors.diffuse.w;
      Vec3ui8 s = m_lightingColors.specular.xyz()*m_lightingColors.specular.w;
      
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

  
  
  
  /*
  
  
  Mat4f mvp = m_modelView*m_projection;
  Mat4f mvInverse = m_modelView.inverse();

  m_activeShaderProgram->Enable();

  m_activeShaderProgram->Set("mModelViewProjection", mvp);
  m_activeShaderProgram->Set("mModelView", m_modelView);

  m_activeShaderProgram->Set("sampleRateModifier", getSampleRateModifier());
  m_activeShaderProgram->Set("mEyeToModel", mvInverse);
  m_activeShaderProgram->Set("mEyeToModel2", m_view.inverse());
  m_activeShaderProgram->Set("vDomainScale", m_IODomainSize);

  //setup textures
  m_activeShaderProgram->SetTexture2D("rayStartPoint", m_pFBORayStart->GetTextureHandle(), 0);
  m_activeShaderProgram->SetTexture2D("rayStartColor", m_pFBOStartColor->GetTextureHandle(), 1);


  //todo FIX ASPECT RATIO AND EXTEND? WHERE IN IIO?
  m_volumePool->enable(m_fLODFactor,
                       Vec3f(0,0,0),       //todo vExtend
                       Vec3f(0,0,0),     //todo vAspect
                       m_activeShaderProgram);
  m_hashTable->enable();

  //todo why do we divide xyz by w?
  if (m_enableLighting || m_renderMode == IRenderer::ERenderMode::RM_ISOSURFACE) {
    Vec3ui8 a = m_lightingColors.ambient.xyz()*m_lightingColors.ambient.w;
    Vec3ui8 d = m_lightingColors.diffuse.xyz()*m_lightingColors.diffuse.w;
    Vec3ui8 s = m_lightingColors.specular.xyz()*m_lightingColors.specular.w;

    m_activeShaderProgram->Set("vLightAmbient", a);
    m_activeShaderProgram->Set("vLightDiffuse", d);
    m_activeShaderProgram->Set("vLightSpecular", s);
    m_activeShaderProgram->Set("vModelSpaceLightDir", m_lightDirection);
    m_activeShaderProgram->Set("vModelSpaceEyePos", Vec3f(0,0,10));
  }


  if (m_renderMode == IRenderer::ERenderMode::RM_ISOSURFACE) {
    m_activeShaderProgram->Set("fIsoval", getIsoValue(0));

    m_activeShaderProgram->Set("mModelToEye", m_modelView);
    m_activeShaderProgram->Set("mModelViewIT", m_modelView.inverse());
  }
  else {
    m_activeShaderProgram->Set("fTransScale", 1.0f);
    m_activeShaderProgram->SetTexture1D("transferFunction",
                                        m_texTransferFunc->GetGLID(),
                                        2);
  }

#ifdef GLGRIDLEAPER_DEBUGVIEW
  m_activeShaderProgram->SetTexture2D("debugColor", m_pFBODebug->GetTextureHandle(), 6);
#endif
   
   */
}

void GridLeaper::swapToNextBuffer(){
  std::swap(m_pFBORayStart, m_pFBORayStartNext);
  std::swap(m_pFBOStartColor, m_pFBOStartColorNext);
  std::swap(m_pFBOFinalColor, m_pFBOFinalColorNext);
#ifdef GLGRIDLEAPER_DEBUGVIEW
  std::swap(m_pFBODebug, m_pFBODebugNext);
#endif
}

Core::Math::Vec3ui GridLeaper::calculateVolumePoolSize(const uint64_t GPUMemorySizeInByte,const uint64_t usedMemory){
  uint64_t elementSize = 0;

  // TODO: replace this switch-madness with more reasonable code
  switch(m_semantic){
    case IIO::Semantic::Scalar : elementSize = 1; break;
    case IIO::Semantic::Vector : elementSize = 3; break;
    case IIO::Semantic::Color  : elementSize = 4; break;
  }
  switch(m_type){
    case  IIO::ValueType::T_INT8 :
    case  IIO::ValueType::T_UINT8 : elementSize *= 8; break;
    case  IIO::ValueType::T_INT16 :
    case  IIO::ValueType::T_UINT16 : elementSize *= 16; break;
    case  IIO::ValueType::T_INT32 :
    case  IIO::ValueType::T_UINT32 :
    case  IIO::ValueType::T_FLOAT : elementSize *= 32; break;
    case  IIO::ValueType::T_INT64 :
    case  IIO::ValueType::T_UINT64 :
    case  IIO::ValueType::T_DOUBLE : elementSize *= 64; break;
  }

  const Vec3ui vMaxBS = Vec3ui(m_io->getMaxUsedBrickSizes());

  // Compute the pool size as a (almost) cubed texture that fits
  // into the user specified GPU mem, is a multiple of the bricksize
  // and is no bigger than what OpenGL tells us is possible

  //Fake workaround for first :x \todo fix this
  uint64_t GPUmemoryInByte = GPUMemorySizeInByte;

  GLint iMaxVolumeDims;
  glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &iMaxVolumeDims);
  const uint64_t iMaxGPUMem = GPUmemoryInByte - usedMemory;

  const uint64_t iMaxVoxelCount = iMaxGPUMem / (elementSize / 8);
  const uint64_t r3Voxels = uint64_t(pow(double(iMaxVoxelCount), 1.0 / 3.0));
  Vec3ui maxBricksForGPU;


  // round the starting input size (r3Voxels) to the closest multiple brick size
  // to guarantee as cubic as possible volume pools and to better fill the
  // available amount of memory
  // (e.g. it creates a 1024x512x1536 pool instead of a 512x2048x512 pool for
  // a brick size of 512^3 and 3.4 GB available memory)
  uint64_t multipleOfBrickSizeThatFitsInX = uint64_t(((float)r3Voxels / vMaxBS.x) + 0.5f) *vMaxBS.x;
  if (multipleOfBrickSizeThatFitsInX > uint64_t(iMaxVolumeDims))
    multipleOfBrickSizeThatFitsInX = (iMaxVolumeDims / vMaxBS.x)*vMaxBS.x;
  maxBricksForGPU.x = (uint32_t)multipleOfBrickSizeThatFitsInX;

  uint64_t multipleOfBrickSizeThatFitsInY = ((iMaxVoxelCount / (maxBricksForGPU.x*maxBricksForGPU.x)) / vMaxBS.y)*vMaxBS.y;
  if (multipleOfBrickSizeThatFitsInY > uint64_t(iMaxVolumeDims))
    multipleOfBrickSizeThatFitsInY = (iMaxVolumeDims / vMaxBS.y)*vMaxBS.y;
  maxBricksForGPU.y = (uint32_t)multipleOfBrickSizeThatFitsInY;

  uint64_t multipleOfBrickSizeThatFitsInZ = ((iMaxVoxelCount / (maxBricksForGPU.x*maxBricksForGPU.y)) / vMaxBS.z)*vMaxBS.z;
  if (multipleOfBrickSizeThatFitsInZ > uint64_t(iMaxVolumeDims))
    multipleOfBrickSizeThatFitsInZ = (iMaxVolumeDims / vMaxBS.z)*vMaxBS.z;
  maxBricksForGPU.z = (uint32_t)multipleOfBrickSizeThatFitsInZ;

  // the max brick layout required by the dataset
  const uint64_t iMaxBrickCount = m_io->getTotalBrickCount(m_activeModality);
  const uint64_t r3Bricks = uint64_t(pow(double(iMaxBrickCount), 1.0 / 3.0));
  Vec3ui64 maxBricksForDataset;

  multipleOfBrickSizeThatFitsInX = vMaxBS.x*r3Bricks;
  if (multipleOfBrickSizeThatFitsInX > uint64_t(iMaxVolumeDims))
    multipleOfBrickSizeThatFitsInX = (iMaxVolumeDims / vMaxBS.x)*vMaxBS.x;
  maxBricksForDataset.x = (uint32_t)multipleOfBrickSizeThatFitsInX;

  multipleOfBrickSizeThatFitsInY = vMaxBS.y*uint64_t(ceil(float(iMaxBrickCount) / ((maxBricksForDataset.x / vMaxBS.x) * (maxBricksForDataset.x / vMaxBS.x))));
  if (multipleOfBrickSizeThatFitsInY > uint64_t(iMaxVolumeDims))
    multipleOfBrickSizeThatFitsInY = (iMaxVolumeDims / vMaxBS.y)*vMaxBS.y;
  maxBricksForDataset.y = (uint32_t)multipleOfBrickSizeThatFitsInY;

  multipleOfBrickSizeThatFitsInZ = vMaxBS.z*uint64_t(ceil(float(iMaxBrickCount) / ((maxBricksForDataset.x / vMaxBS.x) * (maxBricksForDataset.y / vMaxBS.y))));
  if (multipleOfBrickSizeThatFitsInZ > uint64_t(iMaxVolumeDims))
    multipleOfBrickSizeThatFitsInZ = (iMaxVolumeDims / vMaxBS.z)*vMaxBS.z;
  maxBricksForDataset.z = (uint32_t)multipleOfBrickSizeThatFitsInZ;

  // now use the smaller of the two layouts, normally that
  // would be the maxBricksForGPU but for small datasets that
  // can be rendered entirely in-core we may need less space
  const Vec3ui poolSize = (maxBricksForDataset.volume() < Vec3ui64(maxBricksForGPU).volume())
  ? Vec3ui(maxBricksForDataset)
  : maxBricksForGPU;

  return poolSize;
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
        vEmptyBrickCount = m_volumePool->RecomputeVisibility(m_visibilityState,*m_io, m_activeTimestep, bForceSynchronousUpdate);
      }
      break; }
    case ERenderMode::RM_2DTRANS: {
      /* double const fMin = double(m_p2DTrans->GetNonZeroLimits().x) * fRescaleFactor;
       double const fMax = double(m_p2DTrans->GetNonZeroLimits().y) * fRescaleFactor;
       double const fMinGradient = double(m_p2DTrans->GetNonZeroLimits().z);
       double const fMaxGradient = double(m_p2DTrans->GetNonZeroLimits().w);
       if (m_VisibilityState->NeedsUpdate(fMin, fMax, fMinGradient, fMaxGradient) ||
       bForceSynchronousUpdate) {
       vEmptyBrickCount = m_volumePool->RecomputeVisibility(*(m_VisibilityState.get()), m_iTimestep, bForceSynchronousUpdate);
       }*/
      break; }
    case ERenderMode::RM_ISOSURFACE: {
      double const fIsoValue = m_isoValue[0];
      if (m_visibilityState.needsUpdate(fIsoValue) ||
          bForceSynchronousUpdate) {
        vEmptyBrickCount = m_volumePool->RecomputeVisibility(m_visibilityState,*m_io, m_activeTimestep, bForceSynchronousUpdate);
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
      return 0;
    }

    LINFOC("GridLeaper", "available VRAM in kb: " << freememory);
  }
  return freememory;
}


void GridLeaper::set1DTransferFunction(const TransferFunction1D& tf) {
  if ( m_texTransferFunc ) {
    m_texTransferFunc->SetData(0, tf.getSize(), tf.getRAWData().data());
  }
  AbstractRenderer::set1DTransferFunction(tf);
}

/*
void GridLeaper::set2DTransferFunction(const TransferFunction2D& tf) {
  if ( m_texTransferFunc2D ) {
    m_texTransferFunc2D->SetData(0, tf.getSize(), tf.getRAWData().data());
  }

  AbstractRenderer::set2DTransferFunction(tf);
}
 */

bool GridLeaper::isIdle() {
  return m_isIdle;
}


void GridLeaper::enableClipping(bool enable) {
  AbstractRenderer::enableClipping(enable);
  // TODO: update m_bbBox
}

void GridLeaper::setClipVolume(const Core::Math::Vec3f& minValues,
                               const Core::Math::Vec3f& maxValues)  {
  AbstractRenderer::setClipVolume(minValues, maxValues);
  // TODO: update m_bbBox
}


bool GridLeaper::proceedRendering() {
  if (isIdle()) {
    return false;
  } else {
    paintInternal(IRenderer::PaintLevel::PL_CONTINUE);
    return true;
  }
}
