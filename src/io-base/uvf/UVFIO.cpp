#include "io-base/UVFListData.h"
#include "io-base/fractal/UVFIO.h"
#include "silverbullet/io/FileTools.h"
#include "silverbullet/base/StringTools.h"

#include "common/TrinityError.h"
#include "mocca/log/LogManager.h"
#include "mocca/base/Error.h"

using namespace Core::IO::FileTools;
using namespace Core::StringTools;
using namespace trinity;


UVFIO::UVFIO(const std::string& fileId, const IListData& listData) {
  LINFO("(UVFIO) initializing for file id " + fileId);
  const auto uvfListData = dynamic_cast<const UVFListData*>(&listData);

  if (uvfListData) {
    std::string filename = fileId;
    if (!uvfListData.stripListerID(filename)) {
      throw TrinityError("invalid fileId" << fileId,  __FILE__, __LINE__);
    }

    if (!fileExists(filename)) {
      throw TrinityError(filename << " not found", __FILE__, __LINE__);
    }

    if (isDirectory(path) || ToLowerCase(getExt(path)) != "uvf" ) {
      throw TrinityError(filename << " is not a uvf file", __FILE__, __LINE__);
    }
    
    // TODO: open UVF file here
    
    LINFO("(UVFIO) uvf file looks ok, ready to open");

  } else {
    throw TrinityError("invalid listData type", __FILE__, __LINE__);
  }
}

Vec3ui64 UVFIO::getMaxBrickSize() const {
  // TODO
  return Vec3ui64();
}

Vec3ui64 UVFIO::getMaxUsedBrickSizes() const {
  // TODO
  return Vec3ui64();
}

MinMaxBlock UVFIO::maxMinForKey(const BrickKey& key) const {
  // TODO
  return MinMaxBlock(0, 255, 0, 1);
}

uint64_t UVFIO::getLODLevelCount(uint64_t modality) const {
  // TODO
  return 0;
}

uint64_t UVFIO::getNumberOfTimesteps() const {
  // TODO
  return 0;
}

Vec3ui64 UVFIO::getDomainSize(uint64_t lod, uint64_t modality) const {
  // TODO
  return Vec3ui64();
}

Mat4d UVFIO::getTransformation(uint64_t) const {
  // TODO
  return Mat4d();
}

Vec3ui UVFIO::getBrickOverlapSize() const {
  // TODO
  return Vec3ui(0, 0, 0);
}

uint64_t UVFIO::getLargestSingleBrickLOD(uint64_t modality) const {
  //TODO
  return 0;
}

Vec3f UVFIO::getBrickExtents(const BrickKey& key) const {
  //TODO
  return Vec3f(1, 1, 1);
}

Vec3ui UVFIO::getBrickLayout(uint64_t lod, uint64_t modality) const {
  //TODO
  return Vec3ui(1, 1, 1);
}

uint64_t UVFIO::getModalityCount() const {
  //TODO
  return 1;
}

uint64_t UVFIO::getComponentCount(uint64_t modality) const {
  //TODO
  return 1;
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
  //TODO
  return "UVF dataset";
}

Vec2f UVFIO::getRange(uint64_t modality) const {
  //TODO
  return Vec2f(0, 255);
}


uint64_t UVFIO::getTotalBrickCount(uint64_t modality) const {
  //TODO
  return 0;
}

bool UVFIO::getBrick(const BrickKey& key, std::vector<uint8_t>& data) const{
  //TODO
}

IIO::ValueType UVFIO::getType(uint64_t modality) const {
  // TODO
  return ValueType::T_UINT8;
}

IIO::Semantic UVFIO::getSemantic(uint64_t modality) const {
  // TODO
  return Semantic::Scalar;
}