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
