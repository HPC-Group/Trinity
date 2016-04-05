#pragma once

#include <vector>

#include "common/IIO.h"
#include "io-base/IListData.h"
#include "Dataset/uvfDataset.h"

#include "mocca/log/LogManager.h"

namespace trinity {
  
  class UVFIO : public IIO {
  public:
    UVFIO(const std::string& fileId, const IListData& listData);
    
    Core::Math::Vec3ui64 getMaxBrickSize() const override;
    Core::Math::Vec3ui64 getMaxUsedBrickSizes() const override;
    MinMaxBlock maxMinForKey(const BrickKey&) const override;
    uint64_t getLODLevelCount(uint64_t modality) const override;
    uint64_t getNumberOfTimesteps() const override;
    Core::Math::Vec3ui64 getDomainSize(uint64_t lod, uint64_t modality) const override;
    Core::Math::Vec3f getDomainScale(uint64_t modality) const override;
    Core::Math::Mat4d getTransformation(uint64_t modality) const override;
    Core::Math::Vec3ui getBrickOverlapSize() const override;
    uint64_t getLargestSingleBrickLOD(uint64_t modality) const override;
    Core::Math::Vec3ui getBrickVoxelCounts(const BrickKey&) const override;
    Core::Math::Vec3f getBrickExtents(const BrickKey&) const override;
    Core::Math::Vec3ui getBrickLayout(uint64_t lod, uint64_t modality) const override;
    Core::Math::Vec3f getFloatBrickLayout(uint64_t lod, uint64_t modality) const override;
    std::vector<MinMaxBlock> getBrickMaxMin() const override;
    uint64_t getModalityCount() const override;
    uint64_t getComponentCount(uint64_t modality) const override;
    Core::Math::Vec2f getRange(uint64_t modality) const override;
    uint64_t getTotalBrickCount(uint64_t modality) const override;
    std::shared_ptr<const std::vector<uint8_t>> getBrick(const BrickKey& brickKey, bool& success) const override;
    IIO::ValueType getType(uint64_t modality) const override;
    IIO::Semantic getSemantic(uint64_t modality) const override;
    uint64_t getDefault1DTransferFunctionCount() const override;
    uint64_t getDefault2DTransferFunctionCount() const override;
    std::vector<uint64_t> get1DHistogram() const override;
    std::vector<uint64_t> get2DHistogram() const override;
    TransferFunction1D getDefault1DTransferFunction(uint64_t index) const override;
    std::string getUserDefinedSemantic(uint64_t modality) const override;
    
    // HACK: todo function calls
    
    // TransferFunction2D getDefault2DTransferFunction(uint64_t index) const; // override;
    
    // todo end
  private:
    std::unique_ptr<UVFDataset> m_dataset;
    std::string                 m_filename;
    
    Core::Math::Vec3ui64 getEffectiveBricksize() const;
  };
}