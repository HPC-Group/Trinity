#include "AbstractRenderer.h"
#include "common/TrinityError.h"

using namespace trinity;

AbstractRenderer::AbstractRenderer(std::shared_ptr<VisStream> stream,
                                   std::unique_ptr<IIO> ioSession)  :
IRenderer(stream, std::move(ioSession))
{
  initValueDefaults();
}

void AbstractRenderer::initValueDefaults(){
  // TODO
  m_bPaitingActive = true;
}

void AbstractRenderer::setRenderMode(ERenderMode mode){
  if (!supportsRenderMode(mode)) {
    throw TrinityError("unsuported rendermode requested", __FILE__, __LINE__);
  }
  m_renderMode = mode;
  paint();
}

// by default we support nothing (needs to be overriden by the actual renderer
bool AbstractRenderer::supportsRenderMode(ERenderMode mode){
  return false;
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

uint64_t AbstractRenderer::getNumberOfTimesteps() const {
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

TransferFunction1D AbstractRenderer::getDefault1DTransferFunction(uint64_t index) const {
  return m_io->getDefault1DTransferFunction(index);
}

uint64_t AbstractRenderer::getDefault1DTransferFunctionCount() const {
  return m_io->getDefault1DTransferFunctionCount();
}

std::vector<uint64_t> AbstractRenderer::get1DHistogram() const {
  return m_io->get1DHistogram();
}

/*
 // 2D TF
 void AbstractRenderer::set2DTransferFunction(const TransferFunction2D& tf){
   m_2Dtf = tf;
   paint();
 }

 TransferFunction2D AbstractRenderer::getDefaul2DTransferFunction(uint64_t index) const{
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

void AbstractRenderer::setIsosurfaceColor(uint8_t surfaceIndex, const Core::Math::Vec3ui8& vColor){
  if (surfaceIndex >= m_isoValue.size()) {
    throw TrinityError("invalid surfaceIndex", __FILE__, __LINE__);
  }

  m_isoValueColor[surfaceIndex] = vColor;
  paint(PL_RECOMPOSE);
}

Core::Math::Vec3ui8 AbstractRenderer::getIsosurfaceColor(uint8_t surfaceIndex) const{
  if (surfaceIndex >= m_isoValue.size()) {
    throw TrinityError("invalid surfaceIndex", __FILE__, __LINE__);
  }

  return m_isoValueColor[surfaceIndex];
}

Core::Math::Vec2f AbstractRenderer::getRange(uint64_t modality) const {
  return m_io->getRange(modality);
}

// CLEARVIEW FUNCTIONS
void AbstractRenderer::setClearViewPosition(const Core::Math::Vec2f& vNormalizedWindowPos){
  m_clearViewNormalizedWindowPos = vNormalizedWindowPos;
  paint(PL_RECOMPOSE);
}

void AbstractRenderer::setClearViewRadius(float f){
  m_clearViewRadius = f;
  paint(PL_RECOMPOSE);
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

PhongColorTriple AbstractRenderer::getLightingColors() const {
  return m_lightingColors;
}

void AbstractRenderer::setLightDirection(const Core::Math::Vec3f& direction){
  m_lightDirection = direction;
  paint();
}

Core::Math::Vec3f AbstractRenderer::getLightDirection() const {
  return m_lightDirection;
}

// SAMPLE RATE FUNCTIONS
void AbstractRenderer::setSampleRateModifier(float fSampleRateModifier){
  m_fSampleRateModifier = fSampleRateModifier;
  paint();
}

float AbstractRenderer::getSampleRateModifier(){
  return m_fSampleRateModifier;
}

// render parameters
void AbstractRenderer::setBoundingBoxMode(BBoxMode mode){
  m_BBoxMode = mode;
  paint();
}

BBoxMode AbstractRenderer::getBoundingBoxMode() const{
  return m_BBoxMode;
}

void AbstractRenderer::setRendererSpecials(const std::vector<uint64_t>& params){
  m_RendererSpecials = params;
  paint();
}

std::vector<uint64_t> AbstractRenderer::getRendererSpecials() const {
  return m_RendererSpecials;
}

void AbstractRenderer::setBackgroundColors(std::pair<Core::Math::Vec3ui8, Core::Math::Vec3ui8> colors){
  m_backgroundColors = colors;
  paint();
}

std::pair<Core::Math::Vec3ui8, Core::Math::Vec3ui8> AbstractRenderer::getBackgroundColors() const {
  return m_backgroundColors;
}

// CLIPPING

void AbstractRenderer::enableClipping(bool enable) {
  m_enableClipping = enable;
  paint();
}

void AbstractRenderer::setClipVolume(const Core::Math::Vec3f& minValues,
                                     const Core::Math::Vec3f& maxValues) {
  m_clipVolumeMin = minValues;
  m_clipVolumeMax = maxValues;
  paint();
}

// TRANSFORMATION
void AbstractRenderer::setViewParameters(float angle, float znear, float zfar) {
  m_angle = angle;
  m_znear = znear;
  m_zfar = zfar;
  recomputeViewMatrix();
  paint();
}

void AbstractRenderer::rotateCamera(Core::Math::Vec3f rotation) {
  m_camRotation = m_camRotation * rotation;
  recomputeViewMatrix();
  paint();
}

void AbstractRenderer::moveCamera(Core::Math::Vec3f direction) {
  m_camPosition = m_camPosition * direction;
  recomputeViewMatrix();
  paint();
}

void AbstractRenderer::zoomCamera(float f) {
  m_camZoom = m_camZoom * f;
  recomputeViewMatrix();
  paint();
}

void AbstractRenderer::rotateScene(Core::Math::Vec3f rotation) {
  m_sceneRotation = m_sceneRotation * rotation;
  recomputeWorldMatrix();
  paint();
}

void AbstractRenderer::moveScene(Core::Math::Vec3f direction) {
  m_scenePosition = m_scenePosition * direction;
  recomputeWorldMatrix();
  paint();
}

void AbstractRenderer::rescaleScene(float f) {
  m_sceneScale = m_sceneScale * f;
  recomputeWorldMatrix();
  paint();
}

void AbstractRenderer::recomputeViewMatrix() {
  // TODO
}

void AbstractRenderer::recomputeWorldMatrix() {
  // TODO
}

void AbstractRenderer::resetCamera() {
  m_camPosition = Core::Math::Vec3f();
  m_camRotation = Core::Math::Vec3f();
  m_camZoom = 1;
  recomputeViewMatrix();
  paint();
}

void AbstractRenderer::resetObject() {
  m_scenePosition = Core::Math::Vec3f();
  m_sceneRotation = Core::Math::Vec3f();
  m_sceneScale = 1;
  recomputeWorldMatrix();
  paint();
}

void AbstractRenderer::startRendering() {
  m_bPaitingActive = true;
  paintInternal(PL_REDRAW);
}

void AbstractRenderer::stopRendering() {
  m_bPaitingActive = false;
}

