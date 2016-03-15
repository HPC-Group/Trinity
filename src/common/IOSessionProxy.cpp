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

int IOSessionProxy::getLODLevelCount() const {
    GetLODLevelCountCmd::RequestParams params;
    GetLODLevelCountRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getLODLevelCount();
}

Core::Math::Vec3ui64 IOSessionProxy::getMaxBrickSize() const  {
    GetMaxBrickSizeCmd::RequestParams params;
    GetMaxBrickSizeRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getMaxBrickSize();
}

//Core::Math::Vec3ui64 IOSessionProxy::getMaxUsedBrickSizes() const {
//    // TODO
//    Core::Math::Vec3ui64 x; return x;
//}
//
//MinMaxBlock IOSessionProxy::maxMinForKey(const BrickKey&) const {
//    // TODO
//    MinMaxBlock x; return x;
//}
//
//uint64_t IOSessionProxy::getNumberOfTimesteps() const {
//    // TODO
//    uint64_t x; return x;
//}
//
//Core::Math::Vec3ui64 IOSessionProxy::getDomainSize(const uint64_t lod, const uint64_t ts) const {
//    // TODO
//    Core::Math::Vec3ui64 x; return x;
//}
//
//Core::Math::Mat4d IOSessionProxy::getTransformation() const {
//    // TODO
//    Core::Math::Mat4d x; return x;
//}
//
//Core::Math::Vec3ui IOSessionProxy::getBrickOverlapSize() const {
//    // TODO
//    Core::Math::Vec3ui x; return x;
//}
//
//uint64_t IOSessionProxy::getLargestSingleBrickLOD(uint64_t ts) const {
//    // TODO
//    uint64_t x; return x;
//}
//
//Core::Math::Vec3ui IOSessionProxy::getBrickVoxelCounts(const BrickKey&) const {
//    // TODO
//    Core::Math::Vec3ui x; return x;
//}
//
//Vec3f IOSessionProxy::getBrickExtents(const BrickKey &) const {
//    // TODO
//    Vec3f x; return x;
//}
//
//Core::Math::Vec3ui IOSessionProxy::getBrickLayout(uint64_t LoD, uint64_t timestep) const {
//    // TODO
//    Core::Math::Vec3ui x; return x;
//}
//
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