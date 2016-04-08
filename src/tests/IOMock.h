#include "gmock/gmock.h"

#include "common/IIO.h"

class IOMock : public trinity::IIO {
public:
    MOCK_CONST_METHOD1(getLODLevelCount, uint64_t(uint64_t));
    MOCK_CONST_METHOD0(getMaxBrickSize, Core::Math::Vec3ui64());
    MOCK_CONST_METHOD0(getMaxUsedBrickSizes, Core::Math::Vec3ui64());
    MOCK_CONST_METHOD1(getMaxMinForKey, MinMaxBlock(const BrickKey&));
    MOCK_CONST_METHOD0(getNumberOfTimesteps, uint64_t());
    MOCK_CONST_METHOD2(getDomainSize, Core::Math::Vec3ui64(const uint64_t, const uint64_t));
    MOCK_CONST_METHOD1(getDomainScale, Core::Math::Vec3f(uint64_t));
    MOCK_CONST_METHOD1(getTransformation, Core::Math::Mat4d(uint64_t));
    MOCK_CONST_METHOD0(getBrickOverlapSize, Core::Math::Vec3ui());
    MOCK_CONST_METHOD1(getLargestSingleBrickLOD, uint64_t(uint64_t));
    MOCK_CONST_METHOD1(getBrickVoxelCounts, Core::Math::Vec3ui(const BrickKey&));
    MOCK_CONST_METHOD1(getBrickExtents, Core::Math::Vec3f(const BrickKey&));
    MOCK_CONST_METHOD2(getBrickLayout, Core::Math::Vec3ui(uint64_t, uint64_t));
    MOCK_CONST_METHOD2(getFloatBrickLayout, Core::Math::Vec3f(uint64_t, uint64_t));
    MOCK_CONST_METHOD2(getBrickMaxMin, std::vector<MinMaxBlock>(uint64_t, uint64_t));
    MOCK_CONST_METHOD0(getModalityCount, uint64_t());
    MOCK_CONST_METHOD1(getComponentCount, uint64_t(uint64_t));
    MOCK_CONST_METHOD1(getRange, Core::Math::Vec2f(uint64_t));
    MOCK_CONST_METHOD1(getTotalBrickCount, uint64_t(uint64_t));
    MOCK_CONST_METHOD2(getBrick, std::shared_ptr<std::vector<uint8_t>>(const BrickKey&, bool&));
    MOCK_CONST_METHOD1(getType, IIO::ValueType(uint64_t));
    MOCK_CONST_METHOD1(getSemantic, IIO::Semantic(uint64_t));
    MOCK_CONST_METHOD0(getDefault1DTransferFunctionCount, uint64_t());
    MOCK_CONST_METHOD0(getDefault2DTransferFunctionCount, uint64_t());
    MOCK_CONST_METHOD0(get1DHistogram, std::vector<uint64_t>());
    MOCK_CONST_METHOD0(get2DHistogram, std::vector<uint64_t>());
    MOCK_CONST_METHOD1(getDefault1DTransferFunction, TransferFunction1D(uint64_t));
    MOCK_CONST_METHOD1(getUserDefinedSemantic, std::string(uint64_t));
};