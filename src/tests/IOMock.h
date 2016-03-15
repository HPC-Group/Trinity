#include "gmock/gmock.h"

#include "common/IIO.h"

class IOMock : public trinity::IIO {
public:
    MOCK_CONST_METHOD0(getLODLevelCount, int());
    MOCK_CONST_METHOD0(getMaxBrickSize, Core::Math::Vec3ui64());
    MOCK_CONST_METHOD0(getMaxUsedBrickSizes, Core::Math::Vec3ui64());
    MOCK_CONST_METHOD1(maxMinForKey, trinity::MinMaxBlock(const trinity::BrickKey&));
    MOCK_CONST_METHOD0(getNumberOfTimesteps, uint64_t());
};