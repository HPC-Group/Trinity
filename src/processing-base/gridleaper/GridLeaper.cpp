#include "GridLeaper.h"

#include "common/VisStream.h"
#include "opengl-base/OpenGLError.h"
#include "silverbullet/math/Vectors.h"

#include "mocca/log/LogManager.h"

#include <thread>
#include <vector>

using namespace trinity;
using namespace Core::Math;
using namespace std;


GridLeaper::GridLeaper(std::shared_ptr<VisStream> stream,
                       std::unique_ptr<IIO> ioSession) :
AbstractRenderer(stream, std::move(ioSession)),
m_texTransferFunc(nullptr),
m_texVolume(nullptr),
m_targetBinder(nullptr),
m_programRenderFrontFaces(nullptr),
m_programRenderFrontFacesNearPlane(nullptr),
m_programRayCast1D(nullptr),
m_programRayCast1DColor(nullptr),
m_programRayCast1DLighting(nullptr),
m_programRayCastISO(nullptr),
m_programRayCastISOLighting(nullptr),
m_programRayCastISOColor(nullptr),
m_currentShaderProgram(nullptr),
m_programCompose(nullptr),
m_programComposeColorDebugMix(nullptr),
m_programComposeColorDebugMixAlpha(nullptr),
m_programComposeClearViewIso(nullptr),
m_hashTable(nullptr),
m_volumePool(nullptr),
m_backfaceBuffer(nullptr),
m_resultBuffer(nullptr),
m_bbBox(nullptr),
m_nearPlane(nullptr),
m_context(nullptr),
m_VisibilityState()
{
}

void GridLeaper::deleteContext() {
    m_texTransferFunc = nullptr;
    m_texVolume = nullptr;
    m_targetBinder = nullptr;
    m_backfaceBuffer = nullptr;
    m_resultBuffer = nullptr;
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
    m_currentShaderProgram = nullptr;
    m_programCompose = nullptr;
    m_programComposeColorDebugMix = nullptr;
    m_programComposeColorDebugMixAlpha = nullptr;
    m_programComposeClearViewIso = nullptr;
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

    m_width = m_visStream->getStreamingParams().getResX();
    m_height = m_visStream->getStreamingParams().getResY();
    m_bufferData.resize(m_width * m_height);


    LINFO("(p) grid leaper: resolution: " + std::to_string(m_width) + " x " + std::to_string(m_height));

  initHashTable();

  loadFrameBuffers();
  loadShaders(GLVolumePool::MissingBrickStrategy::RequestAll); // guess we could use the "renderspecials" here
  loadGeometry();
  loadVolumeData();
  loadTransferFunction();

    m_targetBinder = mocca::make_unique<GLTargetBinder>();
    LINFO("(p) grid leaper created. OpenGL Version " << m_context->getVersion());
}

