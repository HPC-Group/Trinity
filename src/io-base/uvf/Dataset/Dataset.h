#pragma once

#include <cstdlib>
#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>

#include "commands/Grids.h"
#include "silverbullet/math/Vectors.h"
#include "silverbullet/dataio/base/Brick.h"

#define MAX_TRANSFERFUNCTION_SIZE 4096

typedef Grid1D<uint32_t> Histogram1D;
typedef Grid2D<uint32_t> Histogram2D;
class LargeRAWFile;

class Metadata;
class Mesh;

/// Abstract interface to a dataset.

class Dataset {

public:
  Dataset();
  virtual ~Dataset();

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
  virtual Core::Math::Vec3ui GetBrickVoxelCounts(const BrickKey&) const = 0;
  /// World space extents.
  virtual Core::Math::Vec3f GetBrickExtents(const BrickKey &) const = 0;
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
  void SetRescaleFactors(const Core::Math::Vec3d&);
  Core::Math::Vec3d GetRescaleFactors() const;
  /// If the underlying file format supports it, save the current scaling
  /// factors to the file.  The format should implicitly load and apply the
  /// scaling factors when opening the file.
  virtual bool SaveRescaleFactors() {return false;}
  ///@}


  /// If the underlying file format supports it, crop the dataset at the
  /// given plane, i.e. set all values one side to zero
  virtual bool Crop(const Core::Math::PLANE<float>&, const std::string&, bool, bool, bool) {
    return false;
  }

  virtual unsigned GetLODLevelCount() const = 0;
  /// @todo FIXME, should be pure virtual && overridden in derived
  virtual uint64_t GetNumberOfTimesteps() const { return 1; }
  virtual Core::Math::Vec3ui64 GetDomainSize(const size_t lod=0,
                                      const size_t ts=0) const = 0;
  virtual Core::Math::Vec3d GetScale() const {return m_DomainScale * m_UserScale;}
  virtual Core::Math::Vec3ui GetBrickOverlapSize() const = 0;
  /// @return the number of voxels for the given brick, per dimension, taking
  ///         into account any brick overlaps.
  virtual Core::Math::Vec3ui64 GetEffectiveBrickSize(const BrickKey &) const = 0;

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

  typedef bool (bfqn)(void*, const Core::Math::Vec3ui64&, const Core::Math::Vec3ui64&, void*);
  virtual bool ApplyFunction(uint64_t iLODLevel, bfqn* bfunc,
                             void* userContext, uint64_t noverlap) const = 0;

  /// Virtual constructor.
  virtual Dataset* Create(const std::string&, uint64_t, bool) const=0;  

  /// this function computes the texture coordinates for a given brick
  /// this may be non trivial with power of two padding, overlap handling
  /// and per brick rescale
  virtual  std::pair<Core::Math::Vec3f, Core::Math::Vec3f> GetTextCoords(
    BrickTable::const_iterator brick,
    bool bUseOnlyPowerOfTwo
  ) const;

protected:
  std::shared_ptr<Histogram1D>       m_pHist1D;
  std::shared_ptr<Histogram2D>       m_pHist2D;
  std::vector<std::shared_ptr<Mesh>> m_vpMeshList;

  Core::Math::Vec3d m_UserScale;
  Core::Math::Vec3d m_DomainScale;

  void DeleteMeshes();
};
