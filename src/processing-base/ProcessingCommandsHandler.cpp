#include "processing-base/ProcessingCommandsHandler.h"

#include "common/NetConfig.h"
#include "common/ProxyUtils.h"
#include "processing-base/ProcessingCommandFactory.h"
#include "processing-base/RenderSession.h"

#include "mocca/base/Memory.h"
#include "mocca/log/LogManager.h"

using namespace trinity;

InitProcessingSessionHdl::InitProcessingSessionHdl(const InitProcessingSessionRequest& request, ProcessingNode* node)
    : m_node(node)
    , m_request(request) {}

std::unique_ptr<Reply> InitProcessingSessionHdl::execute() {
    if (m_node->maxSessionsReached()) {
        throw TrinityError("Processing session cannot be created: maximum number of sessions has been reached", __FILE__, __LINE__);
    }

    auto requestParams = m_request.getParams();

    // create an IO node proxy and pass it to the renderer
    mocca::net::Endpoint ioEndpoint = m_request.getParams().getIoEndpoint();
    IONodeProxy ioNodeProxy(ioEndpoint);

    bool useLoopback = m_node->executionMode() == AbstractNode::ExecutionMode::Combined && m_node->isLocalMachine(ioEndpoint.machine);
    auto ioSessionProxy = ioNodeProxy.initIO(m_request.getParams().getFileId(), useLoopback);

    std::unique_ptr<RenderSession> session(
        new RenderSession(requestParams.getRenderType(), requestParams.getStreamingParams(), requestParams.getProtocol(), std::move(ioSessionProxy)));
    session->start();

    LINFO("(p) handling init req, session port " << session->getControlPort());
    InitProcessingSessionCmd::ReplyParams replyParams(session->getControlPort(), session->getVisPort());
    std::unique_ptr<Reply> reply = mocca::make_unique<InitProcessingSessionReply>(replyParams, m_request.getRid(), session->getSid());
    m_node->addSession(std::move(session));
    return reply;
}

SetIsoValueHdl::SetIsoValueHdl(const SetIsoValueRequest& request, RenderSession* session)
    : m_session(session)
    , m_request(request) {}

std::unique_ptr<Reply> SetIsoValueHdl::execute() {
    auto& renderer = m_session->getRenderer();
    renderer.setIsoValue(m_request.getParams().getSurfaceIndex(), m_request.getParams().getIsoValue());
    return nullptr;
}

InitContextHdl::InitContextHdl(const InitContextRequest& request, RenderSession* session)
    : m_session(session)
    , m_sid(request.getSid()) {}

std::unique_ptr<Reply> InitContextHdl::execute() {
    auto& renderer = m_session->getRenderer();
    renderer.initContext();
    return nullptr;
}

