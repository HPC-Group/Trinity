#pragma once

#include "commands/IOData.h"

#include <vector>

namespace trinity {

class IListData {
public:
    ~IListData() {}

    virtual bool canList(const std::string& fileID) const = 0;
    virtual std::vector<IOData> listData(const std::string& dirID = 0) const = 0;
};
}