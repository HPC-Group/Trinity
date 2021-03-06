#include "IOSessionProxy.h"
#include <thread>

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "common/ProxyUtils.h"
#include "common/TrinityError.h"

#include "mocca/log/LogManager.h"

using namespace trinity;

IOSessionProxy::IOSessionProxy(const int remoteSid, const mocca::net::Endpoint& ioEndpoint, CompressionMode compressionMode)
    : m_inputChannel(ioEndpoint, compressionMode)
    , m_remoteSid(remoteSid) {
    if (!connectInputChannel(m_inputChannel)) {
        throw TrinityError("Error connecting to IO session", __FILE__, __LINE__);
    }
}

Core::Math::Vec3ui64 IOSessionProxy::getMaxBrickSize() const {
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

uint64_t IOSessionProxy::getLODLevelCount(uint64_t modality) const {
    GetLODLevelCountCmd::RequestParams params(modality);
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

Core::Math::Vec3ui64 IOSessionProxy::getDomainSize(uint64_t lod, uint64_t modality) const {
    GetDomainSizeCmd::RequestParams params(lod, modality);
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

uint64_t IOSessionProxy::getLargestSingleBrickLOD(uint64_t modality) const {
    GetLargestSingleBrickLODCmd::RequestParams params(modality);
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

Core::Math::Vec3f IOSessionProxy::getBrickExtents(const BrickKey& brickKey) const {
    GetBrickExtentsCmd::RequestParams params(brickKey);
    GetBrickExtentsRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getBrickExtents();
}

Core::Math::Vec3ui IOSessionProxy::getBrickLayout(uint64_t lod, uint64_t modality) const {
    GetBrickLayoutCmd::RequestParams params(lod, modality);
    GetBrickLayoutRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getBrickLayout();
}

uint64_t IOSessionProxy::getModalityCount() const {
    GetModalityCountCmd::RequestParams params;
    GetModalityCountRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getModalityCount();
}

uint64_t IOSessionProxy::getComponentCount(uint64_t modality) const {
    GetComponentCountCmd::RequestParams params(modality);
    GetComponentCountRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getComponentCount();
}

Core::Math::Vec2f IOSessionProxy::getRange(uint64_t modality) const {
    GetRangeCmd::RequestParams params(modality);
    GetRangeRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getRange();
}

uint64_t IOSessionProxy::getTotalBrickCount(uint64_t modality) const {
    GetTotalBrickCountCmd::RequestParams params(modality);
    GetTotalBrickCountRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getTotalBrickCount();
}

std::shared_ptr<std::vector<uint8_t>> IOSessionProxy::getBrick(const BrickKey& brickKey, bool& success) const {
    GetBrickCmd::RequestParams params(brickKey);
    GetBrickRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    const auto replyParams = reply->getParams();
    success = replyParams.getSuccess();
    return replyParams.getBrick();
}

IIO::ValueType IOSessionProxy::getType(uint64_t modality) const {
    GetTypeCmd::RequestParams params(modality);
    GetTypeRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getValueType();
}

IIO::Semantic IOSessionProxy::getSemantic(uint64_t modality) const {
    GetSemanticCmd::RequestParams params(modality);
    GetSemanticRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getSemantic();
}

uint64_t IOSessionProxy::getDefault1DTransferFunctionCount() const {
    GetDefault1DTransferFunctionCountCmd::RequestParams params;
    GetDefault1DTransferFunctionCountRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getCount();
}

uint64_t IOSessionProxy::getDefault2DTransferFunctionCount() const {
    GetDefault2DTransferFunctionCountCmd::RequestParams params;
    GetDefault2DTransferFunctionCountRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getCount();
}

std::vector<uint64_t> IOSessionProxy::get1DHistogram() const {
    Get1DHistogramCmd::RequestParams params;
    Get1DHistogramRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getHistogram();
}

std::vector<uint64_t> IOSessionProxy::get2DHistogram() const {
    Get2DHistogramCmd::RequestParams params;
    Get2DHistogramRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getHistogram();
}

std::string trinity::IOSessionProxy::getUserDefinedSemantic(uint64_t modality) const {
    GetUserDefinedSemanticCmd::RequestParams params(modality);
    GetUserDefinedSemanticRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getSemantic();
}

TransferFunction1D IOSessionProxy::getDefault1DTransferFunction(uint64_t index) const {
    GetDefault1DTransferFunctionCmd::RequestParams params(index);
    GetDefault1DTransferFunctionRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getFunction();
}

Core::Math::Vec3f IOSessionProxy::getDomainScale(uint64_t modality) const {
    GetDomainScaleCmd::RequestParams params(modality);
    GetDomainScaleRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

Core::Math::Vec3f IOSessionProxy::getFloatBrickLayout(uint64_t lod, uint64_t modality) const {
    GetFloatBrickLayoutCmd::RequestParams params(lod, modality);
    GetFloatBrickLayoutRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}

std::vector<BrickMetaData> IOSessionProxy::getBrickMetaData(uint64_t modality, uint64_t timestep) const {
    GetBrickMetaDataCmd::RequestParams params(modality, timestep);
    GetBrickMetaDataRequest request(params, IDGenerator::nextID(), m_remoteSid);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->releaseParams().releaseResult();
}

/* AUTOGEN IOSessionProxyImpl */