#include "IOCommandsHandler.h"
#include "IOSession.h"
#include "IOCommandFactory.h"
using namespace trinity::io;
using namespace trinity::commands;

InitIOSessionHdl::InitIOSessionHdl(InitIOSessionCmd cmd) :
m_reply(cmd.getSid(), cmd.getRid()),
m_protocol(cmd.getProtocol()),
m_fileId(cmd.getFileId())
{}

InitIOSessionHdl::~InitIOSessionHdl() {}


void InitIOSessionHdl::execute() {
    
    std::unique_ptr<commands::ICommandFactory> factory(new IOCommandFactory);
    std::unique_ptr<IOSession>
    session(new IOSession(m_protocol, std::move(factory), m_fileId));
    session->start();
    m_reply.setNewSid(session->getSid());
    m_reply.setControlPort(session->getControlPort());
    
    common::SessionManagerSingleton::instance()->addSession(std::move(session));
}

std::unique_ptr<ICommand> InitIOSessionHdl::getReturnValue() {
    std::unique_ptr<ReplyInitIOSessionCmd> cmd(new ReplyInitIOSessionCmd(m_reply));
    return std::move(cmd);
}


GetLODLevelCountHdl::GetLODLevelCountHdl(GetLODLevelCountCmd cmd) :
m_reply(cmd.getSid(), cmd.getRid()),
m_sid(cmd.getSid())
{}

GetLODLevelCountHdl::~GetLODLevelCountHdl() {}


void GetLODLevelCountHdl::execute() {
    
    auto& session = common::SessionManagerSingleton::instance()->getSession(m_sid);
    IOSession* ioSession = dynamic_cast<IOSession*>(&session); // dmc: see comments in ISession.h
    auto& io = ioSession->getIO();
    m_reply.setLODLevelCount(io.getLODLevelCount());
}


std::unique_ptr<ICommand> GetLODLevelCountHdl::getReturnValue() {
    std::unique_ptr<ReplyGetLODLevelCountCmd> cmd(new ReplyGetLODLevelCountCmd(m_reply));
    return std::move(cmd);
}