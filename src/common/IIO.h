#pragma once

#include "silverbullet/math/Vectors.h"
#include "silverbullet/dataio/base/Brick.h"
#include "silverbullet/math/MinMaxBlock.h"

namespace trinity {

class IIO {
public:
    virtual ~IIO() {}


    // changed from Vec3ui to Vec3ui64 (who did that? Andre?)
    virtual Core::Math::Vec3ui64 getMaxBrickSize() const = 0;
    virtual Core::Math::Vec3ui64 getMaxUsedBrickSizes() const = 0;
    virtual MinMaxBlock maxMinForKey(const BrickKey&) const = 0;
    virtual int getLODLevelCount() const = 0;
    virtual uint64_t getNumberOfTimesteps() const = 0;
    virtual Core::Math::Vec3ui64 getDomainSize(uint64_t lod, uint64_t modality, uint64_t ts) const = 0;
    virtual Core::Math::Mat4d getTransformation(uint64_t modality) const = 0;
    virtual Core::Math::Vec3ui getBrickOverlapSize() const = 0;
    virtual uint64_t getLargestSingleBrickLOD(uint64_t modality, uint64_t ts) const = 0;
    virtual Core::Math::Vec3ui getBrickVoxelCounts(const BrickKey&) const = 0;
    virtual Core::Math::Vec3f getBrickExtents(const BrickKey &) const = 0;
    virtual Core::Math::Vec3ui getBrickLayout(uint64_t lod, uint64_t modality, uint64_t timestep) const = 0;


    //virtual uint64_t getModalityCount() const = 0;
    //virtual uint64_t getComponentCount(uint64_t modality) const = 0;
    //virtual Core::Math::Vec2f getRange(uint64_t modality) const = 0;
    //virtual uint64_t getTotalBrickCount() const = 0;
    //virtual bool getBrick(const BrickKey&, std::vector<uint8_t>&) const = 0;

    // not sure if we need that
    // virtual ValueType getType() const;
    // virtual std::string getUserDefinedSemantic(uint64_t modality) const;
    // virtual Semantic getSemantict(uint64_t modality) const;
};
}