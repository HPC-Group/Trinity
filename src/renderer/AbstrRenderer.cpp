#include "AbstrRenderer.h"
#include "Camera.h"

using namespace Tuvok::Renderer;
using namespace Tuvok;
using namespace Core::Math;

AbstrRenderer::AbstrRenderer(std::shared_ptr<Tuvok::Renderer::Context::Context> context,Vec2ui vWinSize, ERenderMode mode) :
m_pRenderRegion(nullptr),
m_pClipplane(nullptr),
m_bCompleteRedraw(true),
m_bFinishedInit(false),
m_iFrameCounter(0),
m_storedFrame(),
m_pContext(context),
m_bKeepRunning(true)
{
  m_pRenderState = std::make_shared<State>();
  m_pRenderState->m_vLightDir = m_pRenderState->m_pCameraPtr.GetWorldPosition();
  m_pRenderState->m_vLightDir.normalize();
  m_pRenderState->m_resolution = vWinSize;

  //default search pathes for shaders
  m_vSearchPathes.push_back("");
  m_vSearchPathes.push_back("Shaders/");
  m_vSearchPathes.push_back("shaders/");
  m_vSearchPathes.push_back("Release/");
  m_vSearchPathes.push_back("release/");
  m_vSearchPathes.push_back("Debug/");
  m_vSearchPathes.push_back("debug/");
}

AbstrRenderer::~AbstrRenderer(){

}

bool AbstrRenderer::Initialize(){
  return true;
}

void AbstrRenderer::Cleanup(){

}

bool AbstrRenderer::Paint(){

  return true;
}

void AbstrRenderer::SetClearFrameBuffer(bool bClearFramebuffer){
    m_pRenderState->m_bClearFrameBuffer = bClearFramebuffer;
}

bool AbstrRenderer::GetClearFrameBuffer() const{
    return m_pRenderState->m_bClearFrameBuffer;
}

void AbstrRenderer::SetViewParameters(float angle, float znear, float zfar){
    m_pRenderState->m_fFOV = angle;
    m_pRenderState->m_fZNear = znear;
    m_pRenderState->m_fZFar = zfar;
    this->BuildViewMatrix();
    this->BuildProjectionMatrix();
    this->ScheduleCompleteRedraw();
}

void AbstrRenderer::ScheduleCompleteRedraw() {
    m_bCompleteRedraw = true;
    m_iFinishCounter = 1;
}

Core::Math::Mat4f& AbstrRenderer::GetProjectionMatrix(){
  return m_pRenderState->m_mProjection;
}

Core::Math::Mat4f& AbstrRenderer::GetViewMatrix(){
  return m_pRenderState->m_mView;
}

bool AbstrRenderer::SetBackgroundColors(Vec3f vTopColor,
                                 Vec3f vBotColor) {
	if (vTopColor != m_pRenderState->m_vBackGroundColorTop ||
		vBotColor != m_pRenderState->m_vBackGroundColorBottom) {
	m_pRenderState->m_vBackGroundColorTop = vTopColor;
	m_pRenderState->m_vBackGroundColorBottom = vBotColor;
    ScheduleCompleteRedraw();
    return true;
  } return false;
}
Core::Math::Vec3f AbstrRenderer::GetBackgroundColor(const uint8_t index){
	if (index == 0){
		return m_pRenderState->m_vBackGroundColorTop;
	}
	else{
		return m_pRenderState->m_vBackGroundColorBottom;
	}
}

void AbstrRenderer::SetUseLighting(bool bUseLighting){
  m_pRenderState->m_bUseLighting = bUseLighting;
}
void AbstrRenderer::SetSampleRateModifier(float fSampleRateModifier){
  m_pRenderState->m_fSampleRateModifier = fSampleRateModifier;
  ScheduleCompleteRedraw();
}

void AbstrRenderer::SetIsoValue(float fIsoValue){
  m_pRenderState->m_fIsoValue = fIsoValue;
  ScheduleCompleteRedraw();
}
void AbstrRenderer::SetIsoValueRelative(float fIsoValue){
  m_pRenderState->m_fIsoValue = fIsoValue;
  ScheduleCompleteRedraw();
}
float AbstrRenderer::GetIsoValue(){
  return m_pRenderState->m_fIsoValue;
}
void AbstrRenderer::SetIsosurfaceColor(const Core::Math::Vec3f& vColor){
  m_pRenderState->m_vIsoColor = vColor;
}
Core::Math::Vec3f AbstrRenderer::GetIsosurfaceColor() const {
  return m_pRenderState->m_vIsoColor;
}

Core::Math::Vec2ui AbstrRenderer::GetSize() const {
  return m_pRenderState->m_resolution;
}

void AbstrRenderer::Resize(const Core::Math::Vec2ui& vWinSize){
  m_pRenderState->m_resolution = vWinSize;
  m_pRenderState->m_bFirstDrawAfterResize = true;
  ScheduleCompleteRedraw();
}

void AbstrRenderer::GetVolumeAABB(Core::Math::Vec3f& vCenter, Core::Math::Vec3f& vExtend) const {
  vCenter = Vec3f(0,0,0);
  vExtend = Vec3f(1,1,1);
}


//! function to set a pointer to a dataio object \todo should later communicate over the network
void AbstrRenderer::SetDataset(IOPtr dio){
	m_pToCDataset = dio;
}


void AbstrRenderer::BuildProjectionMatrix(){
  m_pRenderState->m_mProjection.Perspective(m_pRenderState->m_fFOV, (float)m_pRenderState->m_resolution.x/(float)m_pRenderState->m_resolution.y, m_pRenderState->m_fZNear, m_pRenderState->m_fZFar);
}
void AbstrRenderer::BuildViewMatrix(){
  m_pRenderState->m_mView = m_pRenderState->m_pCameraPtr.buildLookAt();
  this->ScheduleCompleteRedraw();
}

