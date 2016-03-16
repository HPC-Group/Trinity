#pragma once

#include "Mandelbulb.h"
#include "common/IIO.h"
#include "mocca/log/LogManager.h"

/**
 OLD FILE - modified to fit the IIO
 */

namespace trinity {

class FractalIO : public IIO {
public:
    FractalIO() { LINFO("(io) fractal io created"); }
    // testing purpose


    /*
    FractalIO(uint64_t sx, uint64_t sy, uint64_t sz,
              uint64_t bx, uint64_t by, uint64_t bz);
     */

    Core::Math::Vec3ui64 getMaxBrickSize() const override {Core::Math::Vec3ui64 x; return x; }
    Core::Math::Vec3ui64 getMaxUsedBrickSizes() const override {Core::Math::Vec3ui64 x; return x; }
    MinMaxBlock maxMinForKey(const BrickKey&) const override {MinMaxBlock x; return x; }
    int getLODLevelCount() const override { return 44; }
    uint64_t getNumberOfTimesteps() const override {uint64_t x; return x; }
    Core::Math::Vec3ui64 getDomainSize(uint64_t lod, uint64_t modality, uint64_t ts) const override {Core::Math::Vec3ui64 x; return x; }
    Core::Math::Mat4d getTransformation() const override {Core::Math::Mat4d x; return x; }
    Core::Math::Vec3ui getBrickOverlapSize() const override {Core::Math::Vec3ui x; return x; }



    //uint64_t getLargestSingleBrickLOD(uint64_t ts) const override {uint64_t x; return x; }
    //Core::Math::Vec3ui getBrickVoxelCounts(const BrickKey&) const override {Core::Math::Vec3ui x; return x; }
    //Vec3f getBrickExtents(const BrickKey &) const override {Vec3f x; return x; }
    //Core::Math::Vec3ui getBrickLayout(uint64_t LoD, uint64_t timestep) const override {Core::Math::Vec3ui x; return x; }
    //uint64_t getModalityCount() const override {uint64_t x; return x; }
    //uint64_t getComponentCount(uint64_t modality) const override {uint64_t x; return x; }
    //Core::Math::Vec2f getRange() const override {Core::Math::Vec2f x; return x; }
    //uint64_t getTotalBrickCount() const override {uint64_t x; return x; }
    //bool getBrick(const BrickKey&, std::vector<uint8_t>&) const override {bool x; return x; }

private:
    Core::Math::Vec3ui64 m_totalSize;
    Core::Math::Vec3ui64 m_brickSize;
    bool m_bFlat;
    // Mandelbulb<uint8_t> m_mbGenerator;
    
};
}