#pragma once

#include "Mandelbulb.h"
#include "common/IIO.h"
#include "mocca/log/LogManager.h"

/**
 OLD FILE - modified to fit the IIO
 */

namespace trinity {
namespace io {

class FractalIO : public common::IIO {
public:
    FractalIO() { LINFO("(io) fractal io created"); }
    // testing purpose
    virtual int getLODLevelCount() { return 44; }


    /*
    FractalIO(uint64_t sx, uint64_t sy, uint64_t sz,
              uint64_t bx, uint64_t by, uint64_t bz);

    // changed from Vec3ui to Vec3ui64
    virtual Core::Math::Vec3ui64 getMaxBrickSize() const;
    virtual Core::Math::Vec3ui64 getMaxUsedBrickSizes() const;

    virtual MinMaxBlock maxMinForKey(const BrickKey&) const;
    virtual unsigned getLODLevelCount() const;
    virtual uint64_t getNumberOfTimesteps() const;
    virtual Core::Math::Vec3ui64 getDomainSize(const uint64_t lod = 0,
                                               const uint64_t ts = 0) const;

    virtual Core::Math::Mat4d getTransformation() const;

    virtual Core::Math::Vec3ui getBrickOverlapSize() const;

    virtual uint64_t getLargestSingleBrickLOD(uint64_t ts) const;
    virtual Core::Math::Vec3ui getBrickVoxelCounts(const BrickKey&) const;
    virtual Vec3f getBrickExtents(const BrickKey &) const;
    virtual Core::Math::Vec3ui getBrickLayout(uint64_t LoD, uint64_t timestep) const;

    virtual ValueType getType() const;
    virtual uint64_t getModalityCount() const;
    virtual std::string getUserDefinedSemantic(uint64_t modality) const;
    virtual uint64_t getComponentCount(uint64_t modality) const;
    virtual Semantic getSemantict(uint64_t modality) const;
    virtual Core::Math::Vec2f getRange() const;
    virtual uint64_t getTotalBrickCount() const;
    virtual bool getBrick(const BrickKey&, std::vector<uint8_t>&) const;

private:
    Core::Math::Vec3ui64 m_totalSize;
    Core::Math::Vec3ui64 m_brickSize;
    bool m_bFlat;
    Mandelbulb<uint8_t> m_mbGenerator;
     */
};
}
}