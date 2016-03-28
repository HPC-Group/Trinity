#include "processing-base/ProcessingCommandsHandler.h"

#include "common/ProxyUtils.h"
#include "processing-base/ProcessingCommandFactory.h"
#include "processing-base/RenderSession.h"

#include "mocca/base/Memory.h"
#include "mocca/log/LogManager.h"

using namespace trinity;

InitProcessingSessionHdl::InitProcessingSessionHdl(const InitProcessingSessionRequest& request, ProcessingNode* node)
    : m_node(node)
    , m_request(request)
    , m_ioProxy(request.getParams().getIoEndpoint()) {}

std::unique_ptr<Reply> InitProcessingSessionHdl::execute() {
    auto params = m_request.getParams();

    // create an IO node proxy and pass it to the renderer
    auto ioSessionProxy = m_ioProxy.initIO(m_request.getParams().getFileId());

    try {
        std::unique_ptr<RenderSession> session(
            new RenderSession(params.getRenderType(), params.getStreamingParams(), params.getProtocol(), std::move(ioSessionProxy)));
        session->start();

        LINFO("(p) handling init req, session port " << session->getControlPort());
        InitProcessingSessionCmd::ReplyParams params(session->getControlPort(), session->getVisPort());
        std::unique_ptr<Reply> reply = mocca::make_unique<InitProcessingSessionReply>(params, m_request.getRid(), session->getSid());
        m_node->addSession(std::move(session));
        return reply;
    } catch (const std::runtime_error&) {
        ErrorCmd::ReplyParams replyParams(2);
        return mocca::make_unique<ErrorReply>(replyParams, m_request.getRid(), m_request.getSid());
    }
}

SetIsoValueHdl::SetIsoValueHdl(const SetIsoValueRequest& request, RenderSession* session)
    : m_session(session)
    , m_isoValue(request.getParams().getIsoValue())
    , m_sid(request.getSid()) {}

std::unique_ptr<Reply> SetIsoValueHdl::execute() {
    auto& renderer = m_session->getRenderer();
    renderer.setIsoValue(m_isoValue);
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
    GetDefault1DTransferFunctionProcCmd::ReplyParams params(m_session->getRenderer().getDefault1DTransferFunction(m_request.getParams().getIndex()));
    return mocca::make_unique<GetDefault1DTransferFunctionProcReply>(params, m_request.getRid(), m_session->getSid());
}

/* AUTOGEN ProcCommandHandlerImpl */