#include "io-base/IOCommandsHandler.h"

#include "common/TrinityError.h"
#include "io-base/IOCommandFactory.h"
#include "io-base/IOSession.h"

#include "mocca/base/Memory.h"

using namespace trinity;

InitIOSessionHdl::InitIOSessionHdl(const InitIOSessionRequest& request, IONode* node)
    : m_request(request), m_node(node) {}

std::unique_ptr<Reply> InitIOSessionHdl::execute() {
    auto requestParams = m_request.getParams();

    auto session = mocca::make_unique<IOSession>(requestParams.getProtocol(), requestParams.getFileId());
    session->start();
    
    InitIOSessionCmd::ReplyParams replyParams(session->getControlPort());
    std::unique_ptr<Reply> reply = mocca::make_unique<InitIOSessionReply>(replyParams, m_request.getRid(), session->getSid());
    m_node->addSession(std::move(session));
    return reply;
}


GetLODLevelCountHdl::GetLODLevelCountHdl(const GetLODLevelCountRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetLODLevelCountHdl::execute() {
    const auto& io = m_session->getIO();
    GetLODLevelCountCmd::ReplyParams replyParams(io.getLODLevelCount());
    return mocca::make_unique<GetLODLevelCountReply>(replyParams, m_request.getRid(), m_request.getSid());
}


ListFilesHdl::ListFilesHdl(const ListFilesRequest& request, IONode* node)
    : m_request(request), m_node(node) {}

std::unique_ptr<Reply> ListFilesHdl::execute() {
    std::vector<IOData> ioDataVec;
    int32_t dirID = m_request.getParams().getDirID();
    switch (dirID) {
    case 0: {
        ioDataVec.push_back(IOData("Flat Data", 1, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("Bricked Data", 2, IOData::DataType::Directory));
        break;
    }
    case 1: {
        ioDataVec.push_back(IOData("64^3", 3, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("128^3", 4, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("256^3", 5, IOData::DataType::Directory));
        break;
    }
    case 2: {
        ioDataVec.push_back(IOData("512^3 (32^3 bricks)", 6, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("1024^3 (32^3 bricks)", 7, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("2048^3 (32^3 bricks)", 8, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("4096^3 (32^3 bricks)", 9, IOData::DataType::Directory));
        break;
    }
    default:
        throw TrinityError("Invalid directory ID: " + std::to_string(dirID), __FILE__, __LINE__);
        break;
    }
    ListFilesCmd::ReplyParams replyParams(ioDataVec);
    return mocca::make_unique<ListFilesReply>(replyParams, m_request.getRid(), m_request.getSid());
}

#define PYTHON_MAGIC



GetMaxBrickSizeHdl::GetMaxBrickSizeHdl(const GetMaxBrickSizeRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetMaxBrickSizeHdl::execute() {
    return nullptr; // TODO dmc
}

#undef PYTHON_MAGIC