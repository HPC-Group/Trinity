#pragma once

#include "commands/ISerializable.h"

#include "silverbullet/math/Vectors.h"

namespace trinity {

struct BrickMetaData {

    double minScalar;
    double maxScalar;
    double minGradient;
    double maxGradient;
    Core::Math::Vec3ui voxelSize;

    static std::vector<std::shared_ptr<std::vector<uint8_t>>> createBinary(const std::vector<BrickMetaData>& metaDataVec);
    static std::vector<BrickMetaData> createFromBinary(const std::vector<std::shared_ptr<std::vector<uint8_t>>>& binary);
};
}