ZoomCameraHdl::ZoomCameraHdl(const ZoomCameraRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> ZoomCameraHdl::execute() {
    auto& renderer = m_session->getRenderer();
    renderer.zoomCamera(m_request.getParams().getZoom());
    return nullptr;
}

SetRenderModeHdl::SetRenderModeHdl(const SetRenderModeRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetRenderModeHdl::execute() {
    m_session->getRenderer().setRenderMode(m_request.getParams().getRenderMode());
    return nullptr;
}

SupportsRenderModeHdl::SupportsRenderModeHdl(const SupportsRenderModeRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SupportsRenderModeHdl::execute() {
    SupportsRenderModeCmd::ReplyParams params(m_session->getRenderer().supportsRenderMode(m_request.getParams().getRenderMode()));
    return mocca::make_unique<SupportsRenderModeReply>(params, m_request.getRid(), m_session->getSid());
}

SetActiveModalityHdl::SetActiveModalityHdl(const SetActiveModalityRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetActiveModalityHdl::execute() {
    m_session->getRenderer().setActiveModality(m_request.getParams().getModality());
    return nullptr;
}

GetActiveModalityHdl::GetActiveModalityHdl(const GetActiveModalityRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetActiveModalityHdl::execute() {
    GetActiveModalityCmd::ReplyParams params(m_session->getRenderer().getActiveModality());
    return mocca::make_unique<GetActiveModalityReply>(params, m_request.getRid(), m_session->getSid());
}

SetActiveTimestepHdl::SetActiveTimestepHdl(const SetActiveTimestepRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetActiveTimestepHdl::execute() {
    m_session->getRenderer().setActiveTimestep(m_request.getParams().getTimestep());
    return nullptr;
}

GetActiveTimestepHdl::GetActiveTimestepHdl(const GetActiveTimestepRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetActiveTimestepHdl::execute() {
    GetActiveTimestepCmd::ReplyParams params(m_session->getRenderer().getActiveTimestep());
    return mocca::make_unique<GetActiveTimestepReply>(params, m_request.getRid(), m_session->getSid());
}

GetModalityCountProcHdl::GetModalityCountProcHdl(const GetModalityCountProcRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetModalityCountProcHdl::execute() {
    GetModalityCountProcCmd::ReplyParams params(m_session->getRenderer().getModalityCount());
    return mocca::make_unique<GetModalityCountProcReply>(params, m_request.getRid(), m_session->getSid());
}

GetTimestepCountProcHdl::GetTimestepCountProcHdl(const GetTimestepCountProcRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetTimestepCountProcHdl::execute() {
    GetTimestepCountProcCmd::ReplyParams params(m_session->getRenderer().getTimestepCount());
    return mocca::make_unique<GetTimestepCountProcReply>(params, m_request.getRid(), m_session->getSid());
}

Set1DTransferFunctionHdl::Set1DTransferFunctionHdl(const Set1DTransferFunctionRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> Set1DTransferFunctionHdl::execute() {
    m_session->getRenderer().set1DTransferFunction(m_request.getParams().getTf());
    return nullptr;
}

GetDefault1DTransferFunctionProcHdl::GetDefault1DTransferFunctionProcHdl(const GetDefault1DTransferFunctionProcRequest& request,
                                                                         RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetDefault1DTransferFunctionProcHdl::execute() {
    GetDefault1DTransferFunctionProcCmd::ReplyParams params(
        m_session->getRenderer().getDefault1DTransferFunction(m_request.getParams().getIndex()));
    return mocca::make_unique<GetDefault1DTransferFunctionProcReply>(params, m_request.getRid(), m_session->getSid());
}

GetDefault1DTransferFunctionCountProcHdl::GetDefault1DTransferFunctionCountProcHdl(
    const GetDefault1DTransferFunctionCountProcRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetDefault1DTransferFunctionCountProcHdl::execute() {
    GetDefault1DTransferFunctionCountProcCmd::ReplyParams params(m_session->getRenderer().getDefault1DTransferFunctionCount(/* TODO */));
    return mocca::make_unique<GetDefault1DTransferFunctionCountProcReply>(params, m_request.getRid(), m_session->getSid());
}

Get1DHistogramProcHdl::Get1DHistogramProcHdl(const Get1DHistogramProcRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> Get1DHistogramProcHdl::execute() {
    Get1DHistogramProcCmd::ReplyParams params(m_session->getRenderer().get1DHistogram(/* TODO */));
    return mocca::make_unique<Get1DHistogramProcReply>(params, m_request.getRid(), m_session->getSid());
}

GetIsoValueHdl::GetIsoValueHdl(const GetIsoValueRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetIsoValueHdl::execute() {
    GetIsoValueCmd::ReplyParams params(m_session->getRenderer().getIsoValue(m_request.getParams().getSurfaceIndex()));
    return mocca::make_unique<GetIsoValueReply>(params, m_request.getRid(), m_session->getSid());
}

SetIsosurfaceColorHdl::SetIsosurfaceColorHdl(const SetIsosurfaceColorRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetIsosurfaceColorHdl::execute() {
    m_session->getRenderer().setIsosurfaceColor(m_request.getParams().getSurfaceIndex(), m_request.getParams().getVColor());
    return nullptr;
}

GetIsosurfaceColorHdl::GetIsosurfaceColorHdl(const GetIsosurfaceColorRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetIsosurfaceColorHdl::execute() {
    GetIsosurfaceColorCmd::ReplyParams params(m_session->getRenderer().getIsosurfaceColor(m_request.getParams().getSurfaceIndex()));
    return mocca::make_unique<GetIsosurfaceColorReply>(params, m_request.getRid(), m_session->getSid());
}

GetRangeProcHdl::GetRangeProcHdl(const GetRangeProcRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetRangeProcHdl::execute() {
    GetRangeProcCmd::ReplyParams params(m_session->getRenderer().getRange(m_request.getParams().getModality()));
    return mocca::make_unique<GetRangeProcReply>(params, m_request.getRid(), m_session->getSid());
}

SetClearViewPositionHdl::SetClearViewPositionHdl(const SetClearViewPositionRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetClearViewPositionHdl::execute() {
    m_session->getRenderer().setClearViewPosition(m_request.getParams().getVNormalizedWindowPos());
    return nullptr;
}

SetClearViewRadiusHdl::SetClearViewRadiusHdl(const SetClearViewRadiusRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetClearViewRadiusHdl::execute() {
    m_session->getRenderer().setClearViewRadius(m_request.getParams().getRadius());
    return nullptr;
}

SetClearBorderSizeHdl::SetClearBorderSizeHdl(const SetClearBorderSizeRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetClearBorderSizeHdl::execute() {
    m_session->getRenderer().setClearBorderSize(m_request.getParams().getBorderSize());
    return nullptr;
}

EnableLightingHdl::EnableLightingHdl(const EnableLightingRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> EnableLightingHdl::execute() {
    m_session->getRenderer().enableLighting(m_request.getParams().getEnable());
    return nullptr;
}

SetLightingColorsHdl::SetLightingColorsHdl(const SetLightingColorsRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetLightingColorsHdl::execute() {
    m_session->getRenderer().setLightingColors(m_request.getParams().getColors());
    return nullptr;
}

GetLightingColorsHdl::GetLightingColorsHdl(const GetLightingColorsRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetLightingColorsHdl::execute() {
    GetLightingColorsCmd::ReplyParams params(m_session->getRenderer().getLightingColors());
    return mocca::make_unique<GetLightingColorsReply>(params, m_request.getRid(), m_session->getSid());
}

SetLightDirectionHdl::SetLightDirectionHdl(const SetLightDirectionRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetLightDirectionHdl::execute() {
    m_session->getRenderer().setLightDirection(m_request.getParams().getDirection());
    return nullptr;
}

GetLightDirectionHdl::GetLightDirectionHdl(const GetLightDirectionRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetLightDirectionHdl::execute() {
    GetLightDirectionCmd::ReplyParams params(m_session->getRenderer().getLightDirection());
    return mocca::make_unique<GetLightDirectionReply>(params, m_request.getRid(), m_session->getSid());
}

SetSampleRateModifierHdl::SetSampleRateModifierHdl(const SetSampleRateModifierRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetSampleRateModifierHdl::execute() {
    m_session->getRenderer().setSampleRateModifier(m_request.getParams().getFSampleRateModifier());
    return nullptr;
}

GetSampleRateModifierHdl::GetSampleRateModifierHdl(const GetSampleRateModifierRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetSampleRateModifierHdl::execute() {
    GetSampleRateModifierCmd::ReplyParams params(m_session->getRenderer().getSampleRateModifier());
    return mocca::make_unique<GetSampleRateModifierReply>(params, m_request.getRid(), m_session->getSid());
}

SetBoundingBoxModeHdl::SetBoundingBoxModeHdl(const SetBoundingBoxModeRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetBoundingBoxModeHdl::execute() {
    m_session->getRenderer().setBoundingBoxMode(m_request.getParams().getMode());
    return nullptr;
}

GetBoundingBoxModeHdl::GetBoundingBoxModeHdl(const GetBoundingBoxModeRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetBoundingBoxModeHdl::execute() {
    GetBoundingBoxModeCmd::ReplyParams params(m_session->getRenderer().getBoundingBoxMode());
    return mocca::make_unique<GetBoundingBoxModeReply>(params, m_request.getRid(), m_session->getSid());
}

SetRendererSpecialsHdl::SetRendererSpecialsHdl(const SetRendererSpecialsRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetRendererSpecialsHdl::execute() {
    m_session->getRenderer().setRendererSpecials(m_request.getParams().getParams());
    return nullptr;
}

GetRendererSpecialsHdl::GetRendererSpecialsHdl(const GetRendererSpecialsRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetRendererSpecialsHdl::execute() {
    GetRendererSpecialsCmd::ReplyParams params(m_session->getRenderer().getRendererSpecials());
    return mocca::make_unique<GetRendererSpecialsReply>(params, m_request.getRid(), m_session->getSid());
}

SetBackgroundColorsHdl::SetBackgroundColorsHdl(const SetBackgroundColorsRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetBackgroundColorsHdl::execute() {
    m_session->getRenderer().setBackgroundColors(m_request.getParams().getColors());
    return nullptr;
}

GetBackgroundColorsHdl::GetBackgroundColorsHdl(const GetBackgroundColorsRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> GetBackgroundColorsHdl::execute() {
    GetBackgroundColorsCmd::ReplyParams params(m_session->getRenderer().getBackgroundColors());
    return mocca::make_unique<GetBackgroundColorsReply>(params, m_request.getRid(), m_session->getSid());
}

EnableClippingHdl::EnableClippingHdl(const EnableClippingRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> EnableClippingHdl::execute() {
    m_session->getRenderer().enableClipping(m_request.getParams().getEnable());
    return nullptr;
}

SetClipVolumeHdl::SetClipVolumeHdl(const SetClipVolumeRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetClipVolumeHdl::execute() {
    m_session->getRenderer().setClipVolume(m_request.getParams().getMinValues(), m_request.getParams().getMaxValues());
    return nullptr;
}

SetViewParametersHdl::SetViewParametersHdl(const SetViewParametersRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetViewParametersHdl::execute() {
    m_session->getRenderer().setViewParameters(m_request.getParams().getAngle(), m_request.getParams().getZnear(),
                                               m_request.getParams().getZfar());
    return nullptr;
}

RotateCameraHdl::RotateCameraHdl(const RotateCameraRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> RotateCameraHdl::execute() {
    m_session->getRenderer().rotateCamera(m_request.getParams().getRotation());
    return nullptr;
}

MoveCameraHdl::MoveCameraHdl(const MoveCameraRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> MoveCameraHdl::execute() {
    m_session->getRenderer().moveCamera(m_request.getParams().getDirection());
    return nullptr;
}

RotateSceneHdl::RotateSceneHdl(const RotateSceneRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> RotateSceneHdl::execute() {
    m_session->getRenderer().rotateScene(m_request.getParams().getRotation());
    return nullptr;
}

MoveSceneHdl::MoveSceneHdl(const MoveSceneRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> MoveSceneHdl::execute() {
    m_session->getRenderer().moveScene(m_request.getParams().getDirection());
    return nullptr;
}

RescaleSceneHdl::RescaleSceneHdl(const RescaleSceneRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> RescaleSceneHdl::execute() {
    m_session->getRenderer().rescaleScene(m_request.getParams().getScale());
    return nullptr;
}

ResetCameraHdl::ResetCameraHdl(const ResetCameraRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> ResetCameraHdl::execute() {
    m_session->getRenderer().resetCamera();
    return nullptr;
}

ResetSceneHdl::ResetSceneHdl(const ResetSceneRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> ResetSceneHdl::execute() {
    m_session->getRenderer().resetScene();
    return nullptr;
}

StartRenderingHdl::StartRenderingHdl(const StartRenderingRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> StartRenderingHdl::execute() {
    m_session->getRenderer().startRendering();
    return nullptr;
}

StopRenderingHdl::StopRenderingHdl(const StopRenderingRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> StopRenderingHdl::execute() {
    m_session->getRenderer().stopRendering();
    return nullptr;
}

IsIdleHdl::IsIdleHdl(const IsIdleRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> IsIdleHdl::execute() {
    IsIdleCmd::ReplyParams params(m_session->getRenderer().isIdle());
    return mocca::make_unique<IsIdleReply>(params, m_request.getRid(), m_session->getSid());
}

ProceedRenderingHdl::ProceedRenderingHdl(const ProceedRenderingRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> ProceedRenderingHdl::execute() {
    ProceedRenderingCmd::ReplyParams params(m_session->getRenderer().proceedRendering());
    return mocca::make_unique<ProceedRenderingReply>(params, m_request.getRid(), m_session->getSid());
}

SetUserViewMatrixHdl::SetUserViewMatrixHdl(const SetUserViewMatrixRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetUserViewMatrixHdl::execute() {
    m_session->getRenderer().setUserViewMatrix(m_request.getParams().getM());
    return nullptr;
}

SetUserWorldMatrixHdl::SetUserWorldMatrixHdl(const SetUserWorldMatrixRequest& request, RenderSession* session)
    : m_request(request)
    , m_session(session) {}

std::unique_ptr<Reply> SetUserWorldMatrixHdl::execute() {
    m_session->getRenderer().setUserWorldMatrix(m_request.getParams().getM());
    return nullptr;
}

/* AUTOGEN ProcCommandHandlerImpl */