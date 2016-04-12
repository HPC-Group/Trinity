#pragma once

#include "commands/ISerializable.h"

#include "silverbullet/math/Vectors.h"

namespace trinity {

class BrickMetaData : public trinity::SerializableTemplate<BrickMetaData> {
public:
    double minScalar;
    double maxScalar;
    double minGradient;
    double maxGradient;
    Core::Math::Vec3ui voxelSize;

    BrickMetaData() = default;
    BrickMetaData(double minS, double maxS, double minG, double maxG, const Core::Math::Vec3ui& vs)
        : minScalar(minS), maxScalar(maxS), minGradient(minG), maxGradient(maxG), voxelSize(vs)
    {}

    void serialize(trinity::ISerialWriter& writer) const;
    void deserialize(const trinity::ISerialReader& reader);

    bool equals(const BrickMetaData& other) const;
    std::string toString() const;
};

bool operator==(const BrickMetaData& lhs, const BrickMetaData& rhs);
std::ostream& operator<<(std::ostream& os, const BrickMetaData& obj);
}