#include "frontend-base/RendererProxy.h"

#include "commands/ProcessingCommands.h"
#include "common/ProxyUtils.h"
#include "common/TrinityError.h"

#include <thread>

using namespace trinity;

RendererProxy::RendererProxy(std::shared_ptr<VisStream> s, mocca::net::Endpoint controlEndpoint, mocca::net::Endpoint visEndpoint,
                             unsigned int sid)
    : IRenderer(s)
    , m_inputChannel(controlEndpoint)
    , m_visReceiver(std::move(visEndpoint), s)
    , m_remoteSid(sid) {
    if (!connectInputChannel(m_inputChannel)) {
        throw TrinityError("Error connecting to processing node", __FILE__, __LINE__);
    }
    m_visReceiver.startStreaming();
}

RendererProxy::~RendererProxy() {
    m_visReceiver.endStreaming();
}

int RendererProxy::getRemoteSessionId() const {
    return m_remoteSid;
}

void RendererProxy::setIsoValue(uint8_t surfaceIndex, float value) {
    SetIsoValueCmd::RequestParams requestParams(surfaceIndex, value);
    SetIsoValueRequest request(requestParams, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::zoomCamera(float f) {
    // todo
    ZoomCameraCmd::RequestParams requestParams(f);
    ZoomCameraRequest request(requestParams, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::initContext() {
    InitContextCmd::RequestParams requestParams(1);
    InitContextRequest request(requestParams, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::setRenderMode(ERenderMode mode) {
    SetRenderModeCmd::RequestParams requestParams(mode);
    SetRenderModeRequest request(requestParams, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

bool RendererProxy::supportsRenderMode(ERenderMode mode) {
    SupportsRenderModeCmd::RequestParams params;
    SupportsRenderModeRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

void RendererProxy::setActiveModality(uint64_t modality) {
    SetActiveModalityCmd::RequestParams requestParams(modality);
    SetActiveModalityRequest request(requestParams, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

uint64_t RendererProxy::getActiveModality() const {
    GetActiveModalityCmd::RequestParams params;
    GetActiveModalityRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getModality();
}

void RendererProxy::setActiveTimestep(uint64_t timestep) {
    SetActiveTimestepCmd::RequestParams requestParams(timestep);
    SetActiveTimestepRequest request(requestParams, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

uint64_t RendererProxy::getActiveTimestep() const {
    GetActiveTimestepCmd::RequestParams params;
    GetActiveTimestepRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getTimestep();
}

uint64_t RendererProxy::getModalityCount() const {
    GetModalityCountProcCmd::RequestParams params;
    GetModalityCountProcRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

uint64_t RendererProxy::getTimestepCount() const {
    GetTimestepCountProcCmd::RequestParams params;
    GetTimestepCountProcRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

void RendererProxy::set1DTransferFunction(const TransferFunction1D& tf) {
    Set1DTransferFunctionCmd::RequestParams params(tf);
    Set1DTransferFunctionRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

TransferFunction1D RendererProxy::getDefault1DTransferFunction(uint64_t index) const {
    GetDefault1DTransferFunctionProcCmd::RequestParams params(index);
    GetDefault1DTransferFunctionProcRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

uint64_t RendererProxy::getDefault1DTransferFunctionCount() const {
    GetDefault1DTransferFunctionCountProcCmd::RequestParams params;
    GetDefault1DTransferFunctionCountProcRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

std::vector<uint64_t> RendererProxy::get1DHistogram() const {
    Get1DHistogramProcCmd::RequestParams params;
    Get1DHistogramProcRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

float RendererProxy::getIsoValue(uint8_t surfaceIndex) const {
    GetIsoValueCmd::RequestParams params(surfaceIndex);
    GetIsoValueRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

void RendererProxy::setIsosurfaceColor(uint8_t surfaceIndex, const Core::Math::Vec3ui8& vColor) {
    SetIsosurfaceColorCmd::RequestParams params(surfaceIndex, vColor);
    SetIsosurfaceColorRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

Core::Math::Vec3ui8 RendererProxy::getIsosurfaceColor(uint8_t surfaceIndex) const {
    GetIsosurfaceColorCmd::RequestParams params(surfaceIndex);
    GetIsosurfaceColorRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

Core::Math::Vec2f RendererProxy::getRange(uint64_t modality) const {
    GetRangeProcCmd::RequestParams params(modality);
    GetRangeProcRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

void RendererProxy::setClearViewPosition(const Core::Math::Vec2f& vNormalizedWindowPos) {
    SetClearViewPositionCmd::RequestParams params(vNormalizedWindowPos);
    SetClearViewPositionRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::setClearViewRadius(float radius) {
    SetClearViewRadiusCmd::RequestParams params(radius);
    SetClearViewRadiusRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::setClearBorderSize(float borderSize) {
    SetClearBorderSizeCmd::RequestParams params(borderSize);
    SetClearBorderSizeRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::enableLighting(bool enable) {
    EnableLightingCmd::RequestParams params(enable);
    EnableLightingRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::setLightingColors(const PhongColorTriple& colors) {
    SetLightingColorsCmd::RequestParams params(colors);
    SetLightingColorsRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

IRenderer::PhongColorTriple RendererProxy::getLightingColors() const {
    GetLightingColorsCmd::RequestParams params;
    GetLightingColorsRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

void RendererProxy::setLightDirection(const Core::Math::Vec3f& direction) {
    SetLightDirectionCmd::RequestParams params(direction);
    SetLightDirectionRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

Core::Math::Vec3f RendererProxy::getLightDirection() const {
    GetLightDirectionCmd::RequestParams params;
    GetLightDirectionRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

void RendererProxy::setSampleRateModifier(float fSampleRateModifier) {
    SetSampleRateModifierCmd::RequestParams params(fSampleRateModifier);
    SetSampleRateModifierRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

float RendererProxy::getSampleRateModifier() const {
    GetSampleRateModifierCmd::RequestParams params;
    GetSampleRateModifierRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

void RendererProxy::setBoundingBoxMode(BBoxMode mode) {
    SetBoundingBoxModeCmd::RequestParams params(mode);
    SetBoundingBoxModeRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

IRenderer::BBoxMode RendererProxy::getBoundingBoxMode() const {
    GetBoundingBoxModeCmd::RequestParams params;
    GetBoundingBoxModeRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

void RendererProxy::setRendererSpecials(const std::vector<uint64_t>& params) {
    SetRendererSpecialsCmd::RequestParams requestParams(params);
    SetRendererSpecialsRequest request(requestParams, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

std::vector<uint64_t> RendererProxy::getRendererSpecials() const {
    GetRendererSpecialsCmd::RequestParams params;
    GetRendererSpecialsRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

void RendererProxy::setBackgroundColors(const BackgroundColors& colors) {
    SetBackgroundColorsCmd::RequestParams params(colors);
    SetBackgroundColorsRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

IRenderer::BackgroundColors RendererProxy::getBackgroundColors() const {
    GetBackgroundColorsCmd::RequestParams params;
    GetBackgroundColorsRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

void RendererProxy::enableClipping(bool enable) {
    EnableClippingCmd::RequestParams params(enable);
    EnableClippingRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::setClipVolume(const Core::Math::Vec3f& minValues, const Core::Math::Vec3f& maxValues) {
    SetClipVolumeCmd::RequestParams params(minValues, maxValues);
    SetClipVolumeRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::setViewParameters(float angle, float znear, float zfar) {
    SetViewParametersCmd::RequestParams params(angle, znear, zfar);
    SetViewParametersRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::rotateCamera(Core::Math::Vec3f rotation) {
    RotateCameraCmd::RequestParams params(rotation);
    RotateCameraRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::moveCamera(Core::Math::Vec3f direction) {
    MoveCameraCmd::RequestParams params(direction);
    MoveCameraRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::rotateScene(Core::Math::Vec3f rotation) {
    RotateSceneCmd::RequestParams params(rotation);
    RotateSceneRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::moveScene(Core::Math::Vec3f direction) {
    MoveSceneCmd::RequestParams params(direction);
    MoveSceneRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::rescaleScene(float scale) {
    RescaleSceneCmd::RequestParams params(scale);
    RescaleSceneRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::resetCamera() {
    ResetCameraCmd::RequestParams params;
    ResetCameraRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::resetObject() {
    ResetObjectCmd::RequestParams params;
    ResetObjectRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::startRendering() {
    StartRenderingCmd::RequestParams params;
    StartRenderingRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::stopRendering() {
    StopRenderingCmd::RequestParams params;
    StopRenderingRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

bool RendererProxy::isIdle() {
    IsIdleCmd::RequestParams params;
    IsIdleRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

bool RendererProxy::proceedRendering() {
    ProceedRenderingCmd::RequestParams params;
    ProceedRenderingRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

void RendererProxy::setUserViewMatrix(Core::Math::Mat4f m) {
    SetUserViewMatrixCmd::RequestParams params(m);
    SetUserViewMatrixRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

void RendererProxy::setUserWorldMatrix(Core::Math::Mat4f m) {
    SetUserWorldMatrixCmd::RequestParams params(m);
    SetUserWorldMatrixRequest request(params, IDGenerator::nextID(), m_remoteSid);
    m_inputChannel.sendRequest(request);
}

/* AUTOGEN RendererProxyImpl */