#include "io-base/FractalListData.h"
#include "io-base/fractal/FractalIO.h"

#include "common/TrinityError.h"
#include "mocca/log/LogManager.h"
#include "mocca/base/Error.h"

using namespace Core::Math;
using namespace trinity;

FractalIO::FractalIO(const std::string& fileId, const IListData& listData)
: m_fractalGenerator(nullptr)
#ifdef CACHE_BRICKS
, m_bc(".","fractal")
#endif
{
  LINFO("(fractalio) initializing fractal for file id " + fileId);
  const auto fractalListData = dynamic_cast<const FractalListData*>(&listData);

  if (fractalListData) {
    LINFO("(fractalio) acquiring total size... ");
    m_totalSize = fractalListData->totalSize(fileId);
    LINFO("(fractalio) acquiring brick size... ");
    m_brickSize = fractalListData->brickSize(fileId);
    m_bFlat = (m_totalSize.x <= m_brickSize.x &&
               m_totalSize.z <= m_brickSize.z &&
               m_totalSize.z <= m_brickSize.z);
    LINFO("(fractalio) creating mandelbulb... ");
    m_fractalGenerator = mocca::make_unique<Mandelbulb<uint8_t>>(m_totalSize.x,
                                                                 m_totalSize.y,
                                                                 m_totalSize.z);
    LINFO("(fractalio) mandelbulb created");

    if (!m_bFlat) {
      computeLODInfo();
      LINFO("(fractalio) metadata computed");
    }

  } else {
    throw TrinityError("invalid listData type", __FILE__, __LINE__);
  }
}

Vec3ui64 FractalIO::getMaxBrickSize() const {
  return m_brickSize;
}

Vec3ui64 FractalIO::getMaxUsedBrickSizes() const {
  if (!m_bFlat) {
    return Vec3ui64(std::min(m_brickSize.x, m_totalSize.x),
                    std::min(m_brickSize.y, m_totalSize.y),
                    std::min(m_brickSize.z, m_totalSize.z));
  } else {
    return m_totalSize;
  }
}

MinMaxBlock FractalIO::maxMinForKey(const BrickKey& key) const {
  // TODO: compute gradients and fill 3rd and 4th parameters accordingly
  
  uint8_t min = 255, max = 0;
#ifdef CACHE_BRICKS
  if (m_bc.getMaxMin(key,min, max)) {
    return MinMaxBlock(min, max, 0, 1);
  }
#endif

  if (!m_bFlat) {
    std::vector<uint8_t> data;
    getBrick(key, data);
    
    for (uint8_t v : data) {
      if (v < min) min = v;
      if (v > max) max = v;
      if (min == 0 && max == 255) break;
    }
    
    return MinMaxBlock(min, max, 0, 1);
  } else {
    return MinMaxBlock(0, 255, 0, 1);
  }
}

uint64_t FractalIO::getLODLevelCount(uint64_t modality) const {
  if (modality != 0 )
    throw TrinityError("invalid modality", __FILE__, __LINE__);

  if (!m_bFlat) {
    return m_vLODTable.size();
  } else {
    return 1;
  }
}

uint64_t FractalIO::getNumberOfTimesteps() const {
  return 1;
}

Vec3ui64 FractalIO::getDomainSize(uint64_t lod, uint64_t modality) const {
  if (modality != 0 )
    throw TrinityError("invalid modality", __FILE__, __LINE__);

  if (!m_bFlat) {
    return m_vLODTable[lod].m_iLODVoxelSize;
  } else {
    return m_totalSize;
  }
}

Mat4d FractalIO::getTransformation(uint64_t) const {
  return Mat4d(); // always return identity matix for fractals
}

Vec3ui FractalIO::getBrickOverlapSize() const {
  if (!m_bFlat) {
    return Vec3ui(2, 2, 2);
  } else {
    return Vec3ui(0, 0, 0);
  }
}

uint64_t FractalIO::getLargestSingleBrickLOD(uint64_t modality) const {
  if (modality != 0 )
    throw TrinityError("invalid modality", __FILE__, __LINE__);

  if (!m_bFlat) {
    for (size_t lod = 0;lod<m_vLODTable.size(); ++lod) {
      if (m_vLODTable[lod].m_iLODVoxelSize.volume() == 1)
        return lod;
    }

    // this return is never hit by design of the m_vLODTable
    return m_vLODTable.size()-1;
  } else {
    return 0;
  }
}


/*
 isLastBrick:

 Computes whether a brick is the last brick in a row, column, or slice.
 To do this we simply fetch the brick count of the brick's LoD and
 then check if it's index is equal to the max index.
 */
FractalIO::boolVec FractalIO::isLastBrick(const BrickKey& key) const {
  const Vec3ui64 vBrickCoords = get3DIndex(key);
  const Vec3ui vLODSize = getBrickLayout(key.lod, key.modality);

  return boolVec(vBrickCoords.x >= vLODSize.x-1,
                 vBrickCoords.y >= vLODSize.y-1,
                 vBrickCoords.z >= vLODSize.z-1);
}

