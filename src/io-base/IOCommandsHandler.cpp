#include "io-base/IOCommandsHandler.h"

#include "io-base/IOCommandFactory.h"
#include "io-base/IOSession.h"

#include "mocca/base/Memory.h"

using namespace trinity::io;
using namespace trinity::commands;

InitIOSessionHdl::InitIOSessionHdl(const InitIOSessionRequest& request)
    : m_request(request) {}

std::unique_ptr<Reply> InitIOSessionHdl::execute() {
    auto requestParams = m_request.getParams();

    auto session = mocca::make_unique<IOSession>(requestParams.getProtocol(), mocca::make_unique<IOCommandFactory>(), requestParams.getFileId());
    session->start();
    
    InitIOSessionCmd::ReplyParams replyParams(session->getControlPort());
    auto reply = mocca::make_unique<InitIOSessionReply>(replyParams, m_request.getRid(), session->getSid());
    common::SessionManagerSingleton::instance()->addSession(std::move(session));
    return reply;
}


GetLODLevelCountHdl::GetLODLevelCountHdl(const GetLODLevelCountRequest& request)
    : m_request(request) {}

std::unique_ptr<Reply> GetLODLevelCountHdl::execute() {
    auto& session = common::SessionManagerSingleton::instance()->getSession(m_request.getSid());
    IOSession* ioSession = dynamic_cast<IOSession*>(&session); // dmc: see comments in ISession.h
    auto& io = ioSession->getIO();

    GetLODLevelCountCmd::ReplyParams replyParams(io.getLODLevelCount());
    return mocca::make_unique<GetLODLevelCountReply>(replyParams, m_request.getRid(), m_request.getSid());
}