#include "processing-base/ProcessingCommandsHandler.h"

#include "processing-base/ProcessingCommandFactory.h"
#include "processing-base/RenderSession.h"

#include "mocca/base/Memory.h"
#include "mocca/log/LogManager.h"

using namespace trinity::processing;
using namespace trinity::common;
using namespace trinity::commands;

InitProcessingSessionHdl::InitProcessingSessionHdl(const InitProcessingSessionRequest& request)
    : m_request(request)
    , m_ioProxy(mocca::net::Endpoint(request.getParams().getStringifiedEndpoint())) {}

std::unique_ptr<Reply> InitProcessingSessionHdl::execute() {
    // for now, the io has to be available at this point
    if (!m_ioProxy.connect()) {
        throw mocca::Error("cannot connect to IO Node", __FILE__, __LINE__);
    }

    auto params = m_request.getParams();

    // create an IO node proxy and pass it to the renderer
    auto ioSessionProxy = m_ioProxy.initIO(m_request.getParams().getFileId());

    auto factory = mocca::make_unique<ProcessingCommandFactory>();

    try {
        std::unique_ptr<RenderSession> session(new RenderSession(std::move(factory), params.getRenderType(), params.getStreamingParams(),
                                                                 params.getProtocol(), std::move(ioSessionProxy)));
        session->start();
        
        LINFO("(p) handling init req, session port " + std::to_string(session->getControlPort()));
        InitProcessingSessionCmd::ReplyParams params(session->getControlPort(), session->getVisPort());
        std::unique_ptr<Reply> reply = mocca::make_unique<InitProcessingSessionReply>(params, m_request.getRid(), session->getSid());
        SessionManagerSingleton::instance()->addSession(std::move(session));
        return reply;
    } catch (const mocca::Error&) {
        ErrorCmd::ReplyParams replyParams(2);
        return mocca::make_unique<ErrorReply>(replyParams, m_request.getRid(), m_request.getSid());
    }
}


SetIsoValueHdl::SetIsoValueHdl(const SetIsoValueRequest& request)
    : m_isoValue(request.getParams().getIsoValue())
    , m_sid(request.getSid()) {}

std::unique_ptr<Reply> SetIsoValueHdl::execute() {
    auto& session = SessionManagerSingleton::instance()->getSession(m_sid);
    RenderSession* renderSession = dynamic_cast<RenderSession*>(&session); // dmc: see comments in ISession.h
    auto& renderer = renderSession->getRenderer();
    renderer.setIsoValue(m_isoValue);
    return nullptr;
}


InitContextHdl::InitContextHdl(const InitContextRequest& request)
    : m_sid(request.getSid()) {
}

std::unique_ptr<Reply> InitContextHdl::execute() {
    auto& session = SessionManagerSingleton::instance()->getSession(m_sid);
    RenderSession* renderSession = dynamic_cast<RenderSession*>(&session); // dmc: see comments in ISession.h
    auto& renderer = renderSession->getRenderer();
    renderer.initContext();
    return nullptr;
}
