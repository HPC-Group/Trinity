#include <array>
#include "io-base/FractalListData.h"
#include "common/TrinityError.h"
#include "mocca/log/LogManager.h"

using namespace trinity;
using namespace Core::Math;

struct FractalData {
  FractalData(const std::string& container,
              const IOData& ioData,
              Vec3ui64 size = Vec3ui64(),
              Vec3ui64 brick = Vec3ui64()) :
  m_container(container),
  m_ioData(ioData),
  m_size(size),
  m_brick(brick)
  {}


  std::string m_container;
  IOData m_ioData;
  Vec3ui64 m_size;
  Vec3ui64 m_brick;
};

static std::array<FractalData, 10> data {
  FractalData("FractalData", IOData("Flat Data", "FractalData@1", IOData::DataType::Directory)),
  FractalData("FractalData", IOData("Bricked Data", "FractalData@2", IOData::DataType::Directory)),

  FractalData("FractalData@1", IOData("64^3", "FractalData@3", IOData::DataType::Dataset),
              Vec3ui64(64, 64, 64), Vec3ui64(64, 64, 64)),
  FractalData("FractalData@1", IOData("128^3", "FractalData@4", IOData::DataType::Dataset),
              Vec3ui64(128, 128, 128), Vec3ui64(128, 128, 128)),
  FractalData("FractalData@1", IOData("256^3", "FractalData@5", IOData::DataType::Dataset),
              Vec3ui64(256, 256, 256), Vec3ui64(256, 256, 256)),

  FractalData("FractalData@2", IOData("512^3 (32^3 bricks)", "FractalData@6", IOData::DataType::Dataset),
              Vec3ui64(512, 512, 512), Vec3ui64(32, 32, 32)),
  FractalData("FractalData@2", IOData("1024^3 (32^3 bricks)", "FractalData@7", IOData::DataType::Dataset),
              Vec3ui64(1024, 1024, 1024), Vec3ui64(32, 32, 32)),
  FractalData("FractalData@2", IOData("2048^3 (32^3 bricks)", "FractalData@8", IOData::DataType::Dataset),
              Vec3ui64(2048, 2048, 2048), Vec3ui64(32, 32, 32)),
  FractalData("FractalData@2", IOData("4096^3 (32^3 bricks)", "FractalData@9", IOData::DataType::Dataset),
              Vec3ui64(4096, 4096, 4096), Vec3ui64(32, 32, 32)),
  FractalData("FractalData@2", IOData("4096^3 (16^3 bricks)", "FractalData@10", IOData::DataType::Dataset),
              Vec3ui64(4096, 4096, 4096), Vec3ui64(16, 16, 16))
};

bool FractalListData::containsIOData(const std::string& fileOrDirID) const {
  for(const auto& s: data) {
	  if (s.m_ioData.getFileId() == fileOrDirID) return true;
  }
  return false;
}

std::vector<IOData> FractalListData::listData(const std::string& dirID) const {
  std::vector<IOData> ioDataVec;
  LINFO("(fractalio) listData for directory id " + dirID);
  for(const auto& s: data) {
	  if (s.m_container == dirID) {
			  ioDataVec.push_back(s.m_ioData);
	  }		  
  }
  return ioDataVec;
}

Core::Math::Vec3ui64 FractalListData::totalSize(const std::string& fileID) const {
  for(const auto& s: data) {
	  if (s.m_ioData.getFileId() == fileID &&
        s.m_ioData.getDataType() == IOData::DataType::Dataset)
      return s.m_size;
  }
  throw new TrinityError("invalid file ID", __FILE__, __LINE__);
}


Core::Math::Vec3ui64 FractalListData::brickSize(const std::string& fileID) const {
  for(const auto& s: data) {
	  if (s.m_ioData.getFileId() == fileID &&
        s.m_ioData.getDataType() == IOData::DataType::Dataset)
      return s.m_brick;
  }
  throw new TrinityError("invalid file ID", __FILE__, __LINE__);
}
