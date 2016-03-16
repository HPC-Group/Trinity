#include "FractalIO.h"
#include "mocca/base/Error.h"

/*
using namespace Core::Math;

namespace Trinity {
namespace IO {

FractalFractalIO(uint64_t sx, uint64_t sy, uint64_t sz,
                     uint64_t bx, uint64_t by, uint64_t bz) :
m_totalSize(sx, sy, sz),
m_brickSize(bx, by, bz),
m_bFlat(sx<=bx && sz<=bz && sz<=bz),
m_mbGenerator(sx,sy,sz)
{
}


Vec3ui64 FractalgetMaxBrickSize() const {
    return m_brickSize;
}

Vec3ui64 FractalgetMaxUsedBrickSizes() const {
    if (!m_bFlat) {
        return m_brickSize;
    } else {
        return m_totalSize;
    }
}

MinMaxBlock FractalmaxMinForKey(const BrickKey&) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        // added 0 and 1 as the last two arguments
        return MinMaxBlock(0,255, 0, 1);
    }
}

unsigned FractalgetLODLevelCount() const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        return 1;
    }
}

uint64_t FractalgetNumberOfTimesteps() const {
    return 1;
}

Vec3ui64 FractalgetDomainSize(const uint64_t lod, const uint64_t ts) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        return Vec3ui64(m_totalSize);
    }
}

Mat4d FractalgetTransformation() const {
    return Mat4d(); // identity matix
}

Vec3ui FractalgetBrickOverlapSize() const {
    if (!m_bFlat) {
        return Vec3ui(2,2,2);
    } else {
        return Vec3ui(0,0,0);
    }
}

uint64_t FractalgetLargestSingleBrickLOD(uint64_t ts) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        return 1;
    }
}

Vec3ui FractalgetBrickVoxelCounts(const BrickKey& key) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        // Jens returend 1 here
        Vec3ui vec(1,1,1);
        return vec;
    }
}

Vec3f FractalgetBrickExtents(const BrickKey & key) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        // Jens returend 1 here
        Vec3f vec(1,1,1);
        return vec;
    }
}

Vec3ui FractalgetBrickLayout(uint64_t lod, uint64_t timestep) const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        return Vec3ui(1,1,1);
    }
}

ValueType FractalgetType() const {
    return T_UINT8;
}

uint64_t FractalgetModalityCount() const {
    return 1;
}

uint64_t FractalgetComponentCount(uint64_t modality) const {
    return 1;
}

Semantic FractalgetSemantict(uint64_t modality) const {
    return Scalar;
}

Vec2f FractalgetRange() const {
    return Vec2f(0,255);
}

std::string FractalgetUserDefinedSemantic(uint64_t modality) const {
  return "Fractal dataset";
}

uint64_t FractalgetTotalBrickCount() const {
    if (!m_bFlat) {
        // TODO
        throw new mocca::Error("case not implemented", __FILE__, __LINE__);
    } else {
        return 1;
    }
}

bool FractalgetBrick(const BrickKey& key, std::vector<uint8_t>& data) const {
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