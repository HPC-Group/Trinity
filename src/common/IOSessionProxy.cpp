#include "IOSessionProxy.h"
#include <thread>

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "common/ProxyUtils.h"
#include "common/TrinityError.h"

#include "mocca/log/LogManager.h"


using namespace trinity;

IOSessionProxy::IOSessionProxy(const int remoteSid, const mocca::net::Endpoint& ioEndpoint)
    : m_inputChannel(ioEndpoint)
    , m_remoteSid(remoteSid) {
    if (!connectInputChannel(m_inputChannel)) {
        throw TrinityError("Error connecting to IO session", __FILE__, __LINE__);
    }
}

Core::Math::Vec3ui64 IOSessionProxy::getMaxBrickSize() const  {
    GetMaxBrickSizeCmd::RequestParams params;
    GetMaxBrickSizeRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getMaxBrickSize();
}

Core::Math::Vec3ui64 IOSessionProxy::getMaxUsedBrickSizes() const {
    GetMaxUsedBrickSizesCmd::RequestParams params;
    GetMaxUsedBrickSizesRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getMaxUsedBrickSizes();
}

MinMaxBlock IOSessionProxy::maxMinForKey(const BrickKey& brickKey) const {
    MaxMinForKeyCmd::RequestParams params(brickKey);
    MaxMinForKeyRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getMinMaxBlock();
}

int IOSessionProxy::getLODLevelCount() const {
    GetLODLevelCountCmd::RequestParams params;
    GetLODLevelCountRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getLODLevelCount();
}

uint64_t IOSessionProxy::getNumberOfTimesteps() const {
    GetNumberOfTimestepsCmd::RequestParams params;
    GetNumberOfTimestepsRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getNumberOfTimesteps();
}

Core::Math::Vec3ui64 IOSessionProxy::getDomainSize(uint64_t lod, uint64_t modality, uint64_t ts) const {
    GetDomainSizeCmd::RequestParams params(lod, modality, ts);
    GetDomainSizeRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getDomainSize();
}

Core::Math::Mat4d IOSessionProxy::getTransformation(uint64_t modality) const {
    GetTransformationCmd::RequestParams params(modality);
    GetTransformationRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getTransformation();
}

Core::Math::Vec3ui IOSessionProxy::getBrickOverlapSize() const {
    GetBrickOverlapSizeCmd::RequestParams params;
    GetBrickOverlapSizeRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getOverlapSize();
}

uint64_t IOSessionProxy::getLargestSingleBrickLOD(uint64_t modality, uint64_t ts) const {
    GetLargestSingleBrickLODCmd::RequestParams params(modality, ts);
    GetLargestSingleBrickLODRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getLargestSingleBrickLOD();
}

Core::Math::Vec3ui IOSessionProxy::getBrickVoxelCounts(const BrickKey& brickKey) const {
    GetBrickVoxelCountsCmd::RequestParams params(brickKey);
    GetBrickVoxelCountsRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getBrickVoxelCounts();
}

Core::Math::Vec3f IOSessionProxy::getBrickExtents(const BrickKey & brickKey) const {
    GetBrickExtentsCmd::RequestParams params(brickKey);
    GetBrickExtentsRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getBrickExtents();
}

Core::Math::Vec3ui IOSessionProxy::getBrickLayout(uint64_t lod, uint64_t modality, uint64_t timestep) const {
    GetBrickLayoutCmd::RequestParams params(lod, modality, timestep);
    GetBrickLayoutRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getBrickLayout();
}

//uint64_t IOSessionProxy::getModalityCount() const {
//    // TODO
//    uint64_t x; return x;
//}
//
//uint64_t IOSessionProxy::getComponentCount(uint64_t modality) const {
//    // TODO
//    uint64_t x; return x;
//}
//
//Core::Math::Vec2f IOSessionProxy::getRange() const {
//    // TODO
//    Core::Math::Vec2f x; return x;
//}
//
//uint64_t IOSessionProxy::getTotalBrickCount() const {
//    // TODO
//    uint64_t x; return x;
//}
//
//bool IOSessionProxy::getBrick(const BrickKey&, std::vector<uint8_t>&) const {
//    // TODO
//    bool x; return x;
//}