#pragma once

#include "silverbullet/math/MinMaxBlock.h"
#include "Dataset.h"

/// Base for data sets which split their data into blocks.  All bricks are kept
/// into an internal table; derived classes should add to it via AddBrick.
/// This class then handles the query of much meta data.
class BrickedDataset : public Dataset {
public:
  BrickedDataset();
  virtual ~BrickedDataset();

  /// Looks up the spatial range of a brick.
  virtual Core::Math::Vec3f GetBrickExtents(const BrickKey &) const;
  /// number of voxels in the given brick.
  virtual Core::Math::Vec3ui GetBrickVoxelCounts(const BrickKey&) const;

  /// @return an iterator that can be used to visit every brick in the dataset.
  virtual BrickTable::const_iterator BricksBegin() const;
  virtual BrickTable::const_iterator BricksEnd() const;
  /// @return the number of bricks at the given LOD + timestep
  virtual BrickTable::size_type GetBrickCount(size_t lod, size_t ts) const;
  virtual size_t GetLargestSingleBrickLOD(size_t ts) const;
  virtual uint64_t GetTotalBrickCount() const;

  virtual const BrickMD& GetBrickMetadata(const BrickKey&) const;

  /// @returns the bricking size used for this decomposition
  virtual Core::Math::Vec3ui GetMaxBrickSize() const;
  /// @returns the largest brick which *actually* got used when splitting up
  /// the data set.  This can be smaller than GetMaxBrickSize, when, for
  /// example, the domain is smaller than the brick size requested.
  virtual Core::Math::Vec3ui GetMaxUsedBrickSizes() const;
  /// @returns the min/max scalar and gradient values for the given brick.
  virtual MinMaxBlock MaxMinForKey(const BrickKey&) const=0;

  virtual void Clear();

  /// It can be important to know whether the given brick is the first or last
  /// along any particular axis.  As an example, there's 0 brick overlap for a
  /// border brick.
  ///@{
  /// @return true if the brick is the minimum brick in the given dimension.
  virtual bool BrickIsFirstInDimension(size_t, const BrickKey&) const;
  /// @return true if the brick is the maximum brick in the given dimension.
  virtual bool BrickIsLastInDimension(size_t, const BrickKey&) const;
  ///@}

protected:
  /// gives a hint to this object that we'll have 'n' bricks in the end.
  virtual void NBricksHint(size_t n);

  /// Adds a brick to the dataset.
  virtual void AddBrick(const BrickKey&, const BrickMD&);

protected:
  BrickTable bricks;
};
