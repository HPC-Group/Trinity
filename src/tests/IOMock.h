#include "gmock/gmock.h"

#include "common/IIO.h"

class IOMock : public trinity::IIO {
public:
    MOCK_CONST_METHOD0(getLODLevelCount, int());
    MOCK_CONST_METHOD0(getMaxBrickSize, Core::Math::Vec3ui64());
    MOCK_CONST_METHOD0(getMaxUsedBrickSizes, Core::Math::Vec3ui64());
    MOCK_CONST_METHOD1(maxMinForKey, trinity::MinMaxBlock(const trinity::BrickKey&));
    MOCK_CONST_METHOD0(getNumberOfTimesteps, uint64_t());
    MOCK_CONST_METHOD3(getDomainSize, Core::Math::Vec3ui64(const uint64_t, const uint64_t, const uint64_t));
    MOCK_CONST_METHOD1(getTransformation, Core::Math::Mat4d(uint64_t));
    MOCK_CONST_METHOD0(getBrickOverlapSize, Core::Math::Vec3ui());
    MOCK_CONST_METHOD2(getLargestSingleBrickLOD, uint64_t(uint64_t, uint64_t));
    MOCK_CONST_METHOD1(getBrickVoxelCounts, Core::Math::Vec3ui(const trinity::BrickKey&));
    MOCK_CONST_METHOD1(getBrickExtents, Core::Math::Vec3f(const trinity::BrickKey&));
    MOCK_CONST_METHOD3(getBrickLayout, Core::Math::Vec3ui(uint64_t, uint64_t, uint64_t));
    MOCK_CONST_METHOD0(getModalityCount, uint64_t());
    MOCK_CONST_METHOD1(getComponentCount, uint64_t(uint64_t));
};