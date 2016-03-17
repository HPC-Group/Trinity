#include "io-base/FractalListData.h"

#include "common/TrinityError.h"

using namespace trinity;

bool FractalListData::canList(int fileID) const {
    return true; // fixme: must actually check the id
}

std::vector<IOData> FractalListData::listData(uint32_t dirID) const {
    std::vector<IOData> ioDataVec;
    switch (dirID) {
    case 0: {
        ioDataVec.push_back(IOData("Flat Data", 1, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("Bricked Data", 2, IOData::DataType::Directory));
        break;
    }
    case 1: {
        ioDataVec.push_back(IOData("64^3", 3, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("128^3", 4, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("256^3", 5, IOData::DataType::Directory));
        break;
    }
    case 2: {
        ioDataVec.push_back(IOData("512^3 (32^3 bricks)", 6, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("1024^3 (32^3 bricks)", 7, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("2048^3 (32^3 bricks)", 8, IOData::DataType::Directory));
        ioDataVec.push_back(IOData("4096^3 (32^3 bricks)", 9, IOData::DataType::Directory));
        break;
    }
    default:
        throw TrinityError("Invalid directory ID: " + std::to_string(dirID), __FILE__, __LINE__);
        break;
    }
    return ioDataVec;
}