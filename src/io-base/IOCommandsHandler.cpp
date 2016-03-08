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
    std::unique_ptr<Reply> reply = mocca::make_unique<InitIOSessionReply>(replyParams, m_request.getRid(), session->getSid());
    IOSessionManager::instance()->addSession(std::move(session));
    return reply;
}


GetLODLevelCountHdl::GetLODLevelCountHdl(const GetLODLevelCountRequest& request)
    : m_request(request) {}

std::unique_ptr<Reply> GetLODLevelCountHdl::execute() {
    const auto& session = IOSessionManager::instance()->getSession(m_request.getSid());
    const auto& io = session.getIO();

    GetLODLevelCountCmd::ReplyParams replyParams(io.getLODLevelCount());
    return mocca::make_unique<GetLODLevelCountReply>(replyParams, m_request.getRid(), m_request.getSid());
}


ListFilesHdl::ListFilesHdl(const ListFilesRequest& request)
    : m_request(request) {}

std::unique_ptr<Reply> ListFilesHdl::execute() {
    IOData dummy1("dummy1", 42, VclType::FractalIO);
    IOData dummy2("dummy2", 23, VclType::FractalIO);
    std::vector<IOData> ioDataVec{ dummy1, dummy2 };
    ListFilesCmd::ReplyParams replyParams(ioDataVec);
    
    int rid = 1; // dummy value, fix
    int sid = 2; // dummy value, fix
    return mocca::make_unique<ListFilesReply>(replyParams, rid, sid);
}