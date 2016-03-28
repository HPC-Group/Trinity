#include <array>
#include "io-base/FractalListData.h"
#include "common/TrinityError.h"
#include "mocca/log/LogManager.h"
#include "fractal/FractalIO.h"

#define FractalDataRoot "FractalData"

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

static std::array<FractalData, 22> data {
  {FractalData(FractalDataRoot, IOData("Flat Data", "FractalData@Flat",
                                       IOData::DataType::Directory)),
    FractalData(FractalDataRoot, IOData("Bricked Data", "FractalData@Bricked",
                                        IOData::DataType::Directory)),
    
    FractalData("FractalData@Flat", IOData("Power of 2", "FractalData@1a",
                                           IOData::DataType::Directory)),
    FractalData("FractalData@Flat", IOData("Non Power of 2", "FractalData@1b",
                                           IOData::DataType::Directory)),
    
    FractalData("FractalData@Bricked", IOData("Power of 2", "FractalData@2a",
                                              IOData::DataType::Directory)),
    FractalData("FractalData@Bricked", IOData("Non Power of 2", "FractalData@2b",
                                              IOData::DataType::Directory)),
    
    // used for testing and dummy purposes. DO NOT REMOVE
    FractalData("FractalData@1a", IOData("DUMMY AND TEST IO DATA", "FractalData@3",
                                         IOData::DataType::Dataset),
                Vec3ui64(64, 64, 64), Vec3ui64(64, 64, 64)),
    
    FractalData("FractalData@1a", IOData("64^3", "FractalData@3a",
                                         IOData::DataType::Dataset),
                Vec3ui64(64, 64, 64), Vec3ui64(64, 64, 64)),
    FractalData("FractalData@1a", IOData("128^3", "FractalData@4a",
                                         IOData::DataType::Dataset),
                Vec3ui64(128, 128, 128), Vec3ui64(128, 128, 128)),
    FractalData("FractalData@1a", IOData("256^3", "FractalData@5a",
                                         IOData::DataType::Dataset),
                Vec3ui64(256, 256, 256), Vec3ui64(256, 256, 256)),
    
    FractalData("FractalData@1b", IOData("64^3", "FractalData@3b",
                                         IOData::DataType::Dataset),
                Vec3ui64(50, 50, 50), Vec3ui64(50, 50, 50)),
    FractalData("FractalData@1b", IOData("128^3", "FractalData@4b",
                                         IOData::DataType::Dataset),
                Vec3ui64(100, 100, 100), Vec3ui64(100, 100, 100)),
    FractalData("FractalData@1b", IOData("256^3", "FractalData@5b",
                                         IOData::DataType::Dataset),
                Vec3ui64(200, 200, 200), Vec3ui64(200, 200, 200)),
    
    FractalData("FractalData@2a", IOData("512^3 (32^3 bricks)", "FractalData@6a",
                                         IOData::DataType::Dataset),
                Vec3ui64(512, 512, 512), Vec3ui64(32, 32, 32)),
    FractalData("FractalData@2a", IOData("1024^3 (32^3 bricks)", "FractalData@7a",
                                         IOData::DataType::Dataset),
                Vec3ui64(1024, 1024, 1024), Vec3ui64(32, 32, 32)),
    FractalData("FractalData@2a", IOData("2048^3 (32^3 bricks)", "FractalData@8a",
                                         IOData::DataType::Dataset),
                Vec3ui64(2048, 2048, 2048), Vec3ui64(32, 32, 32)),
    FractalData("FractalData@2a", IOData("4096^3 (32^3 bricks)", "FractalData@9a",
                                         IOData::DataType::Dataset),
                Vec3ui64(4096, 4096, 4096), Vec3ui64(16, 16, 16)),
    
    FractalData("FractalData@2b", IOData("500^3 (32^3 bricks)", "FractalData@6b",
                                         IOData::DataType::Dataset),
                Vec3ui64(500, 500, 500), Vec3ui64(32, 32, 32)),
    FractalData("FractalData@2b", IOData("1000^3 (32^3 bricks)", "FractalData@7b",
                                         IOData::DataType::Dataset),
                Vec3ui64(1000, 1000, 1000), Vec3ui64(32, 32, 32)),
    FractalData("FractalData@2b", IOData("2000^3 (32^3 bricks)", "FractalData@8b",
                                         IOData::DataType::Dataset),
                Vec3ui64(2000, 2000, 2000), Vec3ui64(32, 32, 32)),
    FractalData("FractalData@2b", IOData("4000^3 (32^3 bricks)", "FractalData@9b",
                                         IOData::DataType::Dataset),
                Vec3ui64(4000, 4000, 4000), Vec3ui64(32, 32, 32)),
    FractalData("FractalData@2b", IOData("4000^3 (30^3 bricks)","FractalData@10b",
                                         IOData::DataType::Dataset),
                Vec3ui64(4000, 4000, 4000), Vec3ui64(30, 30, 30))}
};

bool FractalListData::containsIOData(const std::string& fileOrDirID) const {
  for(const auto& s: data) {
    if (s.m_container == fileOrDirID) return true;
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

Vec3ui64 FractalListData::totalSize(const std::string& fileID) const {
  for(const auto& s: data) {
    if (s.m_ioData.getFileId() == fileID &&
        s.m_ioData.getDataType() == IOData::DataType::Dataset)
      return s.m_size;
  }
  throw TrinityError("invalid file ID", __FILE__, __LINE__);
}


Vec3ui64 FractalListData::brickSize(const std::string& fileID) const {
  for(const auto& s: data) {
    if (s.m_ioData.getFileId() == fileID &&
        s.m_ioData.getDataType() == IOData::DataType::Dataset)
      return s.m_brick;
  }
  throw TrinityError("invalid file ID", __FILE__, __LINE__);
}


std::unique_ptr<trinity::IIO> FractalListData::createIO(const std::string& fileId) const {
  return std::unique_ptr<FractalIO>(new FractalIO(fileId, *this));
}

