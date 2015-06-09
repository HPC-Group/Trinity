/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2008 Scientific Computing and Imaging Institute,
   University of Utah.


   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included
   in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/
/**
  \file    Dataset.h
  \author  Tom Fogal
           SCI Institute
           University of Utah
*/
#pragma once
#ifndef TUVOK_DATASET_H
#define TUVOK_DATASET_H

#include <Core/StdTuvokDefines.h>

#include <cstdlib>
#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>
#include <Core/Math/Grids.h>
#include "Brick.h"
using Core::Math::Vec3d;
using Core::Math::Vec3ui64;
using Core::Math::PLANE;

#define MAX_TRANSFERFUNCTION_SIZE 4096

typedef Core::Math::Grid1D<uint32_t> Histogram1D;
typedef Core::Math::Grid2D<uint32_t> Histogram2D;
class LargeRAWFile;

namespace Tuvok {

class Metadata;
class Mesh;

/// Abstract interface to a dataset.
class Dataset {

public:
  Dataset();
  virtual ~Dataset();

  /// noncopyable not because it wouldn't work, but because we might be holding a
  /// lot of data -- copying would be prohibitively expensive.
  Dataset(const Dataset&) = delete;

  // GetMeshes no longer returns a reference to its mesh vector.
  // Any function that is routed through the scripting system *cannot* return
  // references. It doesn't make sense in terms of Lua. Pointers in the form of
  // shared_ptr's are fine however and are represented as lightuserdata in Lua.
  // See Tuvok's LuaDatasetProxy.
  std::vector<std::shared_ptr<Mesh>> GetMeshes() const { return m_vpMeshList; }
  virtual std::shared_ptr<const Histogram1D> Get1DHistogram() const {
    return m_pHist1D;
  }
  virtual std::shared_ptr<const Histogram2D> Get2DHistogram() const {
    return m_pHist2D;
  }
  virtual float MaxGradientMagnitude() const = 0;

  /// Remove all cached bricks / brick metadata.
  virtual void Clear() = 0;

  /// Gets the number of voxels, per dimension.
  virtual Vec3ui GetBrickVoxelCounts(const BrickKey&) const = 0;
  /// World space extents.
  virtual Vec3f GetBrickExtents(const BrickKey &) const = 0;
  /// Data access
  ///@{
  virtual bool GetBrick(const BrickKey&, std::vector<uint8_t>&) const=0;
  virtual bool GetBrick(const BrickKey&, std::vector<int8_t>&) const = 0;
  virtual bool GetBrick(const BrickKey&, std::vector<uint16_t>&) const=0;
  virtual bool GetBrick(const BrickKey&, std::vector<int16_t>&) const=0;
  virtual bool GetBrick(const BrickKey&, std::vector<uint32_t>&) const=0;
  virtual bool GetBrick(const BrickKey&, std::vector<int32_t>&) const=0;
  virtual bool GetBrick(const BrickKey&, std::vector<float>&) const=0;
  virtual bool GetBrick(const BrickKey&, std::vector<double>&) const=0;
  ///@}
  virtual BrickTable::const_iterator BricksBegin() const = 0;
  virtual BrickTable::const_iterator BricksEnd() const = 0;
  /// @return the number of bricks in a given LoD + timestep.
  virtual BrickTable::size_type GetBrickCount(size_t lod, size_t ts) const = 0;
  /// @return the LOD idx for a large 1-brick LOD.
  virtual size_t GetLargestSingleBrickLOD(size_t ts) const=0;

  virtual bool BrickIsFirstInDimension(size_t, const BrickKey&) const = 0;
  virtual bool BrickIsLastInDimension(size_t, const BrickKey&) const = 0;

  /// User rescaling factors.
  ///@{
  void SetRescaleFactors(const Vec3d&);
  Vec3d GetRescaleFactors() const;
  /// If the underlying file format supports it, save the current scaling
  /// factors to the file.  The format should implicitly load and apply the
  /// scaling factors when opening the file.
  virtual bool SaveRescaleFactors() {return false;}
  ///@}

  /// If the underlying file format supports it, crop the dataset at the
  /// given plane, i.e. set all values one side to zero
  virtual bool Crop( const PLANE<float>& , const std::string&, bool, bool, bool ) {return false;}

  virtual unsigned GetLODLevelCount() const = 0;
  /// @todo FIXME, should be pure virtual && overridden in derived
  virtual uint64_t GetNumberOfTimesteps() const { return 1; }
  virtual Vec3ui64 GetDomainSize(const size_t lod=0,
                                      const size_t ts=0) const = 0;
  virtual Vec3d GetScale() const {return m_DomainScale * m_UserScale;}
  virtual Vec3ui GetBrickOverlapSize() const = 0;
  /// @return the number of voxels for the given brick, per dimension, taking
  ///         into account any brick overlaps.
  virtual Vec3ui64 GetEffectiveBrickSize(const BrickKey &) const = 0;

  virtual unsigned GetBitWidth() const = 0;
  virtual uint64_t GetComponentCount() const = 0;
  virtual bool GetIsSigned() const = 0;
  virtual bool GetIsFloat() const = 0;
  virtual bool IsSameEndianness() const = 0;
  virtual std::pair<double,double> GetRange() const = 0;
  virtual const std::vector<std::pair<std::string, std::string>> GetMetadata() const {
    std::vector<std::pair<std::string, std::string>> v;
    return v;
  }

  /// Acceleration queries.
  virtual bool ContainsData(const BrickKey&, double /*isoval*/) const {return true;}
  virtual bool ContainsData(const BrickKey&, double /*fMin*/, double /*fMax*/) const {return true;}
  virtual bool ContainsData(const BrickKey&, double /*fMin*/, double /*fMax*/, double /*fMinGradient*/, double /*fMaxGradient*/) const {return true;}

  /// unimplemented!  Override these if you want tools built on this IO layer
  /// to be able to create data in your format.
  virtual bool Export(uint64_t iLODLevel, const std::string& targetFilename,
    bool bAppend) const = 0;

  virtual bool ApplyFunction(uint64_t iLODLevel,
                        bool (*brickFunc)(void* pData,
                                          const Vec3ui64& vBrickSize,
                                          const Vec3ui64& vBrickOffset,
                                          void* pUserContext),
                        void *pUserContext,
                        uint64_t iOverlap) const = 0;

  /// Virtual constructor.
  virtual Dataset* Create(const std::string&, uint64_t, bool) const=0;

  /// this function computes the texture coordinates for a given brick
  /// this may be non trivial with power of two padding, overlap handling
  /// and per brick rescale
  virtual  std::pair<Vec3f, Vec3f> GetTextCoords(
    BrickTable::const_iterator brick,
    bool bUseOnlyPowerOfTwo
  ) const;

protected:
  std::shared_ptr<Histogram1D>          m_pHist1D;
  std::shared_ptr<Histogram2D>          m_pHist2D;
  std::vector<std::shared_ptr<Mesh>>   m_vpMeshList;

  Vec3d      m_UserScale;
  Vec3d      m_DomainScale;

  void DeleteMeshes();
};

}

#endif // TUVOK_DATASET_H
