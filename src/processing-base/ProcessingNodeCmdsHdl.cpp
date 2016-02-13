#include "ProcessingNodeCmdsHdl.h"
#include "RenderSession.h"
#include "RenderSessionManager.h"


using namespace trinity::processing;
using namespace trinity::common;

#include <iostream>
InitRendererHdl::InitRendererHdl(InitRendererCmd cmd) :
m_cmd(cmd),
m_reply(cmd.getSid(), cmd.getRid()),
m_ioProxy(mocca::net::Endpoint(cmd.getStringifiedEndpoint()))
{
}

InitRendererHdl::~InitRendererHdl() {}


void InitRendererHdl::execute() {

    // for now, the io has to be available at this point
    if(!m_ioProxy.connect()) {
        throw mocca::Error("cannot connect to IO Node", __FILE__, __LINE__);
    }

    // create an IO node proxy and pass it to the renderer
    auto ioSessionProxy = m_ioProxy.initIO(m_cmd.getFileId());
    
    std::unique_ptr<RenderSession>
    session(new RenderSession(common::VclType::DummyRenderer,
                              m_cmd.getParams(),
                              m_cmd.getProtocol(),
                              std::move(ioSessionProxy)));
    
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


