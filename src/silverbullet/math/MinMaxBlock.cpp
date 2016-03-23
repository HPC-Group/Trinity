#include "silverbullet/math/MinMaxBlock.h"

using namespace trinity;

void MinMaxBlock::serialize(ISerialWriter& writer) const {
    writer.appendDouble("minScalar", minScalar);
    writer.appendDouble("maxScalar", maxScalar);
    writer.appendDouble("minGradient", minGradient);
    writer.appendDouble("maxGradient", maxGradient);
}

void MinMaxBlock::deserialize(const ISerialReader& reader) {
    minScalar = reader.getDouble("minScalar");
    maxScalar = reader.getDouble("maxScalar");
    minGradient = reader.getDouble("minGradient");
    maxGradient = reader.getDouble("maxGradient");
}

bool MinMaxBlock::equals(const MinMaxBlock& other) const {
    return minScalar == other.minScalar && maxScalar == other.maxScalar && minGradient == other.minGradient &&
           maxGradient == other.maxGradient;
}

std::string MinMaxBlock::toString() const {
    std::stringstream stream;
    stream << "minScalar: " << minScalar << "; maxScalar: " << maxScalar << "; minGradient: " << minGradient
           << "; maxGradient: " << maxGradient;
    return stream.str();
}

bool operator==(const MinMaxBlock& lhs, const MinMaxBlock& rhs) {
    return lhs.equals(rhs);
}

std::ostream& operator<<(std::ostream& os, const MinMaxBlock& obj) {
    return os << obj.toString();
}
