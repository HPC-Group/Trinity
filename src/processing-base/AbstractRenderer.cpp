#include "AbstractRenderer.h"

#include "common/TrinityError.h"
#include "common/VisStream.h"

using namespace trinity;
using namespace Core::Math;


static const Core::Math::Vec3f s_vEye(0,0,1.6f);
static const Core::Math::Vec3f s_vAt(0,0,0);
static const Core::Math::Vec3f s_vUp(0,1,0);
static const float s_fFOV = 50.0f;
static const float s_fZNear = 0.01f;
static const float s_fZFar = 1000.0f;


AbstractRenderer::AbstractRenderer(std::shared_ptr<VisStream> stream,
                                   std::unique_ptr<IIO> ioSession)  :
IRenderer(stream, std::move(ioSession)),
m_bPaitingActive(false)
{
  initValueDefaults();
}

void AbstractRenderer::calculate1DTFDerived() {
  Vec2f range = m_io->getRange(m_activeModality);

  float maxValue = (range.x > range.y) ? m_1Dtf.getSize() : range.y;

  m_1DTFScale = 1;
  switch(m_type){
    case  IIO::ValueType::T_INT8 :
      m_1DTFScale = float(std::numeric_limits<int8_t>::max()/maxValue);
      break;
    case  IIO::ValueType::T_UINT8 :
      m_1DTFScale = float(std::numeric_limits<uint8_t>::max()/maxValue);
      break;
    case  IIO::ValueType::T_INT16 :
      m_1DTFScale = float(std::numeric_limits<int16_t>::max()/maxValue);
      break;
    case  IIO::ValueType::T_UINT16 :
      m_1DTFScale = float(std::numeric_limits<uint16_t>::max()/maxValue);
      break;
    case  IIO::ValueType::T_INT32 :
      m_1DTFScale = float(std::numeric_limits<int32_t>::max()/maxValue);
      break;
    case  IIO::ValueType::T_UINT32 :
      m_1DTFScale = float(std::numeric_limits<uint32_t>::max()/maxValue);
      break;
    case  IIO::ValueType::T_FLOAT :
      m_1DTFScale = float(std::numeric_limits<float>::max()/maxValue);
      break;
    case  IIO::ValueType::T_INT64 :
      m_1DTFScale = float(std::numeric_limits<int64_t>::max()/maxValue);
      break;
    case  IIO::ValueType::T_UINT64 :
      m_1DTFScale = float(std::numeric_limits<uint64_t>::max()/maxValue);
      break;
    case  IIO::ValueType::T_DOUBLE :
      m_1DTFScale = float(std::numeric_limits<double>::max()/maxValue);
      break;
  }
}

void AbstractRenderer::calculateDerived() {
  m_type = m_io->getType(m_activeModality);
  m_semantic = m_io->getSemantic(m_activeModality);
  
  calculate1DTFDerived();

  /*
  float m_2DTFScale = (m_pDataset->MaxGradientMagnitude() == 0) ?
  1.0f : 1.0f/m_pDataset->MaxGradientMagnitude();
  */
  
  Core::Math::Vec3ui64 vDomainSize = m_io->getDomainSize(0,0);
  m_vScale = Core::Math::Vec3f(m_io->getDomainScale(0));
  
  m_vExtend = Core::Math::Vec3f(vDomainSize) * m_vScale;
  m_vExtend /= m_vExtend.maxVal();
  m_vScale /= m_vScale.minVal();
}


void AbstractRenderer::initValueDefaults(){

  m_renderMode = ERenderMode::RM_1DTRANS;
  m_activeModality = 0;
  m_activeTimestep = 0;
  m_1Dtf = m_io->getDefault1DTransferFunction(0);
  //m_2Dtf = m_io->getDefault2DTransferFunction(0);   // TODO: 2D TF
  m_isoValue[0] = 0.0f;
  m_isoValue[1] = 0.0f;
  m_isoValueColor[0] = Vec3ui8(255,0,0);
  m_isoValueColor[1] = Vec3ui8(0,255,0);
  m_clearViewWindowPos = Vec2f(0.5f,0.5f);
  m_clearViewRadius = 0.25f;
  m_clearBorderSize = 0.05f;
  m_enableLighting = true;
  m_lightingColors = PhongColorTriple{
    Vec4ui8(25,25,25,255),
    Vec4ui8(255,255,255,255),
    Vec4ui8(255,255,255,255)
  };
  m_lightDirection = Vec3f(0.0f,0.0f,-1.0f);
  m_fSampleRateModifier = 1.0f;
  m_BBoxMode = BBoxMode::BBM_NONE;
  m_RendererSpecials = std::vector<uint64_t>();
  m_backgroundColors = BackgroundColors(Vec3ui8(0,0,0),Vec3ui8(0,0,0));
  m_enableClipping = false;
  m_clipVolumeMin = Vec3f(0.0f,0.0f,0.0f);
  m_clipVolumeMax = Vec3f(1.0f,1.0f,1.0f);
  m_viewAngle = s_fFOV;
  m_zNear = s_fZNear;
  m_zFar = s_fZFar;
  m_eyePos = s_vEye;

  calculateDerived();

  resetCamera();
  resetScene();
  recomputeProjectionMatrix();
}

