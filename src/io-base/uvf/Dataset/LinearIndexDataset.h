#pragma once

#include "BrickedDataset.h"

/// A LinearIndexDataset is simply a bricked dataset with a particular
/// algorithm for how the indexing is performed.  Namely, the 1D index is
/// actually the linearization of a 4D (LOD, z,y,x, from slowest to fasting
/// moving dimension) index.
/// This necessary implies that there are no 'holes' in the data: if a brick
/// <0, 1,0,0> exists, than the brick <0, 0,0,0> must exist as well.
class LinearIndexDataset : public BrickedDataset {
  public:
    LinearIndexDataset() {}
    virtual ~LinearIndexDataset() {}

    /// @returns the brick layout for a given LoD.  This is the number of
    /// bricks which exist (given per-dimension)
    virtual Core::Math::Vec3ui GetBrickLayout(size_t LoD, size_t timestep) const=0;

    /// @returns the brick key (1D brick index) derived from the 4D key.
    virtual BrickKey IndexFrom4D(const Core::Math::Vec4ui& four,
                                 size_t timestep) const;
    virtual Core::Math::Vec4ui IndexTo4D(const BrickKey& key) const;
};
