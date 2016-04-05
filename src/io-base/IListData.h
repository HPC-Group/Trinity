#pragma once

#include "commands/IOData.h"
#include "common/IIO.h"

#include <vector>

namespace trinity {

class IListData {
public:
    ~IListData() {}

    virtual bool containsIOData(const std::string& fileID) const = 0;
    virtual std::vector<IOData> listData(const std::string& dirID) const = 0;
    virtual std::unique_ptr<IIO> createIO(const std::string& dirID) const = 0;
    virtual std::string getRoot() const = 0;
};
}