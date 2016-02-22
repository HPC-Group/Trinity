#include "FractalIO.h"
#include "mocca/base/Error.h"

/*
using namespace Core::Math;

namespace Trinity {
namespace IO {

FractalIO::FractalIO(uint64_t sx, uint64_t sy, uint64_t sz,
                     uint64_t bx, uint64_t by, uint64_t bz) :
m_totalSize(sx, sy, sz),
m_brickSize(bx, by, bz),
m_bFlat(sx<=bx && sz<=bz && sz<=bz),
m_mbGenerator(sx,sy,sz)
{
}


Vec3ui64 FractalIO::getMaxBrickSize() const {
    return m_brickSize;
}

Vec3ui64 FractalIO::getMaxUsedBrickSizes() const {
    if (!m_bFlat) {
        return m_brickSize;
    } else {
        return m_totalSize;
    }
}

MinMaxBlock FractalIO::maxMinForKey(const BrickKey&) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        // added 0 and 1 as the last two arguments
        return MinMaxBlock(0,255, 0, 1);
    }
}

unsigned FractalIO::getLODLevelCount() const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        return 1;
    }
}

uint64_t FractalIO::getNumberOfTimesteps() const {
    return 1;
}

Vec3ui64 FractalIO::getDomainSize(const uint64_t lod, const uint64_t ts) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        return Vec3ui64(m_totalSize);
    }
}

Mat4d FractalIO::getTransformation() const {
    return Mat4d(); // identity matix
}

Vec3ui FractalIO::getBrickOverlapSize() const {
    if (!m_bFlat) {
        return Vec3ui(2,2,2);
    } else {
        return Vec3ui(0,0,0);
    }
}

uint64_t FractalIO::getLargestSingleBrickLOD(uint64_t ts) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        return 1;
    }
}

Vec3ui FractalIO::getBrickVoxelCounts(const BrickKey& key) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        // Jens returend 1 here
        Vec3ui vec(1,1,1);
        return vec;
    }
}

Vec3f FractalIO::getBrickExtents(const BrickKey & key) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        // Jens returend 1 here
        Vec3f vec(1,1,1);
        return vec;
    }
}

Vec3ui FractalIO::getBrickLayout(uint64_t LoD, uint64_t timestep) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        return Vec3ui(1,1,1);
    }
}

ValueType FractalIO::getType() const {
    return T_UINT8;
}

uint64_t FractalIO::getModalityCount() const {
    return 1;
}

uint64_t FractalIO::getComponentCount(uint64_t modality) const {
    return 1;
}

Semantic FractalIO::getSemantict(uint64_t modality) const {
    return Scalar;
}

Vec2f FractalIO::getRange() const {
    return Vec2f(0,255);
}

std::string FractalIO::getUserDefinedSemantic(uint64_t modality) const {
  return "Fractal dataset";
}

uint64_t FractalIO::getTotalBrickCount() const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        return 1;
    }
}

bool FractalIO::getBrick(const BrickKey& key, std::vector<uint8_t>& data) const {
    bool created = false;

    if (data.size() < getMaxBrickSize().volume()) {
        data.reserve(getMaxBrickSize().volume());
        created = true;
    }

    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        size_t index = 0;
        for (int z = 0;z<m_totalSize.z;++z) {
            for (int y = 0;y<m_totalSize.y;++y) {
                for (int x = 0;x<m_totalSize.x;++x) {
                    data[index++] = m_mbGenerator.computePoint(x,y,z);
                }
            }
        }

    }
    return created;
}
}
}
*/