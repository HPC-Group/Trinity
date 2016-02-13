#include "IONodeCmdsHdl.h"
#include "IOSession.h"
#include "IOSessionManager.h"

using namespace trinity::io;
using namespace trinity::common;

InitIOSessionHdl::InitIOSessionHdl(InitIOSessionCmd cmd) :
m_reply(cmd.getSid(), cmd.getRid()),
m_protocol(cmd.getProtocol()),
m_fileId(cmd.getFileId())
{}

InitIOSessionHdl::~InitIOSessionHdl() {}


void InitIOSessionHdl::execute() {
    
    std::unique_ptr<IOSession>
    session(new IOSession(m_protocol, m_fileId));
    session->start();
    m_reply.setNewSid(session->getSid());
    m_reply.setControlPort(session->getControlPort());
    
    SessionManagerSingleton::instance()->addSession(std::move(session));
}

std::unique_ptr<ICommand> InitIOSessionHdl::getReturnValue() {
    std::unique_ptr<ReplyInitIOSessionCmd> cmd(new ReplyInitIOSessionCmd(m_reply));
    return std::move(cmd);
}


