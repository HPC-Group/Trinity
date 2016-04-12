#include "commands/BrickMetaData.h"

#include "commands/ISerialReader.h"
#include "commands/ISerialWriter.h"

using namespace trinity;

void BrickMetaData::serialize(ISerialWriter& writer) const {
    writer.appendDouble("minScalar", minScalar);
    writer.appendDouble("maxScalar", maxScalar);
    writer.appendDouble("minGradient", minGradient);
    writer.appendDouble("maxGradient", maxGradient);
    writer.appendObject("voxelSize", voxelSize);
}

void BrickMetaData::deserialize(const ISerialReader& reader) {
    minScalar = reader.getDouble("minScalar");
    maxScalar = reader.getDouble("maxScalar");
    minGradient = reader.getDouble("minGradient");
    maxGradient = reader.getDouble("maxGradient");
    voxelSize = reader.getSerializable<Core::Math::Vec3ui>("voxelSize");
}

bool BrickMetaData::equals(const BrickMetaData& other) const {
    return minScalar == other.minScalar && maxScalar == other.maxScalar && minGradient == other.minGradient &&
           maxGradient == other.maxGradient && voxelSize == other.voxelSize;
}

std::string BrickMetaData::toString() const {
    std::stringstream stream;
    stream << "minScalar: " << minScalar << "; maxScalar: " << maxScalar << "; minGradient: " << minGradient
           << "; maxGradient: " << maxGradient << "; voxelSize: " << voxelSize;
    return stream.str();
}

bool trinity::operator==(const BrickMetaData& lhs, const BrickMetaData& rhs) {
    return lhs.equals(rhs);
}

std::ostream& trinity::operator<<(std::ostream& os, const BrickMetaData& obj) {
    return os << obj.toString();
}
