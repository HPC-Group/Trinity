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
    int getLODLevelCount() const override;
    uint64_t getNumberOfTimesteps() const override;
    Core::Math::Vec3ui64 getDomainSize(uint64_t lod, uint64_t modality, uint64_t ts) const override;
    Core::Math::Mat4d getTransformation() const override;
    Core::Math::Vec3ui getBrickOverlapSize() const override;


    //uint64_t getLargestSingleBrickLOD(uint64_t ts) const override;
    //Core::Math::Vec3ui getBrickVoxelCounts(const BrickKey&) const override;
    //Vec3f getBrickExtents(const BrickKey &) const override;
    //Core::Math::Vec3ui getBrickLayout(uint64_t LoD, uint64_t timestep) const override;
    //uint64_t getModalityCount() const override;
    //uint64_t getComponentCount(uint64_t modality) const override;
    //Core::Math::Vec2f getRange() const override;
    //uint64_t getTotalBrickCount() const override;
    //bool getBrick(const BrickKey&, std::vector<uint8_t>&) const override;

private:
    CommandInputChannel m_inputChannel;
    const int m_remoteSid;
};
}