#pragma once

#include "commands/IOData.h"

#include <vector>

namespace trinity {

class IListData {
public:
    ~IListData() {}

    virtual bool canList(int fileID) const = 0;
    virtual std::vector<IOData> listData(uint32_t dirID = 0) const = 0;
};
}