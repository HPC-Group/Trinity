#include <cassert>
#include "LinearIndexDataset.h"

// given a (x,y,z,LOD) tuple, convert that to one of our Brickkeys, which use a
// 1D (brick) index internally.
BrickKey LinearIndexDataset::IndexFrom4D(const Core::Math::Vec4ui& four,
                                         size_t timestep) const {
  // the fourth component represents the LOD.
  const size_t lod = static_cast<size_t>(four.w);
  Core::Math::Vec3ui layout = this->GetBrickLayout(lod, timestep);
  
  const BrickKey k = BrickKey(0, timestep, lod, four.x +
                              four.y*layout.x +
                              four.z*layout.x*layout.y);
  // it must be an actual brick we know about!
  assert(this->bricks.find(k) != this->bricks.end());
  return k;
}

// our brick Keys have 1D indices internally; compute the (x,y,z,LOD) tuple
// index based on the 1D index and the dataset size.
Core::Math::Vec4ui LinearIndexDataset::IndexTo4D(const BrickKey& key) const {
  assert(this->bricks.find(key) != this->bricks.end());
  
  const size_t timestep = key.timestep;
  const size_t lod = key.lod;
  const size_t idx1d = key.index;
  
  const Core::Math::Vec3ui layout = this->GetBrickLayout(lod, timestep);
  const Core::Math::Vec4ui rv = Core::Math::Vec4ui(idx1d % layout.x,
                                                   (idx1d / layout.x) % layout.y,
                                                   idx1d / (layout.x*layout.y),
                                                   lod
                                                   );
  assert(rv[2] < layout[2]);
  return rv;
}