void AbstractRenderer::setRenderMode(ERenderMode mode){
  if (!supportsRenderMode(mode)) {
    throw TrinityError("unsuported rendermode requested", __FILE__, __LINE__);
  }
  m_renderMode = mode;
  paint(IRenderer::PaintLevel::PL_REDRAW_VISIBILITY_CHANGE);
}

// by default we support only 1D TF
// this method should be overriden by the actual renderer
bool AbstractRenderer::supportsRenderMode(ERenderMode mode){
  return mode == ERenderMode::RM_1DTRANS;
}

uint64_t AbstractRenderer::getModalityCount() const {
  return m_io->getModalityCount();
}

void AbstractRenderer::setActiveModality(uint64_t modality) {
  m_activeModality = modality;
  paint();
}

uint64_t AbstractRenderer::getActiveModality() const {
  return m_activeModality;
}

uint64_t AbstractRenderer::getTimestepCount() const {
  return m_io->getNumberOfTimesteps();
}

void AbstractRenderer::setActiveTimestep(uint64_t timestep) {
  m_activeTimestep = timestep;
  paint();
}

uint64_t AbstractRenderer::getActiveTimestep() const {
  return m_activeTimestep;
}

// 1D TF
void AbstractRenderer::set1DTransferFunction(const TransferFunction1D& tf){
  if  (! (m_1Dtf == tf)) {
    m_1Dtf = tf;
    calculate1DTFDerived();
    paint(IRenderer::PaintLevel::PL_REDRAW_VISIBILITY_CHANGE);
  } else {
    paint(IRenderer::PaintLevel::PL_RECOMPOSE);
  }
}

TransferFunction1D
AbstractRenderer::getDefault1DTransferFunction(uint64_t index) const {
  return m_io->getDefault1DTransferFunction(index);
}

uint64_t AbstractRenderer::getDefault1DTransferFunctionCount() const {
  return m_io->getDefault1DTransferFunctionCount();
}

std::vector<uint64_t> AbstractRenderer::get1DHistogram() const {
  return m_io->get1DHistogram();
}

/*
 // TODO: 2D TF

 void AbstractRenderer::set2DTransferFunction(const TransferFunction2D& tf){
   if  (! m_2Dtf == tf) {
     m_2Dtf = tf;
     calculate"DTFDerived();
     paint(IRenderer::PaintLevel::PL_REDRAW_VISIBILITY_CHANGE);
   } else {
     paint(IRenderer::PaintLevel::PL_RECOMPOSE);
   }
 }

 TransferFunction2D
 AbstractRenderer::getDefaul2DTransferFunction(uint64_t index) const{
   return m_io->getDefaul21DTransferFunction(index);
 }

 const uint64_t AbstractRenderer::getDefault2DTransferFunctionCount() const{
   return m_io->getDefault2DTransferFunctionCount();
 }

 std::vector<uint64_t> AbstractRenderer::get2DHistogram() const {
   return m_io->get2DHistogram();
 }
 */

// ISO-SURFACE FUNCTIONS

void AbstractRenderer::setIsoValue(uint8_t surfaceIndex, float fIsoValue) {
  if (surfaceIndex >= m_isoValue.size()) {
    throw TrinityError("invalid surfaceIndex", __FILE__, __LINE__);
  }

  /* original jens
  if (m_isoValue[surfaceIndex] != fIsoValue) {
    m_isoValue[surfaceIndex] = fIsoValue;
    paint(IRenderer::PaintLevel::PL_REDRAW_VISIBILITY_CHANGE);
  } else {
    paint(IRenderer::PaintLevel::PL_RECOMPOSE);
  }
  */

  // andrey, otherwise simplerenderer won't work
  if (true) {
	  m_isoValue[surfaceIndex] = fIsoValue;
	  paint(IRenderer::PaintLevel::PL_REDRAW_VISIBILITY_CHANGE);
  }
}

