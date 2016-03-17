#pragma once

#include "io-base/IListData.h"

namespace trinity {
class FractalListData : public IListData {
public:
    bool canList(const std::string& fileID) const override;
    std::vector<IOData> listData(const std::string& dirID) const override;
};
}