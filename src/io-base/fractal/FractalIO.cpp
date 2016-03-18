#include "io-base/fractal/FractalIO.h"
#include "io-base/FractalListData.h"


#include "common/TrinityError.h"

#include "mocca/base/Error.h"

using namespace Core::Math;
using namespace trinity;

FractalIO::FractalIO(const std::string& fileId, std::shared_ptr<IListData> listData) :
m_mbGenerator(nullptr) {

  const auto fractalListData = std::dynamic_pointer_cast<FractalListData>(listData);

  if (fractalListData) {
    m_totalSize = fractalListData->totalSize(fileId);
    m_brickSize = fractalListData->brickSize(fileId);
    m_bFlat = (m_totalSize.x <= m_brickSize.x &&
               m_totalSize.z <= m_brickSize.z &&
               m_totalSize.z <= m_brickSize.z);
    m_mbGenerator = std::make_shared<Mandelbulb<uint8_t>>(m_totalSize.x,
                                                          m_totalSize.y,
                                                          m_totalSize.z);
  } else {
    throw new TrinityError("invalid listData type", __FILE__, __LINE__);
  }
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
    throw new TrinityError("case not implemented", __FILE__, __LINE__);
  } else {
    // added 0 and 1 as the last two arguments
    return MinMaxBlock(0, 255, 0, 1);
  }
}

int FractalIO::getLODLevelCount() const {
  if (!m_bFlat) {
    // TODO
    return 1;
    // throw new TrinityError("case not implemented", __FILE__, __LINE__);
  } else {
    return 1;
  }
}

uint64_t FractalIO::getNumberOfTimesteps() const {
  return 1;
}

Vec3ui64 FractalIO::getDomainSize(uint64_t lod, uint64_t modality) const {
  if (!m_bFlat) {
    // TODO
    throw new TrinityError("case not implemented", __FILE__, __LINE__);
  } else {
    return Vec3ui64(m_totalSize);
  }
}

Mat4d FractalIO::getTransformation(uint64_t) const {
  return Mat4d(); // identity matix
}

Vec3ui FractalIO::getBrickOverlapSize() const {
  if (!m_bFlat) {
    return Vec3ui(2, 2, 2);
  } else {
    return Vec3ui(0, 0, 0);
  }
}

uint64_t FractalIO::getLargestSingleBrickLOD(uint64_t modality) const {
  if (!m_bFlat) {
    // TODO
    throw new TrinityError("case not implemented", __FILE__, __LINE__);
  } else {
    return 1;
  }
}

Vec3ui FractalIO::getBrickVoxelCounts(const BrickKey& key) const {
  if (!m_bFlat) {
    // TODO
    throw new TrinityError("case not implemented", __FILE__, __LINE__);
  } else {
    // Jens returend 1 here
    Vec3ui vec(1, 1, 1);
    return vec;
  }
}

Vec3f FractalIO::getBrickExtents(const BrickKey& key) const {
  if (!m_bFlat) {
    // TODO
    throw new TrinityError("case not implemented", __FILE__, __LINE__);
  } else {
    // Jens returend 1 here
    Vec3f vec(1, 1, 1);
    return vec;
  }
}

Vec3ui FractalIO::getBrickLayout(uint64_t lod, uint64_t modality) const {
  if (!m_bFlat) {
    // TODO
    throw new TrinityError("case not implemented", __FILE__, __LINE__);
  } else {
    return Vec3ui(1, 1, 1);
  }
}

uint64_t FractalIO::getModalityCount() const {
  return 1;
}

uint64_t FractalIO::getComponentCount(uint64_t modality) const {
  return 1;
}

/*

 HACK: these functions are missing in the header

 ValueType FractalIO::getType() const {
 return T_UINT8;
 }

 Semantic FractalIO::getSemantict(uint64_t modality) const {
 return Scalar;
 }

 std::string FractalIO::getUserDefinedSemantic(uint64_t modality) const {
 return "Fractal dataset";
 }

 */

Vec2f FractalIO::getRange(uint64_t modality) const {
  return Vec2f(0, 255);
}


uint64_t FractalIO::getTotalBrickCount() const {
  if (!m_bFlat) {
    // TODO
    throw new TrinityError("case not implemented", __FILE__, __LINE__);
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
    throw new TrinityError("case not implemented", __FILE__, __LINE__);
  } else {
#pragma omp parallel for
    for (int z = 0; z < m_totalSize.z; ++z) {
      for (int y = 0; y < m_totalSize.y; ++y) {
        for (int x = 0; x < m_totalSize.x; ++x) {
          const size_t i = size_t(x + y * m_totalSize.x + z * m_totalSize.x * m_totalSize.y);
          data[i] = m_mbGenerator->computePoint(x, y, z);
        }
      }
    }
  }
  return created;
}

IIO::ValueType FractalIO::getType() const {
  // TODO
  return IIO::ValueType();
}

IIO::Semantic trinity::FractalIO::getSemantic(uint64_t modality) const {
  // TODO
  return IIO::Semantic();
}