float AbstractRenderer::getIsoValue(uint8_t surfaceIndex) const {
  if (surfaceIndex >= m_isoValue.size()) {
    throw TrinityError("invalid surfaceIndex", __FILE__, __LINE__);
  }

  return m_isoValue[surfaceIndex];
}

void AbstractRenderer::setIsosurfaceColor(uint8_t surfaceIndex,
                                          const Vec3ui8& vColor){
  if (surfaceIndex >= m_isoValue.size()) {
    throw TrinityError("invalid surfaceIndex", __FILE__, __LINE__);
  }

  m_isoValueColor[surfaceIndex] = vColor;
  paint(IRenderer::PaintLevel::PL_RECOMPOSE);
}

Vec3ui8 AbstractRenderer::getIsosurfaceColor(uint8_t surfaceIndex) const{
  if (surfaceIndex >= m_isoValue.size()) {
    throw TrinityError("invalid surfaceIndex", __FILE__, __LINE__);
  }

  return m_isoValueColor[surfaceIndex];
}

Vec2f AbstractRenderer::getRange(uint64_t modality) const {
  return m_io->getRange(modality);
}

// CLEARVIEW FUNCTIONS
void AbstractRenderer::setClearViewPosition(const Vec2f& vNormalizedWindowPos) {
  m_clearViewWindowPos = vNormalizedWindowPos;
  paint(IRenderer::PaintLevel::PL_RECOMPOSE);
}

void AbstractRenderer::setClearViewRadius(float radius) {
  m_clearViewRadius = radius;
  paint(IRenderer::PaintLevel::PL_RECOMPOSE);
}

void AbstractRenderer::setClearBorderSize(float borderSize) {
  m_clearBorderSize = borderSize;
  paint(IRenderer::PaintLevel::PL_RECOMPOSE);
}

// LIGTHING FUNCTIONS
void AbstractRenderer::enableLighting(bool enable) {
  m_enableLighting = enable;
  paint();
}

void AbstractRenderer::setLightingColors(const PhongColorTriple& colors) {
  m_lightingColors = colors;
  paint();
}

IRenderer::PhongColorTriple AbstractRenderer::getLightingColors() const {
  return m_lightingColors;
}

void AbstractRenderer::setLightDirection(const Vec3f& direction){
  m_lightDirection = direction;
  paint();
}

Vec3f AbstractRenderer::getLightDirection() const {
  return m_lightDirection;
}

// SAMPLE RATE FUNCTIONS
void AbstractRenderer::setSampleRateModifier(float fSampleRateModifier) {
  m_fSampleRateModifier = fSampleRateModifier;
  paint();
}

float AbstractRenderer::getSampleRateModifier() const {
  return m_fSampleRateModifier;
}

// render parameters
void AbstractRenderer::setBoundingBoxMode(BBoxMode mode) {
  m_BBoxMode = mode;
  paint();
}

IRenderer::BBoxMode AbstractRenderer::getBoundingBoxMode() const {
  return m_BBoxMode;
}

void AbstractRenderer::setRendererSpecials(const std::vector<uint64_t>& params){
  m_RendererSpecials = params;
  paint();
}

std::vector<uint64_t> AbstractRenderer::getRendererSpecials() const {
  return m_RendererSpecials;
}

void AbstractRenderer::setBackgroundColors(const BackgroundColors& colors) {
  m_backgroundColors = colors;
  paint();
}

IRenderer::BackgroundColors AbstractRenderer::getBackgroundColors() const {
  return m_backgroundColors;
}

// CLIPPING

void AbstractRenderer::performClipping() {}

void AbstractRenderer::enableClipping(bool enable) {
  m_enableClipping = enable;
  performClipping();
  paint();
}

