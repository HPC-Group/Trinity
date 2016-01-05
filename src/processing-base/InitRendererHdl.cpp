#include "InitRendererHdl.h"
#include "RenderSession.h"
#include "SessionManager.h"

using namespace trinity::processing;
using namespace trinity::common;

InitRendererHdl::InitRendererHdl(InitRendererCmd cmd) :
m_params(cmd.getParams()),
m_protocol(cmd.getProtocol()),
m_reply(cmd.getSid(), cmd.getRid())
{}

InitRendererHdl::~InitRendererHdl() {}


void InitRendererHdl::execute() {
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


