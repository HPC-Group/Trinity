#include "commands/BrickMetaData.h"

#include "commands/ISerialReader.h"
#include "commands/ISerialWriter.h"

using namespace trinity;

std::shared_ptr<std::vector<uint8_t>> BrickMetaData::createBinary(const std::vector<BrickMetaData>& metaDataVec) {
    auto binary = std::make_shared<std::vector<uint8_t>>();
    binary->reserve(metaDataVec.size() * (4 * sizeof(double) + 3 * sizeof(uint32_t))); // FIXME: use constexpr instead (requires VS2015)
    for (const auto& metaData : metaDataVec) {
        mocca::net::appendToMessagePart(*binary, metaData.minScalar);
        mocca::net::appendToMessagePart(*binary, metaData.maxScalar);
        mocca::net::appendToMessagePart(*binary, metaData.minGradient);
        mocca::net::appendToMessagePart(*binary, metaData.maxGradient);
        mocca::net::appendToMessagePart(*binary, metaData.voxelSize.x);
        mocca::net::appendToMessagePart(*binary, metaData.voxelSize.y);
        mocca::net::appendToMessagePart(*binary, metaData.voxelSize.z);
    }
    return binary;
}

std::vector<BrickMetaData> BrickMetaData::createFromBinary(std::shared_ptr<std::vector<uint8_t>> binary) {
    std::vector<BrickMetaData> result;
    size_t numElements = binary->size() / (4 * sizeof(double) + 3 * sizeof(uint32_t)); // FIXME: use constexpr instead (requires VS2015)
    const uint8_t* ptr = binary->data();
    for (size_t i = 0; i < numElements; ++i) {
        result.push_back(BrickMetaData());
        ptr = mocca::net::readFromMessagePart(ptr, result.back().minScalar);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().maxScalar);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().minGradient);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().maxGradient);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().voxelSize.x);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().voxelSize.y);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().voxelSize.z);
    }
    return result;
}
