#pragma once

#include "io-base/fractal/Mandelbulb.h"
#include "io-base/IListData.h"
#include "common/IIO.h"

#include "mocca/log/LogManager.h"

namespace trinity {
  
  class FractalIO : public IIO {
  public:
    //FractalIO();
    FractalIO(int fileId, IListData& listData);
    
    
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
    bool getBrick(const BrickKey& brickKey,
                  std::vector<uint8_t>& data) const; // HACK: override;

    // HACK
    std::vector<uint8_t> getBrick(const BrickKey& brickKey,
        bool& success) const override {
        return std::vector<uint8_t>();
    }
    
    
  private:
    Core::Math::Vec3ui64 m_totalSize;
    Core::Math::Vec3ui64 m_brickSize;
    bool m_bFlat;
    Mandelbulb<uint8_t> m_mbGenerator;
    IListData& m_listData;


    // HACK: that's what I would like todo but the API won't let me so I had to
    // move the constructor into the pseudo constructor below
    /*
     FractalIO(uint64_t sx, uint64_t sy, uint64_t sz,
     uint64_t bx, uint64_t by, uint64_t bz);
     */
    
    void pseudoConstructor(uint64_t sx, uint64_t sy, uint64_t sz,
                           uint64_t bx, uint64_t by, uint64_t bz);
    
    
  };
}