#include "gmock/gmock.h"

#include "common/IIO.h"

class IOMock : public trinity::IIO {
public:
    MOCK_CONST_METHOD0(getLODLevelCount, int());
    MOCK_CONST_METHOD0(getMaxBrickSize, Core::Math::Vec3ui64());
    MOCK_CONST_METHOD0(getMaxUsedBrickSizes, Core::Math::Vec3ui64());
    MOCK_CONST_METHOD1(maxMinForKey, trinity::MinMaxBlock(const trinity::BrickKey&));
    MOCK_CONST_METHOD0(getNumberOfTimesteps, uint64_t());
    MOCK_CONST_METHOD2(getDomainSize, Core::Math::Vec3ui64(const uint64_t, const uint64_t));
    MOCK_CONST_METHOD1(getTransformation, Core::Math::Mat4d(uint64_t));
    MOCK_CONST_METHOD0(getBrickOverlapSize, Core::Math::Vec3ui());
    MOCK_CONST_METHOD1(getLargestSingleBrickLOD, uint64_t(uint64_t));
    MOCK_CONST_METHOD1(getBrickVoxelCounts, Core::Math::Vec3ui(const trinity::BrickKey&));
    MOCK_CONST_METHOD1(getBrickExtents, Core::Math::Vec3f(const trinity::BrickKey&));
    MOCK_CONST_METHOD2(getBrickLayout, Core::Math::Vec3ui(uint64_t, uint64_t));
    MOCK_CONST_METHOD0(getModalityCount, uint64_t());
    MOCK_CONST_METHOD1(getComponentCount, uint64_t(uint64_t));
    MOCK_CONST_METHOD1(getRange, Core::Math::Vec2f(uint64_t));
    MOCK_CONST_METHOD0(getTotalBrickCount, uint64_t());
    MOCK_CONST_METHOD2(getBrick, bool(const trinity::BrickKey&, std::vector<uint8_t>&));
    MOCK_CONST_METHOD0(getType, IIO::ValueType());
    MOCK_CONST_METHOD1(getSemantic, IIO::Semantic(uint64_t));
};