void AbstractRenderer::setClipVolume(const Vec3f& minValues,
                                     const Vec3f& maxValues) {

  // make sure the minimal vaues are stored in m_clipVolumeMin ...
  m_clipVolumeMin.x = std::min(minValues.x, maxValues.x);
  m_clipVolumeMin.y = std::min(minValues.y, maxValues.y);
  m_clipVolumeMin.z = std::min(minValues.z, maxValues.z);

  // ... and the maximal vaues are stored in m_clipVolumeMax
  m_clipVolumeMax.x = std::max(minValues.x, maxValues.x);
  m_clipVolumeMax.y = std::max(minValues.y, maxValues.y);
  m_clipVolumeMax.z = std::max(minValues.z, maxValues.z);
  
  // now make sure the values are between 0 and 1
  m_clipVolumeMin.StoreMin(Vec3f(1.0f,1.0f,1.0f));
  m_clipVolumeMin.StoreMax(Vec3f(0.0f,0.0f,0.0f));
  
  m_clipVolumeMax.StoreMin(Vec3f(1.0f,1.0f,1.0f));
  m_clipVolumeMax.StoreMax(Vec3f(0.0f,0.0f,0.0f));
  
  performClipping();
  paint();
}

// TRANSFORMATION
void AbstractRenderer::setViewParameters(float angle, float znear, float zfar) {
  m_viewAngle = angle;
  m_zNear = znear;
  m_zFar = zfar;

  recomputeProjectionMatrix();
  paint();
}

void AbstractRenderer::setUserViewMatrix(Core::Math::Mat4f m)
{
  m_userViewMatrix = m;
  recomputeModelViewMatrix();
}

void AbstractRenderer::setUserWorldMatrix(Core::Math::Mat4f m)
{
  m_userWorldMatrix = m;
  recomputeModelViewMatrix();
}

void AbstractRenderer::recomputeModelViewMatrix() {
  m_view.BuildLookAt(m_eyePos, s_vAt, s_vUp);
  
  m_view = m_userViewMatrix*m_camRotation*m_camTranslation*m_camZoom*m_view;
  m_model = m_userWorldMatrix*m_rotation*m_translation*m_scale;
  m_modelView = m_model*m_view;
}

void AbstractRenderer::recomputeProjectionMatrix() {
  const uint32_t width = m_visStream->getStreamingParams().getResX();
  const uint32_t height = m_visStream->getStreamingParams().getResY();

  m_projection.Perspective(m_viewAngle, (float)width/(float)height,
                           m_zNear, m_zFar);
}

void AbstractRenderer::rotateCamera(Vec3f rotation) {
  Mat4f rotX, rotY, rotZ;
  rotX.RotationX(rotation.x);
  rotY.RotationY(rotation.y);
  rotZ.RotationZ(rotation.z);
  m_camRotation = m_camRotation * rotX * rotY * rotZ;
  recomputeModelViewMatrix();
  paint();
}

void AbstractRenderer::moveCamera(Vec3f direction) {
  Mat4f trans;
  trans.Translation(direction);
  m_camTranslation = m_camTranslation * trans;
  recomputeModelViewMatrix();
  paint();
}

void AbstractRenderer::zoomCamera(float zoom) {
  Mat4f scale;
  scale.Scaling(zoom,zoom,zoom);
  m_camZoom = m_camZoom * scale;
  recomputeModelViewMatrix();
  paint();
}

void AbstractRenderer::rotateScene(Vec3f rotation) {
  Mat4f rotX, rotY, rotZ;
  rotX.RotationX(rotation.x);
  rotY.RotationY(rotation.y);
  rotZ.RotationZ(rotation.z);
  m_rotation = m_rotation * rotX * rotY * rotZ;
  recomputeModelViewMatrix();
  paint();
}

void AbstractRenderer::moveScene(Vec3f direction) {
  Mat4f trans;
  trans.Translation(direction);
  m_translation = m_translation * trans;
  recomputeModelViewMatrix();
  paint();
}

void AbstractRenderer::rescaleScene(float scale) {
  Mat4f matScale;
  matScale.Scaling(scale,scale,scale);
  m_scale = m_scale * matScale;
  recomputeModelViewMatrix();
  paint();
}


void AbstractRenderer::resizeFramebuffer() {
  recomputeProjectionMatrix();
}

void AbstractRenderer::resetCamera() {
  m_userViewMatrix = Mat4f();

  m_camRotation = Mat4f();
  m_camTranslation = Mat4f();
  m_camZoom = Mat4f();
  
  recomputeModelViewMatrix();
  paint();
}

void AbstractRenderer::resetScene() {
  m_userWorldMatrix = Mat4f();

  m_rotation = Mat4f();
  m_translation = Mat4f();
  m_scale = Mat4f();

  recomputeModelViewMatrix();
  paint();
}

void AbstractRenderer::startRendering() {
  m_bPaitingActive = true;
  paintInternal(IRenderer::PaintLevel::PL_REDRAW);
}

void AbstractRenderer::stopRendering() {
  m_bPaitingActive = false;
}

