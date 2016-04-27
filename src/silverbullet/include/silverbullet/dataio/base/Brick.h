#pragma once

#include "silverbullet/base/StdDefines.h"

#include "silverbullet/math/Vectors.h"

#include "commands/ISerializable.h"

#include <functional>
#include <memory>
#include <tuple>
#include <unordered_map>
#include <utility>

using Core::Math::Vec3f;
using Core::Math::Vec3ui;

/// Datasets are organized as a set of bricks, stored in a hash table.  A key
/// into this table consists of an LOD index plus a brick index.
/// An element in the table contains
/// brick metadata, but no data; to obtain the data one must query the dataset.

struct BrickKey : public trinity::SerializableTemplate<BrickKey> {
  uint64_t modality;
  uint64_t timestep;
  uint64_t lod;
  uint64_t index;
  
  BrickKey() = default;
  BrickKey(uint64_t m, uint64_t t, uint64_t l, uint64_t i) : modality(m), timestep(t), lod(l), index(i) {}
  
  bool equals(const BrickKey& other) const;
  std::string toString() const;
  std::string toFileDesc() const;
  
  void serialize(trinity::ISerialWriter& writer) const;
  void deserialize(const trinity::ISerialReader& reader);
};

bool operator==(const BrickKey& lhs, const BrickKey& rhs);
std::ostream& operator<<(std::ostream& os, const BrickKey& obj);

struct BrickMD {
  Vec3f center; ///< center of the brick, in world coords
  Vec3f extents; ///< width/height/depth of the brick.
  Vec3ui  n_voxels; ///< number of voxels, per dimension.
};
struct BKeyHash : std::unary_function<BrickKey, std::size_t> {
  uint64_t operator()(const BrickKey& bk) const {
    uint64_t ts    = std::hash<uint64_t>()(bk.timestep);
    uint64_t h_lod = std::hash<uint64_t>()(bk.lod);
    uint64_t brick = std::hash<uint64_t>()(bk.index);
    uint64_t seed = h_lod;
    seed ^= brick + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= ts + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
};
typedef std::unordered_map<BrickKey, BrickMD, BKeyHash> BrickTable;
