#include "ProcessingCommandsHandler.h"
#include "RenderSession.h"
#include "ProcessingCommandFactory.h"


using namespace trinity::processing;
using namespace trinity::common;
using namespace trinity::commands;

#include <iostream>
InitProcessingSessionHdl::InitProcessingSessionHdl(commands::InitProcessingSessionCmd cmd) :
m_cmd(cmd),
m_reply(cmd.getSid(), cmd.getRid()),
m_ioProxy(mocca::net::Endpoint(cmd.getStringifiedEndpoint()))
{
}

InitProcessingSessionHdl::~InitProcessingSessionHdl() {}


void InitProcessingSessionHdl::execute() {

    // for now, the io has to be available at this point
    if(!m_ioProxy.connect()) {
        throw mocca::Error("cannot connect to IO Node", __FILE__, __LINE__);
    }

    // create an IO node proxy and pass it to the renderer
    auto ioSessionProxy = m_ioProxy.initIO(m_cmd.getFileId());
    
    std::unique_ptr<commands::ICommandFactory> factory(new ProcessingCommandFactory);
    
    std::unique_ptr<RenderSession>
    session(new RenderSession(std::move(factory),
                              commands::VclType::DummyRenderer,
                              m_cmd.getParams(),
                              m_cmd.getProtocol(),
                              std::move(ioSessionProxy)));
    
    session->start();
    
    m_reply.setNewSid(session->getSid());
    m_reply.setControlPort(session->getControlPort());
    m_reply.setVisPort(session->getVisPort());
    
    SessionManagerSingleton::instance()->addSession(std::move(session));
}

std::unique_ptr<ICommand> InitProcessingSessionHdl::getReturnValue() {
    std::unique_ptr<ReplyInitProcessingSessionCmd> cmd(new ReplyInitProcessingSessionCmd(m_reply));
    return std::move(cmd);
}


SetIsoValueHdl::SetIsoValueHdl(SetIsoValueCmd cmd) :
m_isoValue(cmd.getIsoValue()),
m_sid(cmd.getSid())
{}

SetIsoValueHdl::~SetIsoValueHdl() {}


void SetIsoValueHdl::execute() {
    
    auto& session = SessionManagerSingleton::instance()->getSession(m_sid);
    RenderSession* renderSession = dynamic_cast<RenderSession*>(&session);
    auto& renderer = renderSession->getRenderer();
    renderer.setIsoValue(m_isoValue);
    
}





