#include "io-base/FractalListData.h"

#include "common/TrinityError.h"

using namespace trinity;

bool FractalListData::canList(const std::string& fileID) const {
    return true; // fixme: must actually check the id
}

std::vector<IOData> FractalListData::listData(const std::string& dirID) const {
    std::vector<IOData> ioDataVec;
    if(dirID == "0") {
        ioDataVec.push_back(IOData("Flat Data", "1", IOData::DataType::Directory));
        ioDataVec.push_back(IOData("Bricked Data", "2", IOData::DataType::Directory));
    } else if(dirID == "1") {
        ioDataVec.push_back(IOData("64^3", "3", IOData::DataType::Directory));
        ioDataVec.push_back(IOData("128^3", "4", IOData::DataType::Directory));
        ioDataVec.push_back(IOData("256^3", "5", IOData::DataType::Directory));
    } else if(dirID == "2") {
        ioDataVec.push_back(IOData("512^3 (32^3 bricks)", "6", IOData::DataType::Directory));
        ioDataVec.push_back(IOData("1024^3 (32^3 bricks)", "7", IOData::DataType::Directory));
        ioDataVec.push_back(IOData("2048^3 (32^3 bricks)", "8", IOData::DataType::Directory));
        ioDataVec.push_back(IOData("4096^3 (32^3 bricks)", "9", IOData::DataType::Directory));
    } else {
        throw TrinityError("Invalid directory ID: " + dirID, __FILE__, __LINE__);
    }
    return ioDataVec;
}