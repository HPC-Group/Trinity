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

    auto session = mocca::make_unique<IOSession>(requestParams.getProtocol(), IOSession::createIO(requestParams.getFileId()));
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
    GetMaxBrickSizeCmd::ReplyParams params(m_session->getIO().getMaxBrickSize());
    return mocca::make_unique<GetMaxBrickSizeReply>(params, m_request.getRid(), m_session->getSid());
}

GetMaxUsedBrickSizesHdl::GetMaxUsedBrickSizesHdl(const GetMaxUsedBrickSizesRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetMaxUsedBrickSizesHdl::execute() {
    GetMaxUsedBrickSizesCmd::ReplyParams params(m_session->getIO().getMaxUsedBrickSizes());
    return mocca::make_unique<GetMaxUsedBrickSizesReply>(params, m_request.getRid(), m_session->getSid());
}

MaxMinForKeyHdl::MaxMinForKeyHdl(const MaxMinForKeyRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> MaxMinForKeyHdl::execute() {
    auto brickKey = m_request.getParams().getBrickKey();
    auto minMax = m_session->getIO().maxMinForKey(brickKey);
    MaxMinForKeyCmd::ReplyParams params(minMax);
    return mocca::make_unique<MaxMinForKeyReply>(params, m_request.getRid(), m_session->getSid());
}

GetNumberOfTimestepsHdl::GetNumberOfTimestepsHdl(const GetNumberOfTimestepsRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetNumberOfTimestepsHdl::execute() {
    GetNumberOfTimestepsCmd::ReplyParams params(m_session->getIO().getNumberOfTimesteps());
    return mocca::make_unique<GetNumberOfTimestepsReply>(params, m_request.getRid(), m_session->getSid());
}

GetDomainSizeHdl::GetDomainSizeHdl(const GetDomainSizeRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetDomainSizeHdl::execute() {
    auto lod = m_request.getParams().getLod();
    auto modality = m_request.getParams().getModality();
    auto ts = m_request.getParams().getTs(); // todo remove
    GetDomainSizeCmd::ReplyParams params(m_session->getIO().getDomainSize(lod, modality));
    return mocca::make_unique<GetDomainSizeReply>(params, m_request.getRid(), m_session->getSid());
}

GetTransformationHdl::GetTransformationHdl(const GetTransformationRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetTransformationHdl::execute() {
    GetTransformationCmd::ReplyParams params(m_session->getIO().getTransformation(m_request.getParams().getModality()));
    return mocca::make_unique<GetTransformationReply>(params, m_request.getRid(), m_session->getSid());
}

GetBrickOverlapSizeHdl::GetBrickOverlapSizeHdl(const GetBrickOverlapSizeRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetBrickOverlapSizeHdl::execute() {
    GetBrickOverlapSizeCmd::ReplyParams params(m_session->getIO().getBrickOverlapSize());
    return mocca::make_unique<GetBrickOverlapSizeReply>(params, m_request.getRid(), m_session->getSid());
}

GetLargestSingleBrickLODHdl::GetLargestSingleBrickLODHdl(const GetLargestSingleBrickLODRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetLargestSingleBrickLODHdl::execute() {
    auto modality = m_request.getParams().getModality();
    auto ts = m_request.getParams().getTs(); // todo remove
    GetLargestSingleBrickLODCmd::ReplyParams params(m_session->getIO().getLargestSingleBrickLOD(modality));
    return mocca::make_unique<GetLargestSingleBrickLODReply>(params, m_request.getRid(), m_session->getSid());
}

GetBrickVoxelCountsHdl::GetBrickVoxelCountsHdl(const GetBrickVoxelCountsRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetBrickVoxelCountsHdl::execute() {
    GetBrickVoxelCountsCmd::ReplyParams params(m_session->getIO().getBrickVoxelCounts(m_request.getParams().getBrickKey()));
    return mocca::make_unique<GetBrickVoxelCountsReply>(params, m_request.getRid(), m_session->getSid());
}

GetBrickExtentsHdl::GetBrickExtentsHdl(const GetBrickExtentsRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetBrickExtentsHdl::execute() {
    GetBrickExtentsCmd::ReplyParams params(m_session->getIO().getBrickExtents(m_request.getParams().getBrickKey()));
    return mocca::make_unique<GetBrickExtentsReply>(params, m_request.getRid(), m_session->getSid());
}

GetBrickLayoutHdl::GetBrickLayoutHdl(const GetBrickLayoutRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetBrickLayoutHdl::execute() {
    auto lod = m_request.getParams().getLod();
    auto modality = m_request.getParams().getModality();
    auto timestep = m_request.getParams().getTimestep(); // todo remove
    GetBrickLayoutCmd::ReplyParams params(m_session->getIO().getBrickLayout(lod, modality));
    return mocca::make_unique<GetBrickLayoutReply>(params, m_request.getRid(), m_session->getSid());
}

GetModalityCountHdl::GetModalityCountHdl(const GetModalityCountRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetModalityCountHdl::execute() {
    GetModalityCountCmd::ReplyParams params(m_session->getIO().getModalityCount());
    return mocca::make_unique<GetModalityCountReply>(params, m_request.getRid(), m_session->getSid());
}

GetComponentCountHdl::GetComponentCountHdl(const GetComponentCountRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetComponentCountHdl::execute() {
    GetComponentCountCmd::ReplyParams params(m_session->getIO().getComponentCount(m_request.getParams().getModality()));
    return mocca::make_unique<GetComponentCountReply>(params, m_request.getRid(), m_session->getSid());
}

GetRangeHdl::GetRangeHdl(const GetRangeRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetRangeHdl::execute() {
    GetRangeCmd::ReplyParams params(m_session->getIO().getRange(m_request.getParams().getModality()));
    return mocca::make_unique<GetRangeReply>(params, m_request.getRid(), m_session->getSid());
}

GetTotalBrickCountHdl::GetTotalBrickCountHdl(const GetTotalBrickCountRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetTotalBrickCountHdl::execute() {
    GetTotalBrickCountCmd::ReplyParams params(m_session->getIO().getTotalBrickCount());
    return mocca::make_unique<GetTotalBrickCountReply>(params, m_request.getRid(), m_session->getSid());
}

GetBrickHdl::GetBrickHdl(const GetBrickRequest& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> GetBrickHdl::execute() {
    bool mysteriousFlag;
    GetBrickCmd::ReplyParams params(m_session->getIO().getBrick(m_request.getParams().getBrickKey(), mysteriousFlag));
    return mocca::make_unique<GetBrickReply>(params, m_request.getRid(), m_session->getSid());
}

#undef PYTHON_MAGIC