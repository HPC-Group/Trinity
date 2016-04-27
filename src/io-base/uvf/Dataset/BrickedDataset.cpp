#include <cassert>
#include <stdexcept>
#include "BrickedDataset.h"

#include "mocca/log/LogManager.h"

BrickedDataset::BrickedDataset() { }
BrickedDataset::~BrickedDataset() { }

void BrickedDataset::NBricksHint(size_t n) {
  // The following line implements bricks.reserve(n);
  // in a portable way. unordered_map does not define
  // a reserve function but is seems gcc's tr1 does
  bricks.rehash(size_t(ceil(float(n) / bricks.max_load_factor())));
}

/// Adds a brick to the dataset.
void BrickedDataset::AddBrick(const BrickKey& bk,
                              const BrickMD& brick)
{
  this->bricks.insert(std::make_pair(bk, brick));
}

/// Looks up the spatial range of a brick.
Core::Math::Vec3f BrickedDataset::GetBrickExtents(const BrickKey &bk) const
{
  BrickTable::const_iterator iter = this->bricks.find(bk);
  if(iter == this->bricks.end()) {
    LERROR("Unknown brick");
    return Core::Math::Vec3f(0.0f, 0.0f, 0.0f);
  }
  return iter->second.extents;
}
Core::Math::Vec3ui BrickedDataset::GetBrickVoxelCounts(const BrickKey& bk) const {
  BrickTable::const_iterator iter = this->bricks.find(bk);
  if(this->bricks.end() == iter) {
    throw std::domain_error("unknown brick.");
  }
  return iter->second.n_voxels;
}

/// @return an iterator that can be used to visit every brick in the dataset.
BrickTable::const_iterator BrickedDataset::BricksBegin() const
{
  return this->bricks.begin();
}

BrickTable::const_iterator BrickedDataset::BricksEnd() const
{
  return this->bricks.end();
}

/// @return the number of bricks at the given LOD.
BrickTable::size_type BrickedDataset::GetBrickCount(size_t lod, size_t ts) const
{
  BrickTable::size_type count = 0;
  BrickTable::const_iterator iter = this->bricks.begin();
  for(; iter != this->bricks.end(); ++iter) {
    if((iter->first).timestep == ts &&
       (iter->first).lod == lod) {
      ++count;
    }
  }
  return count;
}

size_t BrickedDataset::GetLargestSingleBrickLOD(size_t ts) const {
  const size_t n_lods = this->GetLODLevelCount();
  for(size_t lod=0; lod < n_lods; ++lod) {
    if(this->GetBrickCount(lod, ts) == 1) {
      return lod;
    }
  }
  assert("not reachable");
  return 0;
}

uint64_t BrickedDataset::GetTotalBrickCount() const {
  return static_cast<uint64_t>(this->bricks.size());
}

const BrickMD& BrickedDataset::GetBrickMetadata(const BrickKey& k) const {
  return this->bricks.find(k)->second;
}

// we don't actually know how the user bricked the data set here; only a
// derived class would know.  so, calculate it instead.
Core::Math::Vec3ui BrickedDataset::GetMaxBrickSize() const {
  return this->GetMaxUsedBrickSizes();
}
Core::Math::Vec3ui BrickedDataset::GetMaxUsedBrickSizes() const {
  Core::Math::Vec3ui bsize(0,0,0);
  for(auto b=this->bricks.begin(); b != this->bricks.end(); ++b) {
    bsize.StoreMax(b->second.n_voxels);
  }
  return bsize;
}

bool
BrickedDataset::BrickIsFirstInDimension(size_t dim, const BrickKey& k) const
{
  assert(dim <= 3);
  const BrickMD& md = this->bricks.find(k)->second;
  for(BrickTable::const_iterator iter = this->BricksBegin();
      iter != this->BricksEnd(); ++iter) {
    if(iter->second.center[dim] < md.center[dim]) {
      return false;
    }
  }
  return true;
}

bool
BrickedDataset::BrickIsLastInDimension(size_t dim, const BrickKey& k) const
{
  assert(dim <= 3);
  const BrickMD& md = this->bricks.find(k)->second;
  for(BrickTable::const_iterator iter = this->BricksBegin();
      iter != this->BricksEnd(); ++iter) {
    if(iter->second.center[dim] > md.center[dim]) {
      return false;
    }
  }
  return true;
}

void BrickedDataset::Clear() {
  LINFO("Clearing brick metadata.");
  bricks.clear();
}