/*
 ComputeBrickSize:

 computes the size of a given brick. The idea is as follows: Any inner brick
 has the maximum size, if it's the last brick it may
 be smaller, so first we determine of the brick is a boundary brick in x, y, and
 z. If yes then we compute it's size, which is the remainder of what we get when
 we divide the size of the LoD by the effective size of a brick, i.e. the size
 minus the two overlaps. To this required effective size we add the two overlaps
 back to get the total size including overlaps.
 One special case needs to observed, that is if the boundary brick has to have
 the maximum size in this case we also return the maximum size (just like with
 inner bricks).
 */
Vec3ui FractalIO::getBrickVoxelCounts(const BrickKey& key) const {
  if (!m_bFlat) {
    const boolVec    bIsLast = isLastBrick(key);
    const Vec3ui64 voxelSize = getDomainSize(key.lod, key.modality);
    const Vec3ui   i2Overlap = getBrickOverlapSize()*2;
    const Vec3ui64 effectiveBricksize = getEffectiveBricksize();

    return Vec3ui(bIsLast.m_x && (voxelSize.x % effectiveBricksize.x)
                  ? (i2Overlap.x + (voxelSize.x % effectiveBricksize.x))
                  : getMaxUsedBrickSizes().x,
                  bIsLast.m_y && (voxelSize.y % effectiveBricksize.y)
                  ? (i2Overlap.y + (voxelSize.y % effectiveBricksize.y))
                  : getMaxUsedBrickSizes().y,
                  bIsLast.m_z && (voxelSize.z % effectiveBricksize.z)
                  ? (i2Overlap.z + (voxelSize.z % effectiveBricksize.z))
                  : getMaxUsedBrickSizes().z);
  } else {
    return Vec3ui(m_totalSize);
  }
}

Vec3f FractalIO::getBrickExtents(const BrickKey& key) const {
  if (!m_bFlat) {
    return m_vLODTable[key.lod].m_vAspect;
  } else {
    return Vec3f(1, 1, 1);
  }
}

Vec3ui FractalIO::getBrickLayout(uint64_t lod, uint64_t modality) const {
  if (modality != 0 )
    throw TrinityError("invalid modality", __FILE__, __LINE__);

  if (!m_bFlat) {
    return Vec3ui(m_vLODTable[lod].m_iLODBrickCount);
  } else {
    return Vec3ui(1, 1, 1);
  }
}

uint64_t FractalIO::getModalityCount() const {
  return 1;
}

uint64_t FractalIO::getComponentCount(uint64_t modality) const {
  if (modality != 0 )
    throw TrinityError("invalid modality", __FILE__, __LINE__);

  return 1;
}

uint64_t FractalIO::getDefault1DTransferFunctionCount() const {
  return 5;
}

uint64_t FractalIO::getDefault2DTransferFunctionCount() const {
  return 1;
}


TransferFunction1D FractalIO::getDefault1DTransferFunction(uint64_t index) const {
  if (index >= getDefault1DTransferFunctionCount() )
    throw TrinityError("invalid 1D TF index", __FILE__, __LINE__);

  float center = float(index)/float(getDefault1DTransferFunctionCount());

  // create a transfer function with 4 different ramps
  // resulting in a smooth image with color shifts
  TransferFunction1D tf(256);
  tf.setStdFunction(center, 0.3, 0, false);
  tf.setStdFunction(center, 0.1, 1, true);
  tf.setStdFunction(center, 0.8, 2, true);
  tf.setStdFunction(center, 0.9, 3, false);
  return tf;
}

/*
 TransferFunction2D FractalIO::getDefault2DTransferFunction(uint64_t index) const {
 if (index != 0 )
 throw TrinityError("invalid 2D TF index", __FILE__, __LINE__);

 return
 }
 */

std::vector<uint64_t> FractalIO::get1DHistogram() const {
  // returning an empty vector is well defined an means that this
  // IO-node is unable to compute a 1D histogram
  return std::vector<uint64_t>();
}

std::vector<uint64_t> FractalIO::get2DHistogram() const {
  // returning an empty vector is well defined an means that this
  // IO-node is unable to compute a 2D histogram
  return std::vector<uint64_t>();
}

std::string FractalIO::getUserDefinedSemantic(uint64_t modality) const {
  if (modality != 0 )
    throw TrinityError("invalid modality", __FILE__, __LINE__);
  return "Fractal dataset";
}

Vec2f FractalIO::getRange(uint64_t modality) const {
  if (modality != 0 )
    throw TrinityError("invalid modality", __FILE__, __LINE__);

  return Vec2f(0, 255);
}


uint64_t FractalIO::getTotalBrickCount(uint64_t modality) const {
  if (!m_bFlat) {
    uint64_t lodLevel = getLODLevelCount(modality);

    uint64_t totalBrickCount = 0;
    for (uint64_t lod = 0;lod<lodLevel;++lod)
      totalBrickCount += getBrickLayout(lod, modality).volume();
    return totalBrickCount;
  } else {
    return 1;
  }
}

