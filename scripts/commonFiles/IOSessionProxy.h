#pragma once

#include "common/IIO.h"
#include "commands/CommandInputChannel.h"

#include "mocca/net/Endpoint.h"
#include "mocca/net/IMessageConnection.h"

#include <memory>


namespace trinity {

class IOSessionProxy : public IIO {

public:
    IOSessionProxy(const int remoteSid, const mocca::net::Endpoint& ioEndpoint);
    Core::Math::Vec3ui64 getMaxBrickSize() const override;
    Core::Math::Vec3ui64 getMaxUsedBrickSizes() const override;
    MinMaxBlock maxMinForKey(const BrickKey&) const override;
    uint64_t getLODLevelCount(uint64_t modality) const override;
    uint64_t getNumberOfTimesteps() const override;
    Core::Math::Vec3ui64 getDomainSize(uint64_t lod, uint64_t modality) const override;
    Core::Math::Mat4d getTransformation(uint64_t modality) const override;
    Core::Math::Vec3ui getBrickOverlapSize() const override;
    uint64_t getLargestSingleBrickLOD(uint64_t modality) const override;
    Core::Math::Vec3ui getBrickVoxelCounts(const BrickKey& brickKey) const override;
    Core::Math::Vec3f getBrickExtents(const BrickKey & brickKey) const override;
    Core::Math::Vec3ui getBrickLayout(uint64_t lod, uint64_t modality) const override;
    uint64_t getModalityCount() const override;
    uint64_t getComponentCount(uint64_t modality) const override;
    Core::Math::Vec2f getRange(uint64_t modality) const override;
    uint64_t getTotalBrickCount(uint64_t modality) const override;
    bool getBrick(const BrickKey& brickKey, std::vector<uint8_t>& data) const override;
    IIO::ValueType getType(uint64_t modality) const override;
    IIO::Semantic getSemantic(uint64_t modality) const override;
    uint64_t getDefault1DTransferFunctionCount() const override;
    uint64_t getDefault2DTransferFunctionCount() const override;
    std::vector<uint64_t> get1DHistogram() const override;
    std::vector<uint64_t> get2DHistogram() const override;
    TransferFunction1D getDefault1DTransferFunction(uint64_t index) const override;
    std::string getUserDefinedSemantic(uint64_t modality) const override;
#define PYTHON_MAGIC


#undef PYTHON_MAGIC

private:
    CommandInputChannel m_inputChannel;
    const int m_remoteSid;
};
}