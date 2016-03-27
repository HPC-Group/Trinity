#include "UVFIO.h"
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

#define MaxAcceptableBricksize 512

UVFIO::UVFIO(const std::string& fileId, const IListData& listData) :
  dataset(nullptr)
{
  LINFO("(UVFIO) initializing for file id " + fileId);
  const auto uvfListData = dynamic_cast<const UVFListData*>(&listData);

  if (uvfListData) {
    std::string filename = fileId;
    if (!uvfListData->stripListerID(filename)) {
      throw TrinityError(std::string("invalid fileId") + fileId,  __FILE__, __LINE__);
    }

    if (!fileExists(filename)) {
      throw TrinityError(filename + " not found", __FILE__, __LINE__);
    }

    if (isDirectory(filename) || ToLowerCase(getExt(filename)) != "uvf" ) {
      throw TrinityError(filename + " is not a uvf file", __FILE__, __LINE__);
    }
    
    LINFO("(UVFIO) uvf filename looks ok, ready to open");

    dataset = mocca::make_unique<UVFDataset>(filename, MaxAcceptableBricksize, false);
    
    LINFO("(UVFIO) successfully opened uvf file");
    
    if (!dataset->IsSameEndianness()) {
      throw TrinityError("dataset ist stored in incompatible endianness", __FILE__, __LINE__);
    }

  } else {
    throw TrinityError("invalid listData type", __FILE__, __LINE__);
  }
}

Vec3ui64 UVFIO::getMaxBrickSize() const {
  return Vec3ui64(dataset->GetMaxBrickSize());
}

Vec3ui64 UVFIO::getMaxUsedBrickSizes() const {
  return Vec3ui64(dataset->GetMaxUsedBrickSizes());
}

MinMaxBlock UVFIO::maxMinForKey(const BrickKey& key) const {
  return dataset->MaxMinForKey(key);
}

uint64_t UVFIO::getLODLevelCount(uint64_t modality) const {
  return dataset->GetLODLevelCount();
}

uint64_t UVFIO::getNumberOfTimesteps() const {
  return dataset->GetNumberOfTimesteps();
}

Vec3ui64 UVFIO::getDomainSize(uint64_t lod, uint64_t modality) const {
  return dataset->GetDomainSize(lod, 0);  // HACK: assume all timesteps have same size
}

Mat4d UVFIO::getTransformation(uint64_t) const {
  Vec3d scale = dataset->GetScale();
  Mat4d trans;
  trans.Scaling(float(scale.x), float(scale.y), float(scale.z));
  return trans;
}

Vec3ui UVFIO::getBrickOverlapSize() const {
  return dataset->GetBrickOverlapSize();
}

uint64_t UVFIO::getLargestSingleBrickLOD(uint64_t modality) const {
  return dataset->GetLargestSingleBrickLOD(0); // HACK: assume all timesteps have same size
}

Vec3f UVFIO::getBrickExtents(const BrickKey& key) const {
  return dataset->GetBrickExtents(key);
}

Vec3ui UVFIO::getBrickLayout(uint64_t lod, uint64_t modality) const {
  return dataset->GetBrickLayout(lod, 0); // HACK: assume all timesteps have same layout
}

uint64_t UVFIO::getModalityCount() const {
  return 1;
}

uint64_t UVFIO::getComponentCount(uint64_t modality) const {
  return dataset->GetComponentCount();
}

uint64_t UVFIO::getDefault1DTransferFunctionCount() const {
  //TODO
  return 1;
}

uint64_t UVFIO::getDefault2DTransferFunctionCount() const {
  //TODO
  return 1;
}

TransferFunction1D UVFIO::getDefault1DTransferFunction(uint64_t index) const {
  //TODO
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
 TransferFunction2D UVFIO::getDefault2DTransferFunction(uint64_t index) const {
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
  return dataset->Name();
}

Vec2f UVFIO::getRange(uint64_t modality) const {
  std::pair<double,double> r = dataset->GetRange();
  return Vec2f(r.first, r.second);
}


uint64_t UVFIO::getTotalBrickCount(uint64_t modality) const {
  return dataset->GetTotalBrickCount();
}

bool UVFIO::getBrick(const BrickKey& key, std::vector<uint8_t>& data) const{
  return dataset->GetBrick(key, data);
}

Vec3ui UVFIO::getBrickVoxelCounts(const BrickKey& key) const {
  return dataset->GetBrickVoxelCounts(key);
}

IIO::ValueType UVFIO::getType(uint64_t modality) const {
  if (dataset->GetIsSigned()) {
    if (dataset->GetIsFloat()) {
      switch (dataset->GetBitWidth()) {
        case 32 : return ValueType::T_FLOAT;
        case 64 : return ValueType::T_DOUBLE;
        default : throw TrinityError("invalid data type", __FILE__, __LINE__);
      }
    } else { // signed integer
      switch (dataset->GetBitWidth()) {
        case 8 : return ValueType::T_INT8;
        case 16 : return ValueType::T_INT16;
        case 32 : return ValueType::T_INT32;
        case 64 : return ValueType::T_INT64;
        default : throw TrinityError("invalid data type", __FILE__, __LINE__);
      }
    }
  } else { // unsigned
    if (dataset->GetIsFloat()) {
      throw TrinityError("invalid data type", __FILE__, __LINE__);
    } else { // unsigned integer
      switch (dataset->GetBitWidth()) {
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
  if (dataset->GetComponentCount() == 1) {
    return Semantic::Scalar;
  } else {
    return Semantic::Color;  // UVF only support scalar or color
  }
}