bool FractalIO::getBrick(const BrickKey& key, std::vector<uint8_t>& data) const{
  bool created = false;

  if (data.capacity() < getMaxBrickSize().volume()) {
    data.reserve(getMaxBrickSize().volume());
    created = true;
  }

#ifdef CACHE_BRICKS
  if (m_bc.getBrick<uint8_t>(key, data)) {
    return created;
  }
#endif
  

  if (!m_bFlat) {
    Vec3ui64 start, end, size, pos;
    Vec3d step;
    genBrickParams(key, start, step, size);
    pos = start;
    Vec3d dStart = Vec3d(start);
    data.resize(size.volume());
#pragma omp parallel for
    for (int z = 0; z < size.z; ++z) {
      for (int y = 0; y < size.y; ++y) {
        for (int x = 0; x < size.x; ++x) {
          const size_t i = size_t(x + y * size.x + z * size.x * size.y);
          const Vec3ui64 pos = Vec3ui64(dStart + step * Vec3d(x,y,z));
          data[i] = m_fractalGenerator->computePoint(pos.x, pos.y, pos.z);
        }
      }
    }
  } else {
    data.resize(m_totalSize.volume());
#pragma omp parallel for
    for (int z = 0; z < m_totalSize.z; ++z) {
      for (int y = 0; y < m_totalSize.y; ++y) {
        for (int x = 0; x < m_totalSize.x; ++x) {
          const size_t i = size_t(x + y * m_totalSize.x + z *
                                  m_totalSize.x * m_totalSize.y);
          data[i] = m_fractalGenerator->computePoint(x, y, z);
        }
      }
    }
  }

#ifdef CACHE_BRICKS
  m_bc.setBrick<uint8_t>(key, data);
#endif
  
  return created;
}

IIO::ValueType FractalIO::getType(uint64_t modality) const {
  return ValueType::T_UINT8;
}

IIO::Semantic FractalIO::getSemantic(uint64_t modality) const {
  if (modality != 0 )
    throw TrinityError("invalid modality", __FILE__, __LINE__);

  return Semantic::Scalar;
}

Vec3ui64 FractalIO::getEffectiveBricksize() const {
  return getMaxUsedBrickSizes()-Vec3ui64(getBrickOverlapSize()*2);
}

Vec3ui64 FractalIO::get3DIndex(const BrickKey& brickKey) const {
  const Vec3ui64 count = m_vLODTable[brickKey.lod].m_iLODBrickCount;
  return Vec3ui64(brickKey.index % count.x,
                  (brickKey.index / count.x) % count.y,
                  brickKey.index / (count.x*count.y));
}

void FractalIO::genBrickParams(const BrickKey& brickKey,
                               Vec3ui64& start,
                               Vec3d& stepping,
                               Vec3ui64& size) const {

  const Vec3ui64 effectiveBricksize = getEffectiveBricksize();

  size  = Vec3ui64(getBrickVoxelCounts(brickKey));
  start = get3DIndex(brickKey) * effectiveBricksize - Vec3ui64(getBrickOverlapSize());
  stepping =
  Vec3d(m_totalSize)/Vec3d(getDomainSize(brickKey.lod, brickKey.modality));
}

void FractalIO::computeLODInfo() {
  Vec3ui64 vVolumeSize = m_totalSize;
  Vec3f vAspect(1.0,1.0,1.0);

  const Vec3ui64 vUsableBrickSize = getEffectiveBricksize();
  do {
    LODInfo l;
    l.m_iLODVoxelSize = vVolumeSize;

    // downsample the volume (except for the first LoD)
    if (!m_vLODTable.empty())  {
      if (vVolumeSize.x > 1) {
        l.m_iLODVoxelSize.x = uint64_t(ceil(vVolumeSize.x/2.0));
        vAspect.x *= (vVolumeSize.x%2) ? float(vVolumeSize.x)/float(l.m_iLODVoxelSize.x) : 2;
      }

      if (vVolumeSize.y > 1) {
        l.m_iLODVoxelSize.y = uint64_t(ceil(vVolumeSize.y/2.0));
        vAspect.y *= (vVolumeSize.y%2) ? float(vVolumeSize.y)/float(l.m_iLODVoxelSize.y) : 2;
      }
      if (vVolumeSize.z > 1) {
        l.m_iLODVoxelSize.z = uint64_t(ceil(vVolumeSize.z/2.0));
        vAspect.z *= (vVolumeSize.z%2) ? float(vVolumeSize.z)/float(l.m_iLODVoxelSize.z) : 2;
      }
      vAspect /= vAspect.maxVal();

      vVolumeSize = l.m_iLODVoxelSize;
    }

    l.m_vAspect = vAspect;
    l.m_iLODBrickCount.x = uint64_t(ceil(vVolumeSize.x /
                                         double(vUsableBrickSize.x)));
    l.m_iLODBrickCount.y = uint64_t(ceil(vVolumeSize.y /
                                         double(vUsableBrickSize.y)));
    l.m_iLODBrickCount.z = uint64_t(ceil(vVolumeSize.z /
                                         double(vUsableBrickSize.z)));

    m_vLODTable.push_back(l);
  } while (vVolumeSize.x > 1 ||  vVolumeSize.y > 1 || vVolumeSize.z > 1);
}
