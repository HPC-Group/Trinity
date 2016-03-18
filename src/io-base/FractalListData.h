#pragma once

#include "io-base/IListData.h"
#include "silverbullet/math/Vectors.h"

namespace trinity {
  class FractalListData : public IListData {
  public:
    // IListData interface implementation
    bool canList(const std::string& dirID) const override;
    bool canOpen(const std::string& fileID) const override;
    std::vector<IOData> listData(const std::string& dirID) const override;

    // special fractal functions
    Core::Math::Vec3ui64 totalSize(const std::string& fileID) const;
    Core::Math::Vec3ui64 brickSize(const std::string& fileID) const;
  };
}