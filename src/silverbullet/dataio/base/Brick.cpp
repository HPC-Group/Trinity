#include "silverbullet/dataio/base/Brick.h"

using namespace trinity;

bool BrickKey::equals(const BrickKey& other) const {
    return modality == other.modality && timestep == other.timestep && lod == other.lod && index == other.index;
}

void BrickKey::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", modality);
    writer.appendInt("timestep", timestep);
    writer.appendInt("lod", lod);
    writer.appendInt("index", index);
}

void BrickKey::deserialize(const ISerialReader& reader) {
    modality = reader.getUInt64("modality");
    timestep = reader.getUInt64("timestep");
    lod = reader.getUInt64("lod");
    index = reader.getUInt64("index");
}

bool trinity::operator==(const BrickKey& lhs, const BrickKey& rhs) {
    return lhs.equals(rhs);
}