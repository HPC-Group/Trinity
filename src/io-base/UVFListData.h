#pragma once

#include "io-base/IListData.h"
#include "silverbullet/math/Vectors.h"

namespace trinity {
class UVFListData : public IListData {
public:
    // IListData interface implementation
    bool containsIOData(const std::string& dirOrFileID) const override;
    std::vector<IOData> listData(const std::string& dirID) const override;
    std::unique_ptr<IIO> createIO(const std::string& dirID) const override;
    std::string getRoot() const override;

    bool stripListerID(std::string& id) const;
};
}