bool GridLeaper::loadShaders(GLVolumePool::MissingBrickStrategy brickStrategy) {
  vector<string> fs, vs;
  vs.push_back("ComposeVS.glsl");
  fs.push_back("ComposeFS.glsl");
  ShaderDescriptor sd(vs, fs);

  m_programCompose = mocca::make_unique<GLProgram>();
  m_programCompose->Load(sd);

  if (!m_programCompose->IsValid()) {
    LERROR("(p) invalid compose shader program");
    m_programCompose = nullptr;
    return false;
  }

  fs.clear();
  fs.push_back("ComposeFSColorDebug.glsl");
  sd = ShaderDescriptor(vs, fs);

  m_programComposeColorDebugMix = mocca::make_unique<GLProgram>();
  m_programComposeColorDebugMix->Load(sd);

  if (!m_programComposeColorDebugMix->IsValid()) {
    LERROR("(p) invalid compose color debug shader program");
    m_programComposeColorDebugMix = nullptr;
    return false;
  }

  fs.clear();
  fs.push_back("ComposeFSColorDebugAlpha.glsl");
  sd = ShaderDescriptor(vs, fs);

  m_programComposeColorDebugMixAlpha = mocca::make_unique<GLProgram>();
  m_programComposeColorDebugMixAlpha->Load(sd);

  if (!m_programComposeColorDebugMixAlpha->IsValid()) {
    LERROR("(p) invalid compose color debug alpha shader program");
    m_programComposeColorDebugMixAlpha = nullptr;
    return false;
  }

  fs.clear();
  fs.push_back("ComposeFS_CViso.glsl");
  sd = ShaderDescriptor(vs, fs);

  m_programComposeClearViewIso = mocca::make_unique<GLProgram>();
  m_programComposeClearViewIso->Load(sd);

  if (!m_programComposeClearViewIso->IsValid()) {
    LERROR("(p) invalid compose clear view shader program");
    m_programComposeClearViewIso = nullptr;
    return false;
  }

  fs.clear(); vs.clear();
  vs.push_back("CubeVertex.glsl");
  fs.push_back("CubeFragment.glsl");
  sd = ShaderDescriptor(vs, fs);

  m_programRenderFrontFaces = mocca::make_unique<GLProgram>();
  m_programRenderFrontFaces->Load(sd);

  if (!m_programRenderFrontFaces->IsValid()) {
    LERROR("(p) invalid cube shader program");
    m_programRenderFrontFaces = nullptr;
    return false;
  }

  fs.clear(); vs.clear();
  vs.push_back("NearPlaneVS.glsl");
  fs.push_back("NearPlaneFS.glsl");
  sd = ShaderDescriptor(vs, fs);

  m_programRenderFrontFacesNearPlane = mocca::make_unique<GLProgram>();
  m_programRenderFrontFacesNearPlane->Load(sd);

  if (!m_programRenderFrontFacesNearPlane->IsValid()) {
    LERROR("(p) invalid cube shader program");
    m_programRenderFrontFacesNearPlane = nullptr;
    return false;
  }


    // Load the traversal shaders
  const std::string poolFragment = m_volumePool->getShaderFragment(
      3, 4,
      GLVolumePool::MissingBrickStrategy(brickStrategy)
      );
  const std::string hashFragment = m_hashTable->getShaderFragment(5);

  vs.push_back("GLGridLeaper-entry-VS.glsl");
  fs.push_back("GLGridLeaper-blend.glsl");
  fs.push_back("GLGridLeaper-Method-1D.glsl");
  fs.push_back("Compositing.glsl");
  sd = ShaderDescriptor(vs, fs);
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
  fs.push_back("GLGridLeaper-blend.glsl");
  fs.push_back("GLGridLeaper-Method-1D-color.glsl");
  fs.push_back("Compositing.glsl");
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
  fs.push_back("GLGridLeaper-blend.glsl");
  fs.push_back("GLGridLeaper-Method-1D-L.glsl");
  fs.push_back("GLGridLeaper-GradientTools.glsl");
  fs.push_back("lighting.glsl");
  fs.push_back("Compositing.glsl");
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
  fs.push_back("GLGridLeaper-iso.glsl");
  fs.push_back("GLGridLeaper-Method-iso.glsl");
  fs.push_back("GLGridLeaper-GradientTools.glsl");
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
  fs.push_back("GLGridLeaper-iso.glsl");
  fs.push_back("GLGridLeaper-Method-iso-color.glsl");
  fs.push_back("GLGridLeaper-GradientTools.glsl");
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
  fs.push_back("GLGridLeaper-iso-lighting.glsl");
  fs.push_back("GLGridLeaper-Method-iso.glsl");
  fs.push_back("GLGridLeaper-GradientTools.glsl");
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

void GridLeaper::loadVolumeData() {
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

  BrickKey brickKey(0, 0, singleBrickLoD, 0);

  Core::Math::Vec3ui brickSize = m_io->getBrickVoxelCounts(brickKey);

  LINFO("(p) found suitable volume with single brick of size " << brickSize);


  std::vector<uint8_t> volume;

  // HACK:
  // getBrick should resize the vector, but for now we have to do this manually
  volume.resize(brickSize.volume());

  m_io->getBrick(brickKey, volume);

  if (volume.size() != brickSize.volume()) {
    LERROR("invalid volume data vector. size should be " <<
           brickSize.volume() << " but is " << volume.size());
    return;
  } else {
    LINFO("(p) volume size data ok");
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
  m_bbBox = mocca::make_unique<GLVolumeBox>();
  m_nearPlane = mocca::make_unique<GLRenderPlane>(Vec2ui(m_width,m_height));
}

void GridLeaper::loadFrameBuffers() {
  m_resultBuffer = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                                   GL_CLAMP_TO_EDGE,
                                                   m_width, m_height,
                                                   GL_RGBA, GL_RGBA,
                                                   GL_UNSIGNED_BYTE, true, 1);

  // TODO: check if we need depth (the true below)
  m_backfaceBuffer = std::make_shared<GLFBOTex>(GL_NEAREST, GL_NEAREST,
                                              GL_CLAMP_TO_EDGE,
                                              m_width, m_height,
                                              GL_RGBA, GL_RGBA,
                                              GL_FLOAT, true, 1);
}

void GridLeaper::initHashTable() {
    Vec3ui const finestBrickLayout(m_io->getBrickLayout(0, 0));

    unsigned ht_size = 509; // default taken out of old mastercontroller
    if (ht_size == 0){ // with default value this will never get executed !
        const float rmax = m_io->getMaxBrickSize().volume() / 32768.;
        unsigned ht_size = std::max<unsigned>(512, static_cast<unsigned>(509 / rmax));
    }
    LDEBUGC("GLGridLeaper" , "Using " << ht_size <<"-element hash table.");

    uint32_t rehashcount = 10; // default taken out of old mastercontroller
    m_hashTable = mocca::make_unique<GLHashTable>(finestBrickLayout, uint32_t(ht_size), rehashcount);
    m_hashTable->initGL();
}

void GridLeaper::initVolumePool(uint64_t gpuMemorySizeInByte) {
    Vec3ui64 size = m_io->getDomainSize(0,m_activeModality);

    //CalculateUsedGPUMemory();
    Vec3ui volumepoolsize = CalculateVolumePoolSize(gpuMemorySizeInByte,0);

    //! \todo volume size ? how ?
    m_volumePool =  mocca::make_unique<GLVolumePool>(volumepoolsize, *m_io,m_activeModality, GL_LINEAR);


    if (m_volumePool){
        // upload a brick that covers the entire domain to make sure have
        // something to render

        // find lowest LoD with only a single brick
        uint64_t singleBrickLoD = m_io->getLargestSingleBrickLOD(0);

        BrickKey brickKey(0, 0, singleBrickLoD, 0);

        m_volumePool->uploadFirstBrick(brickKey,*m_io);

        RecomputeBrickVisibility(false);
    }
}

void GridLeaper::paintInternal(PaintLevel paintlevel) {

  if (!m_context) {
    LERROR("(p) incomplete OpenGL initialization");
    return;
  }

  m_context->makeCurrent();


  auto f1 = Frame::createFromRaw(m_bufferData.data(),
                                 m_bufferData.size()*sizeof(Vec4ui8));
  getVisStream()->put(std::move(f1));
}


Core::Math::Vec3ui GridLeaper::CalculateVolumePoolSize(const uint64_t GPUMemorySizeInByte,const uint64_t usedMemory){
    IIO::ValueType type = m_io->getType(m_activeModality);
    IIO::Semantic semantic = m_io->getSemantic(m_activeModality);
    uint64_t elementSize = 0;
    switch(semantic){
        case IIO::Semantic::Scalar : elementSize = 1; break;
        case IIO::Semantic::Vector : elementSize = 3; break;
        case IIO::Semantic::Color  : elementSize = 4; break;
    }
    switch(type){
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
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, &iMaxVolumeDims);
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
    case RM_1DTRANS: {
                         double const fMin = double(m_1Dtf.getNonZeroLimits().x) * fRescaleFactor;
                         double const fMax = double(m_1Dtf.getNonZeroLimits().y) * fRescaleFactor;
                         if (m_VisibilityState.needsUpdate(fMin, fMax) ||
                             bForceSynchronousUpdate) {
                             vEmptyBrickCount = m_volumePool->RecomputeVisibility(m_VisibilityState,*m_io, m_activeTimestep, bForceSynchronousUpdate);
                         }
                         break; }
    case RM_2DTRANS: {
                         /* double const fMin = double(m_p2DTrans->GetNonZeroLimits().x) * fRescaleFactor;
                         double const fMax = double(m_p2DTrans->GetNonZeroLimits().y) * fRescaleFactor;
                         double const fMinGradient = double(m_p2DTrans->GetNonZeroLimits().z);
                         double const fMaxGradient = double(m_p2DTrans->GetNonZeroLimits().w);
                         if (m_VisibilityState->NeedsUpdate(fMin, fMax, fMinGradient, fMaxGradient) ||
                         bForceSynchronousUpdate) {
                         vEmptyBrickCount = m_pVolumePool->RecomputeVisibility(*(m_VisibilityState.get()), m_iTimestep, bForceSynchronousUpdate);
                         }*/
                         break; }
    case RM_ISOSURFACE: {
                            double const fIsoValue = m_isoValue[0];
                            if (m_VisibilityState.needsUpdate(fIsoValue) ||
                                bForceSynchronousUpdate) {
                                vEmptyBrickCount = m_volumePool->RecomputeVisibility(m_VisibilityState,*m_io, m_activeTimestep, bForceSynchronousUpdate);
                            }
                            break; }
    default:
        LERRORC("GLGridLeaper", "Unhandled rendering mode.");
        break;
    }
    return vEmptyBrickCount;
}
