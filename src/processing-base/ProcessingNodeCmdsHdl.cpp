#include "ProcessingNodeCmdsHdl.h"
#include "RenderSession.h"
#include "RenderSessionManager.h"


using namespace trinity::processing;
using namespace trinity::common;

InitRendererHdl::InitRendererHdl(InitRendererCmd cmd) :
m_params(cmd.getParams()),
m_protocol(cmd.getProtocol()),
m_reply(cmd.getSid(), cmd.getRid()),
m_ioProxy(mocca::net::Endpoint(cmd.getStringifiedEndpoint()))
{}

InitRendererHdl::~InitRendererHdl() {}


void InitRendererHdl::execute() {

    if(!m_ioProxy.connect()) {
        throw mocca::Error("cannot connect to IO Node", __FILE__, __LINE__);
    }

    
    std::unique_ptr<RenderSession>
    session(new RenderSession(common::VclType::DummyRenderer, m_params, m_protocol));
    session->start();
    m_reply.setNewSid(session->getSid());
    m_reply.setControlPort(session->getControlPort());
    m_reply.setVisPort(session->getVisPort());
    
    SessionManagerSingleton::instance()->addSession(std::move(session));
}

std::unique_ptr<ICommand> InitRendererHdl::getReturnValue() {
    std::unique_ptr<ReplyInitRendererCmd> cmd(new ReplyInitRendererCmd(m_reply));
    return std::move(cmd);
}


