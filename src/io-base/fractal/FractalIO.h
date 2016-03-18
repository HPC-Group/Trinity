#pragma once

#include "io-base/fractal/Mandelbulb.h"
#include "io-base/IListData.h"
#include "common/IIO.h"

#include "mocca/log/LogManager.h"

namespace trinity {
  
  class FractalIO : public IIO {
  public:
    FractalIO(const std::string& fileId, std::shared_ptr<IListData> listData);
    
    Core::Math::Vec3ui64 getMaxBrickSize() const override;
    Core::Math::Vec3ui64 getMaxUsedBrickSizes() const override;
    MinMaxBlock maxMinForKey(const BrickKey&) const override;
    int getLODLevelCount() const override;
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
    uint64_t getTotalBrickCount() const override;
    bool getBrick(const BrickKey& brickKey, std::vector<uint8_t>& data) const override;
    IIO::ValueType getType() const override;
    IIO::Semantic getSemantic(uint64_t modality) const override;
        
  private:
    Core::Math::Vec3ui64 m_totalSize;
    Core::Math::Vec3ui64 m_brickSize;
    bool m_bFlat;
    std::shared_ptr<Mandelbulb<uint8_t>> m_mbGenerator;

  };
}