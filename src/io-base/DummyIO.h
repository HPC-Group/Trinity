#pragma once
#include <iostream>
#include <memory>

#include "common/IIO.h"


namespace trinity {
class DummyIO : public IIO {
public:
    // testing purpose
    
    Core::Math::Vec3ui64 getMaxBrickSize() const override {Core::Math::Vec3ui64 x; return x; }
    Core::Math::Vec3ui64 getMaxUsedBrickSizes() const override {Core::Math::Vec3ui64 x; return x; }
    MinMaxBlock maxMinForKey(const BrickKey&) const override {MinMaxBlock x; return x; }
    int getLODLevelCount() const override { return 42; }
    uint64_t getNumberOfTimesteps() const override {uint64_t x; return x; }
    Core::Math::Vec3ui64 getDomainSize(uint64_t lod, uint64_t modality, uint64_t ts) const override {Core::Math::Vec3ui64 x; return x; }


    //Core::Math::Mat4d getTransformation() const override {Core::Math::Mat4d x; return x; }
    //Core::Math::Vec3ui getBrickOverlapSize() const override {Core::Math::Vec3ui x; return x; }
    //uint64_t getLargestSingleBrickLOD(uint64_t ts) const override {uint64_t x; return x; }
    //Core::Math::Vec3ui getBrickVoxelCounts(const BrickKey&) const override {Core::Math::Vec3ui x; return x; }
    //Vec3f getBrickExtents(const BrickKey &) const override {Vec3f x; return x; }
    //Core::Math::Vec3ui getBrickLayout(uint64_t LoD, uint64_t timestep) const override {Core::Math::Vec3ui x; return x; }
    //uint64_t getModalityCount() const override {uint64_t x; return x; }
    //uint64_t getComponentCount(uint64_t modality) const override {uint64_t x; return x; }
    //Core::Math::Vec2f getRange() const override {Core::Math::Vec2f x; return x; }
    //uint64_t getTotalBrickCount() const override {uint64_t x; return x; }
    //bool getBrick(const BrickKey&, std::vector<uint8_t>&) const override {bool x; return x; }

    
};
}