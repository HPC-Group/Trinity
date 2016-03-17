#pragma once

#include "io-base/IListData.h"

namespace trinity {
class FractalListData : public IListData {
public:
    bool canList(int fileID) const;
    std::vector<IOData> listData(uint32_t dirID = 0) const override;
};
}