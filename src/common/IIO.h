#pragma once

#include "commands/TransferFunction1D.h"

#include "silverbullet/dataio/base/Brick.h"
#include "silverbullet/math/MinMaxBlock.h"
#include "silverbullet/math/Vectors.h"

#include "mocca/base/BidirectionalMap.h"

namespace trinity {

  class IIO {
  public:
    virtual ~IIO() {}

    enum class Semantic { Scalar, Vector, Color };
    enum class ValueType { T_FLOAT, T_DOUBLE, T_UINT8, T_UINT16, T_UINT32, T_UINT64, T_INT8, T_INT16, T_INT32, T_INT64 };

    using SemanticMapper = mocca::BidirectionalMap<Semantic, std::string>;
    using ValueTypeMapper = mocca::BidirectionalMap<ValueType, std::string>;

    static const SemanticMapper& semanticMapper();
    static const ValueTypeMapper& valueTypeMapper();


    // changed from Vec3ui to Vec3ui64 (who did that? Andre?)
    virtual Core::Math::Vec3ui64 getMaxBrickSize() const = 0;
    virtual Core::Math::Vec3ui64 getMaxUsedBrickSizes() const = 0;
    virtual MinMaxBlock maxMinForKey(const BrickKey&) const = 0;
    virtual uint64_t getLODLevelCount(uint64_t modality) const = 0;
    virtual uint64_t getNumberOfTimesteps() const = 0;
    virtual Core::Math::Vec3ui64 getDomainSize(uint64_t lod, uint64_t modality) const = 0;
    virtual Core::Math::Vec3f getDomainScale(uint64_t modality) const = 0;
    virtual Core::Math::Mat4d getTransformation(uint64_t modality) const = 0;
    virtual Core::Math::Vec3ui getBrickOverlapSize() const = 0;
    virtual uint64_t getLargestSingleBrickLOD(uint64_t modality) const = 0;
    virtual Core::Math::Vec3ui getBrickVoxelCounts(const BrickKey&) const = 0;
    virtual Core::Math::Vec3f getBrickExtents(const BrickKey&) const = 0;
    virtual Core::Math::Vec3ui getBrickLayout(uint64_t lod, uint64_t modality) const = 0;
    virtual uint64_t getModalityCount() const = 0;
    virtual uint64_t getComponentCount(uint64_t modality) const = 0;
    virtual Core::Math::Vec2f getRange(uint64_t modality) const = 0;
    virtual uint64_t getTotalBrickCount(uint64_t modality) const = 0;
    virtual bool getBrick(const BrickKey& brickKey, std::vector<uint8_t>& data) const = 0;
    virtual ValueType getType(uint64_t modality) const = 0;
    virtual Semantic getSemantic(uint64_t modality) const = 0;
    virtual uint64_t getDefault1DTransferFunctionCount() const = 0;
    virtual uint64_t getDefault2DTransferFunctionCount() const = 0;
    virtual std::vector<uint64_t> get1DHistogram() const = 0;
    virtual std::vector<uint64_t> get2DHistogram() const = 0;
    virtual TransferFunction1D getDefault1DTransferFunction(uint64_t index) const = 0;
    virtual std::string getUserDefinedSemantic(uint64_t modality) const = 0;
  };
}