#include "AbstractRenderer.h"

#include "common/TrinityError.h"
#include "common/VisStream.h"

using namespace trinity;
using namespace Core::Math;

AbstractRenderer::AbstractRenderer(std::shared_ptr<VisStream> stream,
                                   std::unique_ptr<IIO> ioSession)  :
IRenderer(stream, std::move(ioSession)),
m_bPaitingActive(false)
{
  initValueDefaults();
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
    Vec4ui8(128,128,128,255),
    Vec4ui8(255,255,255,255)
  };
  m_lightDirection = Vec3f(0.0f,0.0f,1.0f);
  m_fSampleRateModifier = 1.0f;
  m_BBoxMode = BBoxMode::BBM_NONE;
  m_RendererSpecials = std::vector<uint64_t>();
  m_backgroundColors = std::make_pair(Vec3ui8(0,0,0),Vec3ui8(0,0,0));
  m_enableClipping = false;
  m_clipVolumeMin = Vec3f(0.0f,0.0f,0.0f);
  m_clipVolumeMax = Vec3f(1.0f,1.0f,1.0f);
  m_viewAngle = 45;
  m_zNear = 0.01f;
  m_zFar = 1000.0f;

  recomputeProjectionMatrix();
  resetCamera();
  resetObject();
  
  m_bPaitingActive = true;  // TODO change this to false after inital testing

}

void AbstractRenderer::setRenderMode(ERenderMode mode){
  if (!supportsRenderMode(mode)) {
    throw TrinityError("unsuported rendermode requested", __FILE__, __LINE__);
  }
  m_renderMode = mode;
  paint();
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
  m_1Dtf = tf;
  paint();
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
   m_2Dtf = tf;
   paint();
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

  m_isoValue[surfaceIndex] = fIsoValue;
  paint();
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

void AbstractRenderer::setBackgroundColors(std::pair<Vec3ui8, Vec3ui8> colors) {
  m_backgroundColors = colors;
  paint();
}

std::pair<Vec3ui8, Vec3ui8> AbstractRenderer::getBackgroundColors() const {
  return m_backgroundColors;
}

// CLIPPING

void AbstractRenderer::enableClipping(bool enable) {
  m_enableClipping = enable;
  paint();
}

void AbstractRenderer::setClipVolume(const Vec3f& minValues,
                                     const Vec3f& maxValues) {
  m_clipVolumeMin = minValues;
  m_clipVolumeMax = maxValues;
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
  m_view = m_view * rotX * rotY * rotZ;
  paint();
}

void AbstractRenderer::moveCamera(Vec3f direction) {
  Mat4f trans;
  trans.Translation(direction);
  m_view = m_view * trans;
  paint();
}

void AbstractRenderer::zoomCamera(float zoom) {
  Mat4f scale;
  scale.Scaling(zoom,zoom,zoom);
  m_view = m_view * scale;
  paint();
}

void AbstractRenderer::rotateScene(Vec3f rotation) {
  Mat4f rotX, rotY, rotZ;
  rotX.RotationX(rotation.x);
  rotY.RotationY(rotation.y);
  rotZ.RotationZ(rotation.z);
  m_model = m_model * rotX * rotY * rotZ;
  paint();
}

void AbstractRenderer::moveScene(Vec3f direction) {
  Mat4f trans;
  trans.Translation(direction);
  m_model = m_model * trans;
  paint();
}

void AbstractRenderer::rescaleScene(float scale) {
  Mat4f matScale;
  matScale.Scaling(scale,scale,scale);
  m_model = m_model * matScale;
  paint();
}


void AbstractRenderer::resizeFramebuffer() {
  recomputeProjectionMatrix();
}

void AbstractRenderer::resetCamera() {
  /*
   // TODO
   GL_CHECK(glViewport(0, 0, width, height));
   */
  
  m_view.BuildLookAt(Vec3f(0, 0, 3), Vec3f(0, 0, 0), Vec3f(0, 1, 0));
  paint();
}

void AbstractRenderer::resetObject() {
  m_model = Mat4f();
  paint();
}

void AbstractRenderer::startRendering() {
  m_bPaitingActive = true;
  paintInternal(IRenderer::PaintLevel::PL_REDRAW);
}

void AbstractRenderer::stopRendering() {
  m_bPaitingActive = false;
}

