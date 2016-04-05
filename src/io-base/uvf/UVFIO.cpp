#include <algorithm>

#include "UVFIO.h"

#include "common/MemBlockPool.h"
#include "io-base/UVFListData.h"
#include "silverbullet/io/FileTools.h"
#include "silverbullet/base/StringTools.h"

#include "common/TrinityError.h"
#include "mocca/log/LogManager.h"
#include "mocca/base/Error.h"


using namespace Core::IO::FileTools;
using namespace Core::StringTools;
using namespace Core::Math;
using namespace trinity;

#define MaxAcceptableBricksize 1024

UVFIO::UVFIO(const std::string& fileId, const IListData& listData) :
  m_dataset(nullptr),
  m_filename("")
{
  LINFO("(UVFIO) initializing for file id " + fileId);
  const auto uvfListData = dynamic_cast<const UVFListData*>(&listData);

  if (uvfListData) {
    m_filename = fileId;
    if (!uvfListData->stripListerID(m_filename)) {
      throw TrinityError(std::string("invalid fileId") + fileId,
                         __FILE__, __LINE__);
    }

    
    LINFO("(UVFIO) Trying to load " + m_filename);
    
    
    if (!fileExists(m_filename)) {
      throw TrinityError(m_filename + " not found", __FILE__, __LINE__);
    }

    if (isDirectory(m_filename) || ToLowerCase(getExt(m_filename)) != "uvf" ) {
      throw TrinityError(m_filename + " is not a uvf file", __FILE__, __LINE__);
    }
    
    LINFO("(UVFIO) uvf filename passes basic checks, tring to open it");

    m_dataset = mocca::make_unique<UVFDataset>(m_filename,
                                               MaxAcceptableBricksize,
                                               false, false);
    
    LINFO("(UVFIO) successfully opened uvf file");
    
    if (!m_dataset->IsSameEndianness()) {
      throw TrinityError("dataset ist stored in incompatible endianness",
                         __FILE__, __LINE__);
    }

  } else {
    throw TrinityError("invalid listData type", __FILE__, __LINE__);
  }
}

Vec3ui64 UVFIO::getMaxBrickSize() const {
  return Vec3ui64(m_dataset->GetMaxBrickSize());
}

Vec3ui64 UVFIO::getMaxUsedBrickSizes() const {
  return Vec3ui64(m_dataset->GetMaxUsedBrickSizes());
}

MinMaxBlock UVFIO::maxMinForKey(const BrickKey& key) const {
  return m_dataset->MaxMinForKey(key);
}

uint64_t UVFIO::getLODLevelCount(uint64_t modality) const {
  return m_dataset->GetLODLevelCount();
}

uint64_t UVFIO::getNumberOfTimesteps() const {
  return m_dataset->GetNumberOfTimesteps();
}

Vec3ui64 UVFIO::getDomainSize(uint64_t lod, uint64_t modality) const {
  // HACK: assume all timesteps have same size
  return m_dataset->GetDomainSize(lod, 0);
}

Core::Math::Vec3f UVFIO::getDomainScale(uint64_t modality) const {
  return Vec3f(m_dataset->GetScale());
}

Mat4d UVFIO::getTransformation(uint64_t) const {
  return Mat4d();
}

Vec3ui UVFIO::getBrickOverlapSize() const {
  return m_dataset->GetBrickOverlapSize();
}

uint64_t UVFIO::getLargestSingleBrickLOD(uint64_t modality) const {
  // HACK: assume all timesteps have same size
  return m_dataset->GetLargestSingleBrickLOD(0);
}

Vec3f UVFIO::getBrickExtents(const BrickKey& key) const {
  return m_dataset->GetBrickExtents(key);
}

Vec3ui64 UVFIO::getEffectiveBricksize() const {
  return getMaxUsedBrickSizes()-Vec3ui64(getBrickOverlapSize()*2);
}

Vec3f UVFIO::getFloatBrickLayout(uint64_t lod, uint64_t modality) const {
  const Vec3ui bricks = getBrickLayout(lod, modality);
  const Vec3ui brickIndex = bricks-Vec3ui(1,1,1);
  const BrickKey k(modality, 0, lod, brickIndex.volume());
  const Vec3ui effectiveLastBrickInLevelSize = getBrickVoxelCounts(k)-getBrickOverlapSize();
  
  Vec3f floatBrickLayout = Vec3f(effectiveLastBrickInLevelSize)/Vec3f(getEffectiveBricksize());
  
  // subtract smallest possible floating point epsilon from
  // integer values that would mess up the brick index computation in the shader
  if (float(uint32_t(floatBrickLayout.x)) == floatBrickLayout.x)
    floatBrickLayout.x -= floatBrickLayout.x * std::numeric_limits<float>::epsilon();
  if (float(uint32_t(floatBrickLayout.y)) == floatBrickLayout.y)
    floatBrickLayout.y -= floatBrickLayout.y * std::numeric_limits<float>::epsilon();
  if (float(uint32_t(floatBrickLayout.z)) == floatBrickLayout.z)
    floatBrickLayout.z -= floatBrickLayout.z * std::numeric_limits<float>::epsilon();
  
  return floatBrickLayout;
}

std::vector<MinMaxBlock> UVFIO::getBrickMaxMin() const {
  // TODO: fixme: add modality and timestep as a parameters for getBrickMaxMin
  const uint64_t modality = 0;
  const uint64_t timestep = 0;
  
  uint64_t iTotalBrickCount = getTotalBrickCount(modality);
  std::vector<MinMaxBlock> result(iTotalBrickCount);
  
  uint64_t levelCount = getLODLevelCount(modality);
  
  size_t i = 0;
  for (uint32_t lod = 0; lod < levelCount; lod++) {
    uint64_t indexInLodCount = getBrickLayout(lod, modality).volume();
    for (uint32_t indexInLod = 0; indexInLod < indexInLodCount; indexInLod++) {
      BrickKey const key(modality, timestep, lod, indexInLodCount);
      result[i++] = maxMinForKey(key);
    }
  }
  
  return result;
}

