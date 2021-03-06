#include "silverbullet/dataio/base/Brick.h"

bool BrickKey::equals(const BrickKey& other) const {
    return modality == other.modality && timestep == other.timestep && lod == other.lod && index == other.index;
}

std::string BrickKey::toString() const {
    std::stringstream stream;
    stream << "modality: " << modality << "; timestep: " << timestep << "; lod: " << lod << "; index: " << index;
    return stream.str();
}

std::string BrickKey::toFileDesc() const {
  std::stringstream stream;
  stream << modality << "_" << timestep << "_" << lod << "_" << index;
  return stream.str();
}


void BrickKey::serialize(trinity::ISerialWriter& writer) const {
    writer.appendInt("modality", modality);
    writer.appendInt("timestep", timestep);
    writer.appendInt("lod", lod);
    writer.appendInt("index", index);
}

void BrickKey::deserialize(const trinity::ISerialReader& reader) {
    modality = reader.getUInt64("modality");
    timestep = reader.getUInt64("timestep");
    lod = reader.getUInt64("lod");
    index = reader.getUInt64("index");
}

bool operator==(const BrickKey& lhs, const BrickKey& rhs) {
    return lhs.equals(rhs);
}

std::ostream& operator<<(std::ostream& os, const BrickKey& obj) {
    return os << obj.toString();
}