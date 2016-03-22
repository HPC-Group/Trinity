#pragma once

#include <vector>

#include "io-base/fractal/Mandelbulb.h"
#include "io-base/IListData.h"
#include "common/IIO.h"

#include "commands/TransferFunction1D.h"

#include "mocca/log/LogManager.h"

namespace trinity {
  
  class FractalIO : public IIO {
  public:
    FractalIO(const std::string& fileId, const IListData& listData);
    
    Core::Math::Vec3ui64 getMaxBrickSize() const override;
    Core::Math::Vec3ui64 getMaxUsedBrickSizes() const override;
    MinMaxBlock maxMinForKey(const BrickKey&) const override;
    uint64_t getLODLevelCount(uint64_t modality) const override;
    uint64_t getNumberOfTimesteps() const override;
    Core::Math::Vec3ui64 getDomainSize(uint64_t lod, uint64_t modality) const override;
    Core::Math::Mat4d getTransformation(uint64_t modality) const override;
    Core::Math::Vec3ui getBrickOverlapSize() const override;
    uint64_t getLargestSingleBrickLOD(uint64_t modality) const override;
    Core::Math::Vec3ui getBrickVoxelCounts(const BrickKey&) const override;
    Core::Math::Vec3f getBrickExtents(const BrickKey &) const override;
    Core::Math::Vec3ui getBrickLayout(uint64_t lod, uint64_t modality) const override;
    uint64_t getModalityCount() const override;
    uint64_t getComponentCount(uint64_t modality) const override;
    Core::Math::Vec2f getRange(uint64_t modality) const override;
    uint64_t getTotalBrickCount(uint64_t modality) const override;
    bool getBrick(const BrickKey& brickKey, std::vector<uint8_t>& data) const override;
    IIO::ValueType getType(uint64_t modality) const override;
    IIO::Semantic getSemantic(uint64_t modality) const override;
    uint64_t getDefault1DTransferFunctionCount() const override;

    // HACK: todo function calls

    uint64_t getDefault2DTransferFunctionCount() const; // override;

    std::vector<uint64_t> get1DHistogram() const; // override;
    std::vector<uint64_t> get2DHistogram() const; // override;

    TransferFunction1D getDefault1DTransferFunction(uint64_t index) const; // override;
//    TransferFunction2D getDefault2DTransferFunction(uint64_t index) const; // override;

    std::string getUserDefinedSemantic(uint64_t modality) const; // override;

    // todo end

    
  private:
    Core::Math::Vec3ui64 m_totalSize;
    Core::Math::Vec3ui64 m_brickSize;
    bool m_bFlat;
    std::unique_ptr<Mandelbulb<uint8_t>> m_fractalGenerator;

    void genBrickParams(const BrickKey& brickKey,
                        Core::Math::Vec3ui64& start,
                        Core::Math::Vec3d& stepping,
                        Core::Math::Vec3ui64& size) const;
    
    Core::Math::Vec3ui64 get3DIndex(const BrickKey& brickKey) const;
    Core::Math::Vec3ui64 getEffectiveBricksize() const;
    
    struct boolVec {
      boolVec(bool x, bool y, bool z) : m_x(x),m_y(y),m_z(z) {}
      bool m_x,m_y,m_z;
    };
    
    boolVec isLastBrick(const BrickKey& key) const;

    
    void computeLODInfo();
    
    struct LODInfo {
      /// the aspect ratio of all bricks in this LoD, does not
      /// take the overall aspect into account just the aspect
      /// ratio changes that happen during the downsampling
      Core::Math::Vec3f m_vAspect;
      /// size of the entire LoD in voxel, i.e. for LoD 0 this
      /// is equal to the size of the original dataset
      Core::Math::Vec3ui64 m_iLODVoxelSize;
      /// number of bricks in x, y, and z in this LoD
      Core::Math::Vec3ui64 m_iLODBrickCount;
    };
    
    std::vector<LODInfo> m_vLODTable;

  };
}