Vec3ui UVFIO::getBrickLayout(uint64_t lod, uint64_t modality) const {
  // HACK: assume all timesteps have same layout
  return m_dataset->GetBrickLayout(lod, 0);
}

uint64_t UVFIO::getModalityCount() const {
  return 1;
}

uint64_t UVFIO::getComponentCount(uint64_t modality) const {
  return m_dataset->GetComponentCount();
}

uint64_t UVFIO::getDefault1DTransferFunctionCount() const {
  
  const std::string path = getPath(m_filename);
  const std::string filename = removeExt(getFilename(m_filename));
  
  std::vector<std::string> files = getDirContents(path, filename+"*", "1dt");
  
  
  return std::max<uint64_t>(1,files.size());
}

uint64_t UVFIO::getDefault2DTransferFunctionCount() const {
  //TODO
  return 1;
}



TransferFunction1D UVFIO::getDefault1DTransferFunction(uint64_t index) const {
  
  const std::string path = getPath(m_filename);
  const std::string filename = removeExt(getFilename(m_filename));
  
  std::vector<std::string> files = getDirContents(path, filename+"*", "1dt");

  // sort by name
  std::sort(files.begin(), files.end());
  
  if (index < files.size()) {
    const std::string default1DTFFilename = files[index];
    if (fileExists(default1DTFFilename)) {
      TransferFunction1D tf(default1DTFFilename);
      if (tf.getSize() > 0) { // check if something usefull was in the file
        LINFO("(UVFIO) loaded user defined default 1D-tf from file " <<
              default1DTFFilename);
        return tf;
      }
    }
  }
  
  LINFO("(UVFIO) using analytic standard 1D tf ");
  
  TransferFunction1D tf(256);
  tf.setStdFunction();
  return tf;
}

/*
 TransferFunction2D UVFIO::getDefault2DTransferFunction(uint64_t index) const {
   std::string default2DTFFilename = changeExt(m_filename, "2dt");

 
 if (index != 0 )
 throw TrinityError("invalid 2D TF index", __FILE__, __LINE__);

 return
 }
 */

std::vector<uint64_t> UVFIO::get1DHistogram() const {
  //TODO
  return std::vector<uint64_t>();
}

std::vector<uint64_t> UVFIO::get2DHistogram() const {
  //TODO
  return std::vector<uint64_t>();
}

std::string UVFIO::getUserDefinedSemantic(uint64_t modality) const {
  return m_dataset->Name();
}

Vec2f UVFIO::getRange(uint64_t modality) const {
  std::pair<double,double> r = m_dataset->GetRange();
  return Vec2f(r.first, r.second);
}


uint64_t UVFIO::getTotalBrickCount(uint64_t modality) const {
  return m_dataset->GetTotalBrickCount();
}

std::shared_ptr<const std::vector<uint8_t>> UVFIO::getBrick(const BrickKey& key, bool& success) const {
    auto data = MemBlockPool::instance().get(getBrickVoxelCounts(key).volume() *
      m_dataset->GetBitWidth()/8 * m_dataset->GetComponentCount());
    success = m_dataset->GetBrick(key, *data);
    return data;
}

std::vector<std::shared_ptr<const std::vector<uint8_t>>> UVFIO::getBricks(const std::vector<BrickKey>& brickKeys, bool& success) const {
  std::vector<std::shared_ptr<const std::vector<uint8_t>>> result;
  for (auto key : brickKeys) {
    auto data = MemBlockPool::instance().get(getBrickVoxelCounts(key).volume() *
                                             m_dataset->GetBitWidth()/8 * m_dataset->GetComponentCount());
    if (!m_dataset->GetBrick(key, *data)) {
      success = false;
      break;
    }
    result.push_back(data);
  }
  return result;
}

Vec3ui UVFIO::getBrickVoxelCounts(const BrickKey& key) const {
  return m_dataset->GetBrickVoxelCounts(key);
}

IIO::ValueType UVFIO::getType(uint64_t modality) const {
  if (m_dataset->GetIsSigned()) {
    if (m_dataset->GetIsFloat()) {
      switch (m_dataset->GetBitWidth()) {
        case 32 : return ValueType::T_FLOAT;
        case 64 : return ValueType::T_DOUBLE;
        default : throw TrinityError("invalid data type", __FILE__, __LINE__);
      }
    } else { // signed integer
      switch (m_dataset->GetBitWidth()) {
        case 8 : return ValueType::T_INT8;
        case 16 : return ValueType::T_INT16;
        case 32 : return ValueType::T_INT32;
        case 64 : return ValueType::T_INT64;
        default : throw TrinityError("invalid data type", __FILE__, __LINE__);
      }
    }
  } else { // unsigned
    if (m_dataset->GetIsFloat()) {
      throw TrinityError("invalid data type", __FILE__, __LINE__);
    } else { // unsigned integer
      switch (m_dataset->GetBitWidth()) {
        case 8 : return ValueType::T_UINT8;
        case 16 : return ValueType::T_UINT16;
        case 32 : return ValueType::T_UINT32;
        case 64 : return ValueType::T_UINT64;
        default : throw TrinityError("invalid data type", __FILE__, __LINE__);
      }
    }
  }
}

IIO::Semantic UVFIO::getSemantic(uint64_t modality) const {
  if (m_dataset->GetComponentCount() == 1) {
    return Semantic::Scalar;
  } else {
    return Semantic::Color;  // UVF only support scalar or color
  }
}