void AbstrRenderer::SetFirstPersonMode(bool mode){
  m_pRenderState->m_pCameraPtr.setCameraMode(mode);
}
void AbstrRenderer::RotateCamera(Core::Math::Vec3f rotation){
  m_pContext->lockContext();
  m_pRenderState->m_pCameraPtr.rotateCamera(rotation);
  BuildViewMatrix();
  m_pContext->unlockContext();
}
void AbstrRenderer::MoveCamera(Core::Math::Vec3f direction){
  m_pContext->lockContext();
  m_pRenderState->m_pCameraPtr.moveCamera(direction);
  BuildViewMatrix();
  m_pContext->unlockContext();
}

void AbstrRenderer::ResetCamera(){
	m_pRenderState->m_pCameraPtr.ResetCamera();
	BuildViewMatrix();
}

void AbstrRenderer::SetCamera(Camera c){
		m_pRenderState->m_pCameraPtr = c;
		BuildViewMatrix();
	}

void AbstrRenderer::SetCameraZoom(float f){
  m_pRenderState->m_pCameraPtr.setZoom(f);
  BuildViewMatrix();
}

void AbstrRenderer::ZoomCamera(float f){
  m_pRenderState->m_pCameraPtr.setZoom(m_pRenderState->m_pCameraPtr.getZoom()+f);
  BuildViewMatrix();
}


void AbstrRenderer::SetCompositeMode(ECompositeDisplay displaymode){
	m_pRenderState->m_eCompsoitingMode = displaymode;
}

bool AbstrRenderer::RecreateAfterResize(){
	if (m_pRenderState->m_bFirstDrawAfterResize){
		BuildProjectionMatrix();

		m_pRenderState->m_bFirstDrawAfterResize = false;
		return true;
	}
	return false;
}

void AbstrRenderer::SetRenderMode(ERenderMode mode){
	m_pRenderState->m_eRenderMode = mode;
}

void AbstrRenderer::SetLoDFactor(const float f){
	m_pRenderState->m_fLODFactor = f;
}

float AbstrRenderer::GetSampleRateModifier(){
	return m_pRenderState->m_fSampleRateModifier;
}

//! \todo check for negative color values ! better check all this in widget, not in renderer
void AbstrRenderer::SetLightingParameters(Core::Math::Vec4f cAmbient,
	Core::Math::Vec4f cDiffuse,
	Core::Math::Vec4f cSpecular
	){
	cAmbient.x > 1.0f ? m_pRenderState->m_cAmbient.x = 1.0f : m_pRenderState->m_cAmbient.x = cAmbient.x;
	cAmbient.y > 1.0f ? m_pRenderState->m_cAmbient.y = 1.0f : m_pRenderState->m_cAmbient.y = cAmbient.y;
	cAmbient.z > 1.0f ? m_pRenderState->m_cAmbient.z = 1.0f : m_pRenderState->m_cAmbient.z = cAmbient.z;

	cDiffuse.x > 1.0f ? m_pRenderState->m_cDiffuse.x = 1.0f : m_pRenderState->m_cDiffuse.x = cDiffuse.x;
	cDiffuse.y > 1.0f ? m_pRenderState->m_cDiffuse.y = 1.0f : m_pRenderState->m_cDiffuse.y = cDiffuse.y;
	cDiffuse.z > 1.0f ? m_pRenderState->m_cDiffuse.z = 1.0f : m_pRenderState->m_cDiffuse.z = cDiffuse.z;

	cSpecular.x > 1.0f ? m_pRenderState->m_cSpecular.x = 1.0f : m_pRenderState->m_cSpecular.x = cSpecular.x;
	cSpecular.y > 1.0f ? m_pRenderState->m_cSpecular.y = 1.0f : m_pRenderState->m_cSpecular.y = cSpecular.y;
	cSpecular.z > 1.0f ? m_pRenderState->m_cSpecular.z = 1.0f : m_pRenderState->m_cSpecular.z = cSpecular.z;
	this->ScheduleCompleteRedraw();
}


const Vec4f AbstrRenderer::GetAmbientColor(){
	return m_pRenderState->m_cAmbient;
}
const Vec4f AbstrRenderer::GetDiffuseColor(){
	return m_pRenderState->m_cDiffuse;
}
const Vec4f AbstrRenderer::GetSpecularColor(){
	return m_pRenderState->m_cSpecular;
}


void AbstrRenderer::setClearViewEnabled(bool b){
    m_pRenderState->m_bClearViewEnabled = b;
    this->ScheduleCompleteRedraw();
}

void AbstrRenderer::setClearViewRadius(float f){
    m_pRenderState->m_fClearViewRadius = f;
    this->ScheduleCompleteRedraw();
}

State AbstrRenderer::getRenderState(){
    return *(m_pRenderState.get());
}

void AbstrRenderer::setRenderState(State renderState){
    m_pRenderState.reset();
    m_pRenderState = std::make_shared<State>(renderState);
}

void AbstrRenderer::SetColorDataset(bool isColor){
			m_pRenderState->m_bIsColor = isColor;
};

//*************THREADING
void AbstrRenderer::startRenderThread(){
    while(!m_bFinishedInit){}
    //start runthread;
	LINFOC("GLGridLeaper","try to start new renderthread");
	m_pRenderThread = std::make_shared<std::thread>(&AbstrRenderer::run, this);
}
void AbstrRenderer::pauseRenderThread(){}

void AbstrRenderer::stopRenderThread(){
    m_bKeepRunning = false;
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    m_pRenderThread->join();

}

void AbstrRenderer::stopRenderer(){
    m_bKeepRunning = false;
}
